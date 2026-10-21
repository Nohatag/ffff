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

#ifndef __TBB_task_scheduler_init_H
#define __TBB_task_scheduler_init_H

#include "tbb_stddef.h"
#include "limits.h"
#if __TBB_SUPPORTS_WORKERS_WAITING_IN_TERMINATE
#include <new> 
#endif

namespace tbb {

typedef std::size_t stack_size_type;

namespace internal {

    /** @ingroup task_scheduling */
    class scheduler;
} 

/** A thread can construct a task_scheduler_init object and keep it alive
    while it uses TBB's tasking subsystem (including parallel algorithms).

    This class allows to customize properties of the TBB task pool to some extent.
    For example it can limit concurrency level of parallel work initiated by the
    given thread. It also can be used to specify stack size of the TBB worker threads,
    though this setting is not effective if the thread pool has already been created.

    If a parallel construct is used without task_scheduler_init object previously
    created, the scheduler will be initialized automatically with default settings,
    and will persist until this thread exits. Default concurrency level is defined
    as described in task_scheduler_init::initialize().
    @ingroup task_scheduling */
class task_scheduler_init: internal::no_copy {
    enum ExceptionPropagationMode {
        propagation_mode_exact = 1u,
        propagation_mode_captured = 2u,
        propagation_mode_mask = propagation_mode_exact | propagation_mode_captured
    };

    /** NULL if not currently initialized. */
    internal::scheduler* my_scheduler;

    bool internal_terminate( bool blocking );
#if __TBB_SUPPORTS_WORKERS_WAITING_IN_TERMINATE
    bool __TBB_EXPORTED_METHOD internal_blocking_terminate( bool throwing );
#endif
public:

    static const int automatic = -1;

    static const int deferred = -2;

    /** A value of -1 lets TBB decide on the number of threads, which is usually
        maximal hardware concurrency for this process, that is the number of logical
        CPUs on the machine (possibly limited by the processor affinity mask of this
        process (Windows) or of this thread (Linux, FreeBSD). It is preferable option
        for production code because it helps to avoid nasty surprises when several
        TBB based components run side-by-side or in a nested fashion inside the same
        process.

        The number_of_threads is ignored if any other task_scheduler_inits
        currently exist.  A thread may construct multiple task_scheduler_inits.
        Doing so does no harm because the underlying scheduler is reference counted. */
    void __TBB_EXPORTED_METHOD initialize( int number_of_threads=automatic );

    /** Overloading is necessary to preserve ABI compatibility */
    void __TBB_EXPORTED_METHOD initialize( int number_of_threads, stack_size_type thread_stack_size );

    void __TBB_EXPORTED_METHOD terminate();

#if __TBB_SUPPORTS_WORKERS_WAITING_IN_TERMINATE
#if TBB_USE_EXCEPTIONS

    void blocking_terminate() {
        internal_blocking_terminate( /*throwing=*/true );
    }
#endif

    bool blocking_terminate(const std::nothrow_t&) __TBB_NOEXCEPT(true) {
        return internal_blocking_terminate( /*throwing=*/false );
    }
#endif 

    task_scheduler_init( int number_of_threads=automatic, stack_size_type thread_stack_size=0 ) : my_scheduler(NULL)
    {

        __TBB_ASSERT( !(thread_stack_size & propagation_mode_mask), "Requested stack size is not aligned" );
#if TBB_USE_EXCEPTIONS
        thread_stack_size |= TBB_USE_CAPTURED_EXCEPTION ? propagation_mode_captured : propagation_mode_exact;
#endif /* TBB_USE_EXCEPTIONS */
        initialize( number_of_threads, thread_stack_size );
    }

    ~task_scheduler_init() {
        if( my_scheduler )
            terminate();
        internal::poison_pointer( my_scheduler );
    }

    /** Result returned by this method does not depend on whether the scheduler
        has already been initialized.

        Because tbb 2.0 does not support blocking tasks yet, you may use this method
        to boost the number of threads in the tbb's internal pool, if your tasks are
        doing I/O operations. The optimal number of additional threads depends on how
        much time your tasks spend in the blocked state.

        Before TBB 3.0 U4 this method returned the number of logical CPU in the
        system. Currently on Windows, Linux and FreeBSD it returns the number of
        logical CPUs available to the current process in accordance with its affinity
        mask.

        NOTE: The return value of this method never changes after its first invocation.
        This means that changes in the process affinity mask that took place after
        this method was first invoked will not affect the number of worker threads
        in the TBB worker threads pool. */
    static int __TBB_EXPORTED_FUNC default_num_threads ();

    bool is_active() const { return my_scheduler != NULL; }
};

} 

#endif /* __TBB_task_scheduler_init_H */
