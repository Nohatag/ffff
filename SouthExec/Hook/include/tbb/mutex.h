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

#ifndef __TBB_mutex_H
#define __TBB_mutex_H

#if _WIN32||_WIN64
#include "machine/windows_api.h"
#else
#include <pthread.h>
#endif /* _WIN32||_WIN64 */

#include <new>
#include "aligned_space.h"
#include "tbb_stddef.h"
#include "tbb_profiling.h"

namespace tbb {

/** @ingroup synchronization */
class mutex : internal::mutex_copy_deprecated_and_disabled {
public:

    mutex() {
#if TBB_USE_ASSERT || TBB_USE_THREADING_TOOLS
    internal_construct();
#else
  #if _WIN32||_WIN64
        InitializeCriticalSectionEx(&impl, 4000, 0);
  #else
        int error_code = pthread_mutex_init(&impl,NULL);
        if( error_code )
            tbb::internal::handle_perror(error_code,"mutex: pthread_mutex_init failed");
  #endif /* _WIN32||_WIN64*/
#endif /* TBB_USE_ASSERT */
    };

    ~mutex() {
#if TBB_USE_ASSERT
        internal_destroy();
#else
  #if _WIN32||_WIN64
        DeleteCriticalSection(&impl);
  #else
        pthread_mutex_destroy(&impl);

  #endif /* _WIN32||_WIN64 */
#endif /* TBB_USE_ASSERT */
    };

    class scoped_lock;
    friend class scoped_lock;

    /** It helps to avoid the common problem of forgetting to release lock.
        It also nicely provides the "node" for queuing locks. */
    class scoped_lock : internal::no_copy {
    public:

        scoped_lock() : my_mutex(NULL) {};

        scoped_lock( mutex& mutex ) {
            acquire( mutex );
        }

        ~scoped_lock() {
            if( my_mutex )
                release();
        }

        void acquire( mutex& mutex ) {
#if TBB_USE_ASSERT
            internal_acquire(mutex);
#else
            mutex.lock();
            my_mutex = &mutex;
#endif /* TBB_USE_ASSERT */
        }

        bool try_acquire( mutex& mutex ) {
#if TBB_USE_ASSERT
            return internal_try_acquire (mutex);
#else
            bool result = mutex.try_lock();
            if( result )
                my_mutex = &mutex;
            return result;
#endif /* TBB_USE_ASSERT */
        }

        void release() {
#if TBB_USE_ASSERT
            internal_release ();
#else
            my_mutex->unlock();
            my_mutex = NULL;
#endif /* TBB_USE_ASSERT */
        }

    private:

        mutex* my_mutex;

        void __TBB_EXPORTED_METHOD internal_acquire( mutex& m );

        bool __TBB_EXPORTED_METHOD internal_try_acquire( mutex& m );

        void __TBB_EXPORTED_METHOD internal_release();

        friend class mutex;
    };

    static const bool is_rw_mutex = false;
    static const bool is_recursive_mutex = false;
    static const bool is_fair_mutex = false;

    void lock() {
#if TBB_USE_ASSERT
        aligned_space<scoped_lock> tmp;
        new(tmp.begin()) scoped_lock(*this);
#else
  #if _WIN32||_WIN64
        EnterCriticalSection(&impl);
  #else
        int error_code = pthread_mutex_lock(&impl);
        if( error_code )
            tbb::internal::handle_perror(error_code,"mutex: pthread_mutex_lock failed");
  #endif /* _WIN32||_WIN64 */
#endif /* TBB_USE_ASSERT */
    }

    /** Return true if lock acquired; false otherwise. */
    bool try_lock() {
#if TBB_USE_ASSERT
        aligned_space<scoped_lock> tmp;
        scoped_lock& s = *tmp.begin();
        s.my_mutex = NULL;
        return s.internal_try_acquire(*this);
#else
  #if _WIN32||_WIN64
        return TryEnterCriticalSection(&impl)!=0;
  #else
        return pthread_mutex_trylock(&impl)==0;
  #endif /* _WIN32||_WIN64 */
#endif /* TBB_USE_ASSERT */
    }

    void unlock() {
#if TBB_USE_ASSERT
        aligned_space<scoped_lock> tmp;
        scoped_lock& s = *tmp.begin();
        s.my_mutex = this;
        s.internal_release();
#else
  #if _WIN32||_WIN64
        LeaveCriticalSection(&impl);
  #else
        pthread_mutex_unlock(&impl);
  #endif /* _WIN32||_WIN64 */
#endif /* TBB_USE_ASSERT */
    }

  #if _WIN32||_WIN64
    typedef LPCRITICAL_SECTION native_handle_type;
  #else
    typedef pthread_mutex_t* native_handle_type;
  #endif
    native_handle_type native_handle() { return (native_handle_type) &impl; }

    enum state_t {
        INITIALIZED=0x1234,
        DESTROYED=0x789A,
        HELD=0x56CD
    };
private:
#if _WIN32||_WIN64
    CRITICAL_SECTION impl;
    enum state_t state;
#else
    pthread_mutex_t impl;
#endif /* _WIN32||_WIN64 */

    void __TBB_EXPORTED_METHOD internal_construct();

    void __TBB_EXPORTED_METHOD internal_destroy();

#if _WIN32||_WIN64
public:

    void set_state( state_t to ) { state = to; }
#endif
};

__TBB_DEFINE_PROFILING_SET_NAME(mutex)

} 

#endif /* __TBB_mutex_H */
