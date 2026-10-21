// Leaked By Cry Service & Siraj
/*
    Copyright (c) 2005-2018 Intel Corporation

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/

#ifndef __TBB_parallel_reduce_H
#define __TBB_parallel_reduce_H

#include <new>
#include "task.h"
#include "aligned_space.h"
#include "partitioner.h"
#include "tbb_profiling.h"

namespace tbb {

namespace interface9 {

namespace internal {

    using namespace tbb::internal;

    /** Values for reduction_context. */
    enum {
        root_task, left_child, right_child
    };

    /** Represented as a char, not enum, for compactness. */
    typedef char reduction_context;

    /** @ingroup algorithms */
    template<typename Body>
    class finish_reduce: public flag_task {

        bool has_right_zombie;
        const reduction_context my_context;
        Body* my_body;
        aligned_space<Body> zombie_space;
        finish_reduce( reduction_context context_ ) :
            has_right_zombie(false), 
            my_context(context_),
            my_body(NULL)
        {
        }
        ~finish_reduce() {
            if( has_right_zombie )
                zombie_space.begin()->~Body();
        }
        task* execute() __TBB_override {
            if( has_right_zombie ) {

                Body* s = zombie_space.begin();
                my_body->join( *s );

            }
            if( my_context==left_child )
                itt_store_word_with_release( static_cast<finish_reduce*>(parent())->my_body, my_body );
            return NULL;
        }
        template<typename Range,typename Body_, typename Partitioner>
        friend class start_reduce;
    };

    void allocate_sibling(task* start_reduce_task, task *tasks[], size_t start_bytes, size_t finish_bytes);

    /** @ingroup algorithms */
    template<typename Range, typename Body, typename Partitioner>
    class start_reduce: public task {
        typedef finish_reduce<Body> finish_type;
        Body* my_body;
        Range my_range;
        typename Partitioner::task_partition_type my_partition;
        reduction_context my_context;
        task* execute() __TBB_override;

        void note_affinity( affinity_id id ) __TBB_override {
            my_partition.note_affinity( id );
        }
        template<typename Body_>
        friend class finish_reduce;

public:

        start_reduce( const Range& range, Body* body, Partitioner& partitioner ) :
            my_body(body),
            my_range(range),
            my_partition(partitioner),
            my_context(root_task)
        {
        }

        /** parent_ becomes left child.  Newly constructed object is right child. */
        start_reduce( start_reduce& parent_, typename Partitioner::split_type& split_obj ) :
            my_body(parent_.my_body),
            my_range(parent_.my_range, split_obj),
            my_partition(parent_.my_partition, split_obj),
            my_context(right_child)
        {
            my_partition.set_affinity(*this);
            parent_.my_context = left_child;
        }

        /** parent_ remains left child.  Newly constructed object is right child. */
        start_reduce( start_reduce& parent_, const Range& r, depth_t d ) :
            my_body(parent_.my_body),
            my_range(r),
            my_partition(parent_.my_partition, split()),
            my_context(right_child)
        {
            my_partition.set_affinity(*this);
            my_partition.align_depth( d ); 
            parent_.my_context = left_child;
        }
        static void run( const Range& range, Body& body, Partitioner& partitioner ) {
            if( !range.empty() ) {
#if !__TBB_TASK_GROUP_CONTEXT || TBB_JOIN_OUTER_TASK_GROUP
                task::spawn_root_and_wait( *new(task::allocate_root()) start_reduce(range,&body,partitioner) );
#else

                task_group_context context(PARALLEL_REDUCE);
                task::spawn_root_and_wait( *new(task::allocate_root(context)) start_reduce(range,&body,partitioner) );
#endif /* __TBB_TASK_GROUP_CONTEXT && !TBB_JOIN_OUTER_TASK_GROUP */
            }
        }
#if __TBB_TASK_GROUP_CONTEXT
        static void run( const Range& range, Body& body, Partitioner& partitioner, task_group_context& context ) {
            if( !range.empty() )
                task::spawn_root_and_wait( *new(task::allocate_root(context)) start_reduce(range,&body,partitioner) );
        }
#endif /* __TBB_TASK_GROUP_CONTEXT */

        void run_body( Range &r ) { (*my_body)( r ); }

        void offer_work(typename Partitioner::split_type& split_obj) {
            task *tasks[2];
            allocate_sibling(static_cast<task*>(this), tasks, sizeof(start_reduce), sizeof(finish_type));
            new((void*)tasks[0]) finish_type(my_context);
            new((void*)tasks[1]) start_reduce(*this, split_obj);
            spawn(*tasks[1]);
        }

        void offer_work(const Range& r, depth_t d = 0) {
            task *tasks[2];
            allocate_sibling(static_cast<task*>(this), tasks, sizeof(start_reduce), sizeof(finish_type));
            new((void*)tasks[0]) finish_type(my_context);
            new((void*)tasks[1]) start_reduce(*this, r, d);
            spawn(*tasks[1]);
        }
    };

    inline void allocate_sibling(task* start_reduce_task, task *tasks[], size_t start_bytes, size_t finish_bytes) {
        tasks[0] = &start_reduce_task->allocate_continuation().allocate(finish_bytes);
        start_reduce_task->set_parent(tasks[0]);
        tasks[0]->set_ref_count(2);
        tasks[1] = &tasks[0]->allocate_child().allocate(start_bytes);
    }

    template<typename Range, typename Body, typename Partitioner>
    task* start_reduce<Range,Body,Partitioner>::execute() {
        my_partition.check_being_stolen( *this );
        if( my_context==right_child ) {
            finish_type* parent_ptr = static_cast<finish_type*>(parent());
            if( !itt_load_word_with_acquire(parent_ptr->my_body) ) { 
                my_body = new( parent_ptr->zombie_space.begin() ) Body(*my_body,split());
                parent_ptr->has_right_zombie = true;
            }
        } else __TBB_ASSERT(my_context==root_task,NULL);
        my_partition.execute(*this, my_range);
        if( my_context==left_child ) {
            finish_type* parent_ptr = static_cast<finish_type*>(parent());
            __TBB_ASSERT(my_body!=parent_ptr->zombie_space.begin(),NULL);
            itt_store_word_with_release(parent_ptr->my_body, my_body );
        }
        return NULL;
    }

    /** @ingroup algorithms */
    template<typename Body>
    class finish_deterministic_reduce: public task {
        Body &my_left_body;
        Body my_right_body;

        finish_deterministic_reduce( Body &body ) :
            my_left_body( body ),
            my_right_body( body, split() )
        {
        }
        task* execute() __TBB_override {
            my_left_body.join( my_right_body );
            return NULL;
        }
        template<typename Range,typename Body_, typename Partitioner>
        friend class start_deterministic_reduce;
    };

    /** @ingroup algorithms */
    template<typename Range, typename Body, typename Partitioner>
    class start_deterministic_reduce: public task {
        typedef finish_deterministic_reduce<Body> finish_type;
        Body &my_body;
        Range my_range;
        typename Partitioner::task_partition_type my_partition;
        task* execute() __TBB_override;

        start_deterministic_reduce( const Range& range, Body& body, Partitioner& partitioner ) :
            my_body( body ),
            my_range( range ),
            my_partition( partitioner )
        {
        }

        /** parent_ becomes left child.  Newly constructed object is right child. */
        start_deterministic_reduce( start_deterministic_reduce& parent_, finish_type& c, typename Partitioner::split_type& split_obj ) :
            my_body( c.my_right_body ),
            my_range( parent_.my_range, split_obj ),
            my_partition( parent_.my_partition, split_obj )
        {
        }

public:
        static void run( const Range& range, Body& body, Partitioner& partitioner ) {
            if( !range.empty() ) {
#if !__TBB_TASK_GROUP_CONTEXT || TBB_JOIN_OUTER_TASK_GROUP
                task::spawn_root_and_wait( *new(task::allocate_root()) start_deterministic_reduce(range,&body,partitioner) );
#else

                task_group_context context(PARALLEL_REDUCE);
                task::spawn_root_and_wait( *new(task::allocate_root(context)) start_deterministic_reduce(range,body,partitioner) );
#endif /* __TBB_TASK_GROUP_CONTEXT && !TBB_JOIN_OUTER_TASK_GROUP */
            }
        }
#if __TBB_TASK_GROUP_CONTEXT
        static void run( const Range& range, Body& body, Partitioner& partitioner, task_group_context& context ) {
            if( !range.empty() )
                task::spawn_root_and_wait( *new(task::allocate_root(context)) start_deterministic_reduce(range,body,partitioner) );
        }
#endif /* __TBB_TASK_GROUP_CONTEXT */

        void offer_work( typename Partitioner::split_type& split_obj) {
            task* tasks[2];
            allocate_sibling(static_cast<task*>(this), tasks, sizeof(start_deterministic_reduce), sizeof(finish_type));
            new((void*)tasks[0]) finish_type(my_body);
            new((void*)tasks[1]) start_deterministic_reduce(*this, *static_cast<finish_type*>(tasks[0]), split_obj);
            spawn(*tasks[1]);
        }

        void run_body( Range &r ) { my_body(r); }
    };

    template<typename Range, typename Body, typename Partitioner>
    task* start_deterministic_reduce<Range,Body, Partitioner>::execute() {
        my_partition.execute(*this, my_range);
        return NULL;
    }
} 

} 

