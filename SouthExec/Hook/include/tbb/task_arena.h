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

#ifndef __TBB_task_arena_H
#define __TBB_task_arena_H

#include "task.h"
#include "tbb_exception.h"
#include "internal/_template_helpers.h"
#if TBB_USE_THREADING_TOOLS
#include "atomic.h" 
#endif
#include "aligned_space.h"

namespace tbb {

namespace this_task_arena {
    int max_concurrency();
} 

namespace internal {

    /** @ingroup task_scheduling */
    class arena;
    class task_scheduler_observer_v3;
} 

namespace interface7 {
class task_arena;

namespace internal {
using namespace tbb::internal; 

class delegate_base : no_assign {
public:
    virtual void operator()() const = 0;
    virtual ~delegate_base() {}
};

template <typename F>
struct return_type_or_void {
#if __TBB_CPP11_DECLTYPE_PRESENT && !__TBB_CPP11_DECLTYPE_OF_FUNCTION_RETURN_TYPE_BROKEN
    typedef decltype(declval<F>()()) type;
#else
    typedef void type;
#endif
};

template<typename F, typename R>
class delegated_function : public delegate_base {
    F &my_func;
    tbb::aligned_space<R> my_return_storage;

    void operator()() const __TBB_override {
        new (my_return_storage.begin()) R(my_func());
    }
public:
    delegated_function(F& f) : my_func(f) {}

    R consume_result() const {
        return tbb::internal::move(*(my_return_storage.begin()));
    }
    ~delegated_function() {
        my_return_storage.begin()->~R();
    }
};

template<typename F>
class delegated_function<F,void> : public delegate_base {
    F &my_func;
    void operator()() const __TBB_override {
        my_func();
    }
public:
    delegated_function(F& f) : my_func(f) {}
    void consume_result() const {}

    friend class task_arena_base;
};

class task_arena_base {
protected:

    internal::arena* my_arena;

#if __TBB_TASK_GROUP_CONTEXT

    task_group_context *my_context;
#endif

    int my_max_concurrency;

    unsigned my_master_slots;

    intptr_t my_version_and_traits;

    enum {
        default_flags = 0
#if __TBB_TASK_GROUP_CONTEXT
        | (task_group_context::default_traits & task_group_context::exact_exception)  
        , exact_exception_flag = task_group_context::exact_exception 
#endif
    };

    task_arena_base(int max_concurrency, unsigned reserved_for_masters)
        : my_arena(0)
#if __TBB_TASK_GROUP_CONTEXT
        , my_context(0)
#endif
        , my_max_concurrency(max_concurrency)
        , my_master_slots(reserved_for_masters)
        , my_version_and_traits(default_flags)
        {}

    void __TBB_EXPORTED_METHOD internal_initialize();
    void __TBB_EXPORTED_METHOD internal_terminate();
    void __TBB_EXPORTED_METHOD internal_attach();
    void __TBB_EXPORTED_METHOD internal_enqueue( task&, intptr_t ) const;
    void __TBB_EXPORTED_METHOD internal_execute( delegate_base& ) const;
    void __TBB_EXPORTED_METHOD internal_wait() const;
    static int __TBB_EXPORTED_FUNC internal_current_slot();
    static int __TBB_EXPORTED_FUNC internal_max_concurrency( const task_arena * );
public:

    static const int automatic = -1;
    static const int not_initialized = -2;

};

#if __TBB_TASK_ISOLATION
void __TBB_EXPORTED_FUNC isolate_within_arena( delegate_base& d, intptr_t reserved = 0 );

template<typename R, typename F>
R isolate_impl(F& f) {
    delegated_function<F, R> d(f);
    isolate_within_arena(d);
    return d.consume_result();
}
#endif /* __TBB_TASK_ISOLATION */
} 

/** 1-to-1 proxy representation class of scheduler's arena
 * Constructors set up settings only, real construction is deferred till the first method invocation
 * Destructor only removes one of the references to the inner arena representation.
 * Final destruction happens when all the references (and the work) are gone.
 */
class task_arena : public internal::task_arena_base {
    friend class tbb::internal::task_scheduler_observer_v3;
    friend int tbb::this_task_arena::max_concurrency();
    bool my_initialized;
    void mark_initialized() {
        __TBB_ASSERT( my_arena, "task_arena initialization is incomplete" );
#if __TBB_TASK_GROUP_CONTEXT
        __TBB_ASSERT( my_context, "task_arena initialization is incomplete" );
#endif
#if TBB_USE_THREADING_TOOLS

        internal::as_atomic(my_initialized).fetch_and_store<release>(true);
#else
        my_initialized = true;
#endif
    }

    template<typename F>
    void enqueue_impl( __TBB_FORWARDING_REF(F) f
#if __TBB_TASK_PRIORITY
        , priority_t p = priority_t(0)
#endif
    ) {
#if !__TBB_TASK_PRIORITY
        intptr_t p = 0;
#endif
        initialize();
#if __TBB_TASK_GROUP_CONTEXT
        internal_enqueue(*new(task::allocate_root(*my_context)) internal::function_task< typename internal::strip<F>::type >(internal::forward<F>(f)), p);
#else
        internal_enqueue(*new(task::allocate_root()) internal::function_task< typename internal::strip<F>::type >(internal::forward<F>(f)), p);
#endif /* __TBB_TASK_GROUP_CONTEXT */
    }

