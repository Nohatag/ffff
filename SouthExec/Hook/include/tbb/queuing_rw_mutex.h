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

#ifndef __TBB_queuing_rw_mutex_H
#define __TBB_queuing_rw_mutex_H

#include <cstring>
#include "atomic.h"
#include "tbb_profiling.h"

namespace tbb {

/** Adapted from Krieger, Stumm, et al. pseudocode at
    http://www.eecg.toronto.edu/parallel/pubs_abs.html#Krieger_etal_ICPP93
    @ingroup synchronization */
class queuing_rw_mutex : internal::mutex_copy_deprecated_and_disabled {
public:

    queuing_rw_mutex() {
        q_tail = NULL;
#if TBB_USE_THREADING_TOOLS
        internal_construct();
#endif
    }

    ~queuing_rw_mutex() {
#if TBB_USE_ASSERT
        __TBB_ASSERT( !q_tail, "destruction of an acquired mutex");
#endif
    }

    /** It helps to avoid the common problem of forgetting to release lock.
        It also nicely provides the "node" for queuing locks. */
    class scoped_lock: internal::no_copy {

        void initialize() {
            my_mutex = NULL;
            my_internal_lock = 0;
            my_going = 0;
#if TBB_USE_ASSERT
            my_state = 0xFF; 
            internal::poison_pointer(my_next);
            internal::poison_pointer(my_prev);
#endif /* TBB_USE_ASSERT */
        }

    public:

        /** Equivalent to zero-initialization of *this. */
        scoped_lock() {initialize();}

        scoped_lock( queuing_rw_mutex& m, bool write=true ) {
            initialize();
            acquire(m,write);
        }

        ~scoped_lock() {
            if( my_mutex ) release();
        }

        void acquire( queuing_rw_mutex& m, bool write=true );

        bool try_acquire( queuing_rw_mutex& m, bool write=true );

        void release();

        /** Returns whether the upgrade happened without releasing and re-acquiring the lock */
        bool upgrade_to_writer();

        bool downgrade_to_reader();

    private:

        queuing_rw_mutex* my_mutex;

        scoped_lock *__TBB_atomic my_prev, *__TBB_atomic my_next;

        typedef unsigned char state_t;

        atomic<state_t> my_state;

        /** Corresponds to "spin" in the pseudocode but inverted for the sake of zero-initialization */
        unsigned char __TBB_atomic my_going;

        unsigned char my_internal_lock;

        void acquire_internal_lock();

        /** Returns true if lock was successfully acquired. */
        bool try_acquire_internal_lock();

        void release_internal_lock();

        void wait_for_release_of_internal_lock();

        void unblock_or_wait_on_internal_lock( uintptr_t );
    };

    void __TBB_EXPORTED_METHOD internal_construct();

    static const bool is_rw_mutex = true;
    static const bool is_recursive_mutex = false;
    static const bool is_fair_mutex = true;

private:

    atomic<scoped_lock*> q_tail;

};

__TBB_DEFINE_PROFILING_SET_NAME(queuing_rw_mutex)

} 

#endif /* __TBB_queuing_rw_mutex_H */