namespace internal {
    using interface9::internal::start_reduce;
    using interface9::internal::start_deterministic_reduce;

    /** The adaptor class that implements \ref parallel_reduce_body_req "parallel_reduce Body"
        using given \ref parallel_reduce_lambda_req "anonymous function objects".
     **/
    /** @ingroup algorithms */
    template<typename Range, typename Value, typename RealBody, typename Reduction>
    class lambda_reduce_body {

        const Value&     identity_element;
        const RealBody&  my_real_body;
        const Reduction& my_reduction;
        Value            my_value;
        lambda_reduce_body& operator= ( const lambda_reduce_body& other );
    public:
        lambda_reduce_body( const Value& identity, const RealBody& body, const Reduction& reduction )
            : identity_element(identity)
            , my_real_body(body)
            , my_reduction(reduction)
            , my_value(identity)
        { }
        lambda_reduce_body( const lambda_reduce_body& other )
            : identity_element(other.identity_element)
            , my_real_body(other.my_real_body)
            , my_reduction(other.my_reduction)
            , my_value(other.my_value)
        { }
        lambda_reduce_body( lambda_reduce_body& other, tbb::split )
            : identity_element(other.identity_element)
            , my_real_body(other.my_real_body)
            , my_reduction(other.my_reduction)
            , my_value(other.identity_element)
        { }
        void operator()(Range& range) {
            my_value = my_real_body(range, const_cast<const Value&>(my_value));
        }
        void join( lambda_reduce_body& rhs ) {
            my_value = my_reduction(const_cast<const Value&>(my_value), const_cast<const Value&>(rhs.my_value));
        }
        Value result() const {
            return my_value;
        }
    };

} 

