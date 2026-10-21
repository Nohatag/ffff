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

#ifndef __TBB_parallel_for_H
#define __TBB_parallel_for_H

#include <new>
#include "task.h"
#include "partitioner.h"
#include "blocked_range.h"
#include "tbb_exception.h"
#include "internal/_tbb_trace_impl.h"

namespace tbb {

namespace interface9 {

namespace internal {

    void* allocate_sibling(task* start_for_task, size_t bytes);

    /** @ingroup algorithms */
    template<typename Range, typename Body, typename Partitioner>
    class start_for: public task {
        Range my_range;
        const Body my_body;
        typename Partitioner::task_partition_type my_partition;
        task* execute() __TBB_override;

        void note_affinity( affinity_id id ) __TBB_override {
            my_partition.note_affinity( id );
        }

    public:

        start_for( const Range& range, const Body& body, Partitioner& partitioner ) :
            my_range(range),
            my_body(body),
            my_partition(partitioner)
        {
            tbb::internal::fgt_algorithm(tbb::internal::PARALLEL_FOR_TASK, this, NULL);
        }

        /** parent_ becomes left child.  Newly constructed object is right child. */
        start_for( start_for& parent_, typename Partitioner::split_type& split_obj) :
            my_range(parent_.my_range, split_obj),
            my_body(parent_.my_body),
            my_partition(parent_.my_partition, split_obj)
        {
            my_partition.set_affinity(*this);
            tbb::internal::fgt_algorithm(tbb::internal::PARALLEL_FOR_TASK, this, (void *)&parent_);
        }

        /** parent_ remains left child.  Newly constructed object is right child. */
        start_for( start_for& parent_, const Range& r, depth_t d ) :
            my_range(r),
            my_body(parent_.my_body),
            my_partition(parent_.my_partition, split())
        {
            my_partition.set_affinity(*this);
            my_partition.align_depth( d );
            tbb::internal::fgt_algorithm(tbb::internal::PARALLEL_FOR_TASK, this, (void *)&parent_);
        }
        static void run(  const Range& range, const Body& body, Partitioner& partitioner ) {
            if( !range.empty() ) {
#if !__TBB_TASK_GROUP_CONTEXT || TBB_JOIN_OUTER_TASK_GROUP
                start_for& a = *new(task::allocate_root()) start_for(range,body,partitioner);
#else

                task_group_context context(PARALLEL_FOR);
                start_for& a = *new(task::allocate_root(context)) start_for(range,body,partitioner);
#endif /* __TBB_TASK_GROUP_CONTEXT && !TBB_JOIN_OUTER_TASK_GROUP */

                fgt_begin_algorithm( tbb::internal::PARALLEL_FOR_TASK, (void*)&context );
                task::spawn_root_and_wait(a);
                fgt_end_algorithm( (void*)&context );

            }
        }
#if __TBB_TASK_GROUP_CONTEXT
        static void run(  const Range& range, const Body& body, Partitioner& partitioner, task_group_context& context ) {
            if( !range.empty() ) {
                start_for& a = *new(task::allocate_root(context)) start_for(range,body,partitioner);

                fgt_begin_algorithm( tbb::internal::PARALLEL_FOR_TASK, (void*)&context );
                task::spawn_root_and_wait(a);
                fgt_end_algorithm( (void*)&context );

            }
        }
#endif /* __TBB_TASK_GROUP_CONTEXT */

        void run_body( Range &r ) { 
            fgt_alg_begin_body( tbb::internal::PARALLEL_FOR_TASK, (void *)const_cast<Body*>(&(this->my_body)), (void*)this );
            my_body( r ); 
            fgt_alg_end_body( (void *)const_cast<Body*>(&(this->my_body)) );
        }

        void offer_work(typename Partitioner::split_type& split_obj) {
            spawn( *new( allocate_sibling(static_cast<task*>(this), sizeof(start_for)) ) start_for(*this, split_obj) );
        }

        void offer_work(const Range& r, depth_t d = 0) {
            spawn( *new( allocate_sibling(static_cast<task*>(this), sizeof(start_for)) ) start_for(*this, r, d) );
        }
    };

    inline void* allocate_sibling(task* start_for_task, size_t bytes) {
        task* parent_ptr = new( start_for_task->allocate_continuation() ) flag_task();
        start_for_task->set_parent(parent_ptr);
        parent_ptr->set_ref_count(2);
        return &parent_ptr->allocate_child().allocate(bytes);
    }