    template<typename R, typename F>
    R execute_impl(F& f) {
        initialize();
        internal::delegated_function<F, R> d(f);
        internal_execute(d);
        return d.consume_result();
    }

public:

    /** Sets up settings only, real construction is deferred till the first method invocation
     *  @arg max_concurrency specifies total number of slots in arena where threads work
     *  @arg reserved_for_masters specifies number of slots to be used by master threads only.
     *       Value of 1 is default and reflects behavior of implicit arenas.
     **/
    task_arena(int max_concurrency_ = automatic, unsigned reserved_for_masters = 1)
        : task_arena_base(max_concurrency_, reserved_for_masters)
        , my_initialized(false)
    {}

    task_arena(const task_arena &s) 
        : task_arena_base(s.my_max_concurrency, s.my_master_slots)
        , my_initialized(false)
    {}

    struct attach {};

    explicit task_arena( attach )
        : task_arena_base(automatic, 1) 
        , my_initialized(false)
    {
        internal_attach();
        if( my_arena ) my_initialized = true;
    }

    inline void initialize() {
        if( !my_initialized ) {
            internal_initialize();
            mark_initialized();
        }
    }

    inline void initialize(int max_concurrency_, unsigned reserved_for_masters = 1) {

        __TBB_ASSERT(!my_arena, "Impossible to modify settings of an already initialized task_arena");
        if( !my_initialized ) {
            my_max_concurrency = max_concurrency_;
            my_master_slots = reserved_for_masters;
            initialize();
        }
    }

    inline void initialize(attach) {

        __TBB_ASSERT(!my_arena, "Impossible to modify settings of an already initialized task_arena");
        if( !my_initialized ) {
            internal_attach();
            if ( !my_arena ) internal_initialize();
            mark_initialized();
        }
    }

    inline void terminate() {
        if( my_initialized ) {
            internal_terminate();
            my_initialized = false;
        }
    }

    ~task_arena() {
        terminate();
    }

    bool is_active() const { return my_initialized; }

#if __TBB_CPP11_RVALUE_REF_PRESENT
    template<typename F>
    void enqueue( F&& f ) {
        enqueue_impl(std::forward<F>(f));
    }
#else
    template<typename F>
    void enqueue( const F& f ) {
        enqueue_impl(f);
    }
#endif

#if __TBB_TASK_PRIORITY

    template<typename F>
#if __TBB_CPP11_RVALUE_REF_PRESENT
    void enqueue( F&& f, priority_t p ) {
#if __TBB_PREVIEW_CRITICAL_TASKS
        __TBB_ASSERT(p == priority_low || p == priority_normal || p == priority_high
                     || p == internal::priority_critical, "Invalid priority level value");
#else
        __TBB_ASSERT(p == priority_low || p == priority_normal || p == priority_high, "Invalid priority level value");
#endif
        enqueue_impl(std::forward<F>(f), p);
    }
#else
    void enqueue( const F& f, priority_t p ) {
#if __TBB_PREVIEW_CRITICAL_TASKS
        __TBB_ASSERT(p == priority_low || p == priority_normal || p == priority_high
                     || p == internal::priority_critical, "Invalid priority level value");
#else
        __TBB_ASSERT(p == priority_low || p == priority_normal || p == priority_high, "Invalid priority level value");
#endif
        enqueue_impl(f,p);
    }
#endif
#endif

    template<typename F>
    typename internal::return_type_or_void<F>::type execute(F& f) {
        return execute_impl<typename internal::return_type_or_void<F>::type>(f);
    }

    template<typename F>
    typename internal::return_type_or_void<F>::type execute(const F& f) {
        return execute_impl<typename internal::return_type_or_void<F>::type>(f);
    }

#if __TBB_EXTRA_DEBUG

    void debug_wait_until_empty() {
        initialize();
        internal_wait();
    }
#endif 

    inline static int current_thread_index() {
        return internal_current_slot();
    }

    inline int max_concurrency() const {

        return (my_max_concurrency>1) ? my_max_concurrency : internal_max_concurrency(this);
    }
};

#if __TBB_TASK_ISOLATION
namespace this_task_arena {

    template<typename F>
    typename internal::return_type_or_void<F>::type isolate(F& f) {
        return internal::isolate_impl<typename internal::return_type_or_void<F>::type>(f);
    }

    template<typename F>
    typename internal::return_type_or_void<F>::type isolate(const F& f) {
        return internal::isolate_impl<typename internal::return_type_or_void<F>::type>(f);
    }
}
#endif /* __TBB_TASK_ISOLATION */
} 

using interface7::task_arena;
#if __TBB_TASK_ISOLATION
namespace this_task_arena {
    using namespace interface7::this_task_arena;
}
#endif /* __TBB_TASK_ISOLATION */

namespace this_task_arena {

    inline int current_thread_index() {
        int idx = tbb::task_arena::current_thread_index();
        return idx == -1 ? tbb::task_arena::not_initialized : idx;
    }

    inline int max_concurrency() {
        return tbb::task_arena::internal_max_concurrency(NULL);
    }
} 

} 

#endif /* __TBB_task_arena_H */