/** \page parallel_reduce_body_req Requirements on parallel_reduce body
    Class \c Body implementing the concept of parallel_reduce body must define:
    - \code Body::Body( Body&, split ); \endcode        Splitting constructor.
                                                        Must be able to run concurrently with operator() and method \c join
    - \code Body::~Body(); \endcode                     Destructor
    - \code void Body::operator()( Range& r ); \endcode Function call operator applying body to range \c r
                                                        and accumulating the result
    - \code void Body::join( Body& b ); \endcode        Join results.
                                                        The result in \c b should be merged into the result of \c this
**/

/** \page parallel_reduce_lambda_req Requirements on parallel_reduce anonymous function objects (lambda functions)
    TO BE DOCUMENTED
**/

/** \name parallel_reduce
    See also requirements on \ref range_req "Range" and \ref parallel_reduce_body_req "parallel_reduce Body". **/

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body ) {
    internal::start_reduce<Range,Body, const __TBB_DEFAULT_PARTITIONER>::run( range, body, __TBB_DEFAULT_PARTITIONER() );
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body, const simple_partitioner& partitioner ) {
    internal::start_reduce<Range,Body,const simple_partitioner>::run( range, body, partitioner );
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body, const auto_partitioner& partitioner ) {
    internal::start_reduce<Range,Body,const auto_partitioner>::run( range, body, partitioner );
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body, const static_partitioner& partitioner ) {
    internal::start_reduce<Range,Body,const static_partitioner>::run( range, body, partitioner );
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body, affinity_partitioner& partitioner ) {
    internal::start_reduce<Range,Body,affinity_partitioner>::run( range, body, partitioner );
}

#if __TBB_TASK_GROUP_CONTEXT

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body, task_group_context& context ) {
    internal::start_reduce<Range,Body,const __TBB_DEFAULT_PARTITIONER>::run( range, body, __TBB_DEFAULT_PARTITIONER(), context );
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body, const simple_partitioner& partitioner, task_group_context& context ) {
    internal::start_reduce<Range,Body,const simple_partitioner>::run( range, body, partitioner, context );
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body, const auto_partitioner& partitioner, task_group_context& context ) {
    internal::start_reduce<Range,Body,const auto_partitioner>::run( range, body, partitioner, context );
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body, const static_partitioner& partitioner, task_group_context& context ) {
    internal::start_reduce<Range,Body,const static_partitioner>::run( range, body, partitioner, context );
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_reduce( const Range& range, Body& body, affinity_partitioner& partitioner, task_group_context& context ) {
    internal::start_reduce<Range,Body,affinity_partitioner>::run( range, body, partitioner, context );
}
#endif /* __TBB_TASK_GROUP_CONTEXT */

/** parallel_reduce overloads that work with anonymous function objects
    (see also \ref parallel_reduce_lambda_req "requirements on parallel_reduce anonymous function objects"). **/

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,const __TBB_DEFAULT_PARTITIONER>
                          ::run(range, body, __TBB_DEFAULT_PARTITIONER() );
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
                       const simple_partitioner& partitioner ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,const simple_partitioner>
                          ::run(range, body, partitioner );
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
                       const auto_partitioner& partitioner ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,const auto_partitioner>
                          ::run( range, body, partitioner );
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
                       const static_partitioner& partitioner ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,const static_partitioner>
                                        ::run( range, body, partitioner );
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
                       affinity_partitioner& partitioner ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,affinity_partitioner>
                                        ::run( range, body, partitioner );
    return body.result();
}