    template<typename Range, typename Body, typename Partitioner>
    task* start_for<Range,Body,Partitioner>::execute() {
        my_partition.check_being_stolen( *this );
        my_partition.execute(*this, my_range);
        return NULL;
    }
} 

} 

namespace internal {
    using interface9::internal::start_for;

    template<typename Function, typename Index>
    class parallel_for_body : internal::no_assign {
        const Function &my_func;
        const Index my_begin;
        const Index my_step;
    public:
        parallel_for_body( const Function& _func, Index& _begin, Index& _step )
            : my_func(_func), my_begin(_begin), my_step(_step) {}

        void operator()( const tbb::blocked_range<Index>& r ) const {

            Index b = r.begin();
            Index e = r.end();
            Index ms = my_step;
            Index k = my_begin + b*ms;

#if __INTEL_COMPILER
#pragma ivdep
#if __TBB_ASSERT_ON_VECTORIZATION_FAILURE
#pragma vector always assert
#endif
#endif
            for ( Index i = b; i < e; ++i, k += ms ) {
                my_func( k );
            }
        }
    };
} 

/** \page parallel_for_body_req Requirements on parallel_for body
    Class \c Body implementing the concept of parallel_for body must define:
    - \code Body::Body( const Body& ); \endcode                 Copy constructor
    - \code Body::~Body(); \endcode                             Destructor
    - \code void Body::operator()( Range& r ) const; \endcode   Function call operator applying the body to range \c r.
**/

/** \name parallel_for
    See also requirements on \ref range_req "Range" and \ref parallel_for_body_req "parallel_for Body". **/

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body ) {
    internal::start_for<Range,Body,const __TBB_DEFAULT_PARTITIONER>::run(range,body,__TBB_DEFAULT_PARTITIONER());
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body, const simple_partitioner& partitioner ) {
    internal::start_for<Range,Body,const simple_partitioner>::run(range,body,partitioner);
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body, const auto_partitioner& partitioner ) {
    internal::start_for<Range,Body,const auto_partitioner>::run(range,body,partitioner);
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body, const static_partitioner& partitioner ) {
    internal::start_for<Range,Body,const static_partitioner>::run(range,body,partitioner);
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body, affinity_partitioner& partitioner ) {
    internal::start_for<Range,Body,affinity_partitioner>::run(range,body,partitioner);
}

#if __TBB_TASK_GROUP_CONTEXT

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body, task_group_context& context ) {
    internal::start_for<Range,Body,const __TBB_DEFAULT_PARTITIONER>::run(range, body, __TBB_DEFAULT_PARTITIONER(), context);
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body, const simple_partitioner& partitioner, task_group_context& context ) {
    internal::start_for<Range,Body,const simple_partitioner>::run(range, body, partitioner, context);
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body, const auto_partitioner& partitioner, task_group_context& context ) {
    internal::start_for<Range,Body,const auto_partitioner>::run(range, body, partitioner, context);
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body, const static_partitioner& partitioner, task_group_context& context ) {
    internal::start_for<Range,Body,const static_partitioner>::run(range, body, partitioner, context);
}

/** @ingroup algorithms **/
template<typename Range, typename Body>
void parallel_for( const Range& range, const Body& body, affinity_partitioner& partitioner, task_group_context& context ) {
    internal::start_for<Range,Body,affinity_partitioner>::run(range,body,partitioner, context);
}
#endif /* __TBB_TASK_GROUP_CONTEXT */

