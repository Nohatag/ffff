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

#ifndef __TBB_task_scheduler_observer_H
#define __TBB_task_scheduler_observer_H

#include "atomic.h"
#if __TBB_ARENA_OBSERVER || __TBB_SLEEP_PERMISSION
#include "task_arena.h"
#endif

#if __TBB_SCHEDULER_OBSERVER

namespace tbb {
namespace interface6 {
class task_scheduler_observer;
}
namespace internal {

class observer_proxy;
class observer_list;

class task_scheduler_observer_v3 {
    friend class observer_proxy;
    friend class observer_list;
    friend class interface6::task_scheduler_observer;

    /** Observers are proxied by the scheduler to maintain persistent lists of them. **/
    observer_proxy* my_proxy;

    /** Valid only when observation is on. **/
    atomic<intptr_t> my_busy_count;

public:

    /** For local observers the method can be used only when the current thread
        has the task scheduler initialized or is attached to an arena.

        Repeated calls with the same state are no-ops. **/
    void __TBB_EXPORTED_METHOD observe( bool state=true );

    bool is_observing() const {return my_proxy!=NULL;}

    task_scheduler_observer_v3() : my_proxy(NULL) { my_busy_count.store<relaxed>(0); }

    /** Invoked from inside observe(true) call and whenever a worker enters the arena
        this observer is associated with. If a thread is already in the arena when
        the observer is activated, the entry notification is called before it
        executes the first stolen task.

        Obsolete semantics. For global observers it is called by a thread before
        the first steal since observation became enabled. **/
    virtual void on_scheduler_entry( bool /*is_worker*/ ) {}

    /** Invoked from inside observe(false) call and whenever a worker leaves the
        arena this observer is associated with.

        Obsolete semantics. For global observers it is called by a thread before
        the first steal since observation became enabled. **/
    virtual void on_scheduler_exit( bool /*is_worker*/ ) {}

    virtual ~task_scheduler_observer_v3() { if(my_proxy) observe(false);}
};

} 

#if __TBB_ARENA_OBSERVER || __TBB_SLEEP_PERMISSION
namespace interface6 {
class task_scheduler_observer : public internal::task_scheduler_observer_v3 {
    friend class internal::task_scheduler_observer_v3;
    friend class internal::observer_proxy;
    friend class internal::observer_list;

    /** Negative numbers with the largest absolute value to minimize probability
        of coincidence in case of a bug in busy count usage. **/

    static const intptr_t v6_trait = (intptr_t)((~(uintptr_t)0 >> 1) + 1);

    intptr_t my_context_tag;
    enum { global_tag = 0, implicit_tag = 1 };

public:

    /** For a local observer entry/exit notifications are invoked whenever a worker
        thread joins/leaves the arena of the observer's owner thread. If a thread is
        already in the arena when the observer is activated, the entry notification is
        called before it executes the first stolen task. **/
    /** TODO: Obsolete.
        Global observer semantics is obsolete as it violates master thread isolation
        guarantees and is not composable. Thus the current default behavior of the
        constructor is obsolete too and will be changed in one of the future versions
        of the library. **/
    explicit task_scheduler_observer( bool local = false ) {
#if  __TBB_ARENA_OBSERVER
        my_context_tag = local? implicit_tag : global_tag;
#else
        __TBB_ASSERT_EX( !local, NULL );
        my_context_tag = global_tag;
#endif
    }

#if  __TBB_ARENA_OBSERVER

    /** entry/exit notifications are invoked whenever a thread joins/leaves arena.
        If a thread is already in the arena when the observer is activated, the entry notification
        is called before it executes the first stolen task. **/
    explicit task_scheduler_observer( task_arena & a) {
        my_context_tag = (intptr_t)&a;
    }
#endif /* __TBB_ARENA_OBSERVER */

    /** Destructor protects instance of the observer from concurrent notification.
       It is recommended to disable observation before destructor of a derived class starts,
       otherwise it can lead to concurrent notification callback on partly destroyed object **/
    virtual ~task_scheduler_observer() { if(my_proxy) observe(false); }

    /** Warning: concurrent invocations of this method are not safe.
        Repeated calls with the same state are no-ops. **/
    void observe( bool state=true ) {
        if( state && !my_proxy ) {
            __TBB_ASSERT( !my_busy_count, "Inconsistent state of task_scheduler_observer instance");
            my_busy_count.store<relaxed>(v6_trait);
        }
        internal::task_scheduler_observer_v3::observe(state);
    }

#if  __TBB_SLEEP_PERMISSION

    enum { keep_awake = false, allow_sleep = true };

    /** If it returns false ('keep_awake'), the thread will keep spinning and looking for work.
        It will not be called for master threads. **/
    virtual bool may_sleep() { return allow_sleep; }
#endif /*__TBB_SLEEP_PERMISSION*/
};

} 
using interface6::task_scheduler_observer;
#else /*__TBB_ARENA_OBSERVER || __TBB_SLEEP_PERMISSION*/
typedef tbb::internal::task_scheduler_observer_v3 task_scheduler_observer;
#endif /*__TBB_ARENA_OBSERVER || __TBB_SLEEP_PERMISSION*/

} 

#endif /* __TBB_SCHEDULER_OBSERVER */

#endif /* __TBB_task_scheduler_observer_H */
