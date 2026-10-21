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

#ifndef __TBB__x86_eliding_mutex_impl_H
#define __TBB__x86_eliding_mutex_impl_H

#ifndef __TBB_spin_mutex_H
#error Do not #include this internal file directly; use public TBB headers instead.
#endif

#if ( __TBB_x86_32 || __TBB_x86_64 )

namespace tbb {
namespace interface7 {
namespace internal {

template<typename Mutex, bool is_rw>
class padded_mutex;

/** A x86_eliding_mutex is an HLE-enabled spin mutex. It is recommended to
    put the mutex on a cache line that is not shared by the data it protects.
    It should be used for locking short critical sections where the lock is
    contended but the data it protects are not.  If zero-initialized, the
    mutex is considered unheld.
    @ingroup synchronization */
class x86_eliding_mutex : tbb::internal::mutex_copy_deprecated_and_disabled {

    __TBB_atomic_flag flag;

    friend class padded_mutex<x86_eliding_mutex, false>;

public:

    /** Equivalent to zero-initialization of *this. */
    x86_eliding_mutex() : flag(0) {}

#if __TBB_USE_X86_ELIDING_MUTEX || __TBB_GCC_VERSION < 40000
#else

private:
#endif

    class scoped_lock : tbb::internal::no_copy {
    private:

        x86_eliding_mutex* my_mutex;

    public:

        scoped_lock() : my_mutex(NULL) {}

        scoped_lock( x86_eliding_mutex& m ) : my_mutex(NULL) { acquire(m); }

        void acquire( x86_eliding_mutex& m ) {
            __TBB_ASSERT( !my_mutex, "already holding a lock" );

            my_mutex=&m;
            my_mutex->lock();
        }

        /** Return true if lock acquired; false otherwise. */
        bool try_acquire( x86_eliding_mutex& m ) {
            __TBB_ASSERT( !my_mutex, "already holding a lock" );

            bool result = m.try_lock();
            if( result ) {
                my_mutex = &m;
            }
            return result;
        }

        void release() {
            __TBB_ASSERT( my_mutex, "release on scoped_lock that is not holding a lock" );

            my_mutex->unlock();
            my_mutex = NULL;
        }

        ~scoped_lock() {
            if( my_mutex ) {
                release();
            }
        }
    };
#if __TBB_USE_X86_ELIDING_MUTEX || __TBB_GCC_VERSION < 40000
#else
public:
#endif  /* __TBB_USE_X86_ELIDING_MUTEX */

    static const bool is_rw_mutex = false;
    static const bool is_recursive_mutex = false;
    static const bool is_fair_mutex = false;

    void lock() {
        __TBB_LockByteElided(flag);
    }

    /** Return true if lock acquired; false otherwise. */
    bool try_lock() {
        return __TBB_TryLockByteElided(flag);
    }

    void unlock() {
        __TBB_UnlockByteElided( flag );
    }
}; 

} 
} 
} 

#endif /* ( __TBB_x86_32 || __TBB_x86_64 ) */

#endif /* __TBB__x86_eliding_mutex_impl_H */