namespace strict_ppl {

template <typename Index, typename Function, typename Partitioner>
void parallel_for_impl(Index first, Index last, Index step, const Function& f, Partitioner& partitioner) {
    if (step <= 0 )
        internal::throw_exception(internal::eid_nonpositive_step); 
    else if (last > first) {

        Index end = (last - first - Index(1)) / step + Index(1);
        tbb::blocked_range<Index> range(static_cast<Index>(0), end);
        internal::parallel_for_body<Function, Index> body(f, first, step);
        tbb::parallel_for(range, body, partitioner);
    }
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f) {
    parallel_for_impl<Index,Function,const auto_partitioner>(first, last, step, f, auto_partitioner());
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f, const simple_partitioner& partitioner) {
    parallel_for_impl<Index,Function,const simple_partitioner>(first, last, step, f, partitioner);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f, const auto_partitioner& partitioner) {
    parallel_for_impl<Index,Function,const auto_partitioner>(first, last, step, f, partitioner);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f, const static_partitioner& partitioner) {
    parallel_for_impl<Index,Function,const static_partitioner>(first, last, step, f, partitioner);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f, affinity_partitioner& partitioner) {
    parallel_for_impl(first, last, step, f, partitioner);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f) {
    parallel_for_impl<Index,Function,const auto_partitioner>(first, last, static_cast<Index>(1), f, auto_partitioner());
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f, const simple_partitioner& partitioner) {
    parallel_for_impl<Index,Function,const simple_partitioner>(first, last, static_cast<Index>(1), f, partitioner);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f, const auto_partitioner& partitioner) {
    parallel_for_impl<Index,Function,const auto_partitioner>(first, last, static_cast<Index>(1), f, partitioner);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f, const static_partitioner& partitioner) {
    parallel_for_impl<Index,Function,const static_partitioner>(first, last, static_cast<Index>(1), f, partitioner);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f, affinity_partitioner& partitioner) {
    parallel_for_impl(first, last, static_cast<Index>(1), f, partitioner);
}

#if __TBB_TASK_GROUP_CONTEXT

template <typename Index, typename Function, typename Partitioner>
void parallel_for_impl(Index first, Index last, Index step, const Function& f, Partitioner& partitioner, tbb::task_group_context &context) {
    if (step <= 0 )
        internal::throw_exception(internal::eid_nonpositive_step); 
    else if (last > first) {

        Index end = (last - first - Index(1)) / step + Index(1);
        tbb::blocked_range<Index> range(static_cast<Index>(0), end);
        internal::parallel_for_body<Function, Index> body(f, first, step);
        tbb::parallel_for(range, body, partitioner, context);
    }
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f, tbb::task_group_context &context) {
    parallel_for_impl<Index,Function,const auto_partitioner>(first, last, step, f, auto_partitioner(), context);
}

 template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f, const simple_partitioner& partitioner, tbb::task_group_context &context) {
    parallel_for_impl<Index,Function,const simple_partitioner>(first, last, step, f, partitioner, context);
}

 template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f, const auto_partitioner& partitioner, tbb::task_group_context &context) {
    parallel_for_impl<Index,Function,const auto_partitioner>(first, last, step, f, partitioner, context);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f, const static_partitioner& partitioner, tbb::task_group_context &context) {
    parallel_for_impl<Index,Function,const static_partitioner>(first, last, step, f, partitioner, context);
}

 template <typename Index, typename Function>
void parallel_for(Index first, Index last, Index step, const Function& f, affinity_partitioner& partitioner, tbb::task_group_context &context) {
    parallel_for_impl(first, last, step, f, partitioner, context);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f, tbb::task_group_context &context) {
    parallel_for_impl<Index,Function,const auto_partitioner>(first, last, static_cast<Index>(1), f, auto_partitioner(), context);
}

 template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f, const simple_partitioner& partitioner, tbb::task_group_context &context) {
    parallel_for_impl<Index,Function,const simple_partitioner>(first, last, static_cast<Index>(1), f, partitioner, context);
}

 template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f, const auto_partitioner& partitioner, tbb::task_group_context &context) {
    parallel_for_impl<Index,Function,const auto_partitioner>(first, last, static_cast<Index>(1), f, partitioner, context);
}

template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f, const static_partitioner& partitioner, tbb::task_group_context &context) {
    parallel_for_impl<Index,Function,const static_partitioner>(first, last, static_cast<Index>(1), f, partitioner, context);
}

 template <typename Index, typename Function>
void parallel_for(Index first, Index last, const Function& f, affinity_partitioner& partitioner, tbb::task_group_context &context) {
    parallel_for_impl(first, last, static_cast<Index>(1), f, partitioner, context);
}

#endif /* __TBB_TASK_GROUP_CONTEXT */

} 

using strict_ppl::parallel_for;

} 

#if TBB_PREVIEW_SERIAL_SUBSET
#define __TBB_NORMAL_EXECUTION
#include "../serial/tbb/parallel_for.h"
#undef __TBB_NORMAL_EXECUTION
#endif

#endif /* __TBB_parallel_for_H */