#if __TBB_TASK_GROUP_CONTEXT

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
                       task_group_context& context ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,const __TBB_DEFAULT_PARTITIONER>
                          ::run( range, body, __TBB_DEFAULT_PARTITIONER(), context );
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
                       const simple_partitioner& partitioner, task_group_context& context ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,const simple_partitioner>
                          ::run( range, body, partitioner, context );
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
                       const auto_partitioner& partitioner, task_group_context& context ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,const auto_partitioner>
                          ::run( range, body, partitioner, context );
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
                       const static_partitioner& partitioner, task_group_context& context ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,const static_partitioner>
                                        ::run( range, body, partitioner, context );
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
                       affinity_partitioner& partitioner, task_group_context& context ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>,affinity_partitioner>
                                        ::run( range, body, partitioner, context );
    return body.result();
}
#endif /* __TBB_TASK_GROUP_CONTEXT */

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_deterministic_reduce( const Range& range, Body& body ) {
    internal::start_deterministic_reduce<Range, Body, const simple_partitioner>::run(range, body, simple_partitioner());
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_deterministic_reduce( const Range& range, Body& body, const simple_partitioner& partitioner ) {
    internal::start_deterministic_reduce<Range, Body, const simple_partitioner>::run(range, body, partitioner);
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_deterministic_reduce( const Range& range, Body& body, const static_partitioner& partitioner ) {
    internal::start_deterministic_reduce<Range, Body, const static_partitioner>::run(range, body, partitioner);
}

#if __TBB_TASK_GROUP_CONTEXT

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_deterministic_reduce( const Range& range, Body& body, task_group_context& context ) {
    internal::start_deterministic_reduce<Range,Body, const simple_partitioner>::run( range, body, simple_partitioner(), context );
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_deterministic_reduce( const Range& range, Body& body, const simple_partitioner& partitioner, task_group_context& context ) {
    internal::start_deterministic_reduce<Range, Body, const simple_partitioner>::run(range, body, partitioner, context);
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_deterministic_reduce( const Range& range, Body& body, const static_partitioner& partitioner, task_group_context& context ) {
    internal::start_deterministic_reduce<Range, Body, const static_partitioner>::run(range, body, partitioner, context);
}
#endif /* __TBB_TASK_GROUP_CONTEXT */

/** parallel_reduce overloads that work with anonymous function objects
    (see also \ref parallel_reduce_lambda_req "requirements on parallel_reduce anonymous function objects"). **/

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_deterministic_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction ) {
    return parallel_deterministic_reduce(range, identity, real_body, reduction, simple_partitioner());
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_deterministic_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction, const simple_partitioner& partitioner ) {
    internal::lambda_reduce_body<Range,Value,RealBody,Reduction> body(identity, real_body, reduction);
    internal::start_deterministic_reduce<Range,internal::lambda_reduce_body<Range,Value,RealBody,Reduction>, const simple_partitioner>
                          ::run(range, body, partitioner);
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_deterministic_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction, const static_partitioner& partitioner ) {
    internal::lambda_reduce_body<Range, Value, RealBody, Reduction> body(identity, real_body, reduction);
    internal::start_deterministic_reduce<Range, internal::lambda_reduce_body<Range, Value, RealBody, Reduction>, const static_partitioner>
        ::run(range, body, partitioner);
    return body.result();
}
#if __TBB_TASK_GROUP_CONTEXT

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_deterministic_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
    task_group_context& context ) {
    return parallel_deterministic_reduce(range, identity, real_body, reduction, simple_partitioner(), context);
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_deterministic_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
    const simple_partitioner& partitioner, task_group_context& context ) {
    internal::lambda_reduce_body<Range, Value, RealBody, Reduction> body(identity, real_body, reduction);
    internal::start_deterministic_reduce<Range, internal::lambda_reduce_body<Range, Value, RealBody, Reduction>, const simple_partitioner>
        ::run(range, body, partitioner, context);
    return body.result();
}

/** @ingroup algorithms **/
template<typename Range, typename Value, typename RealBody, typename Reduction>
Value parallel_deterministic_reduce( const Range& range, const Value& identity, const RealBody& real_body, const Reduction& reduction,
    const static_partitioner& partitioner, task_group_context& context ) {
    internal::lambda_reduce_body<Range, Value, RealBody, Reduction> body(identity, real_body, reduction);
    internal::start_deterministic_reduce<Range, internal::lambda_reduce_body<Range, Value, RealBody, Reduction>, const static_partitioner>
        ::run(range, body, partitioner, context);
    return body.result();
}
#endif /* __TBB_TASK_GROUP_CONTEXT */

} 

#endif /* __TBB_parallel_reduce_H */
