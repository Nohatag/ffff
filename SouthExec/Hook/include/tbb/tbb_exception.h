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

#ifndef __TBB_exception_H
#define __TBB_exception_H

#include "tbb_stddef.h"
#include <exception>
#include <new>    
#include <string> 

namespace tbb {

class bad_last_alloc : public std::bad_alloc {
public:
    const char* what() const throw() __TBB_override;
#if __TBB_DEFAULT_DTOR_THROW_SPEC_BROKEN
    ~bad_last_alloc() throw() __TBB_override {}
#endif
};

class improper_lock : public std::exception {
public:
    const char* what() const throw() __TBB_override;
};

class user_abort : public std::exception {
public:
    const char* what() const throw() __TBB_override;
};

class missing_wait : public std::exception {
public:
    const char* what() const throw() __TBB_override;
};

class invalid_multiple_scheduling : public std::exception {
public:
    const char* what() const throw() __TBB_override;
};

namespace internal {

void __TBB_EXPORTED_FUNC throw_bad_last_alloc_exception_v4();

enum exception_id {
    eid_bad_alloc = 1,
    eid_bad_last_alloc,
    eid_nonpositive_step,
    eid_out_of_range,
    eid_segment_range_error,
    eid_index_range_error,
    eid_missing_wait,
    eid_invalid_multiple_scheduling,
    eid_improper_lock,
    eid_possible_deadlock,
    eid_operation_not_permitted,
    eid_condvar_wait_failed,
    eid_invalid_load_factor,
    eid_reserved, 
    eid_invalid_swap,
    eid_reservation_length_error,
    eid_invalid_key,
    eid_user_abort,
    eid_reserved1,
#if __TBB_SUPPORTS_WORKERS_WAITING_IN_TERMINATE

    eid_blocking_thread_join_impossible = eid_reserved1,
#endif
    eid_bad_tagged_msg_cast,

    /** When adding new IDs, place them immediately _before_ this comment (that is
        _after_ all the existing IDs. NEVER insert new IDs between the existing ones. **/
    eid_max
};

/** Its purpose is to minimize code bloat that can be caused by throw operators
    scattered in multiple places, especially in templates. **/
void __TBB_EXPORTED_FUNC throw_exception_v4 ( exception_id );

inline void throw_exception ( exception_id eid ) { throw_exception_v4(eid); }

} 
} 

#if __TBB_TASK_GROUP_CONTEXT
#include "tbb_allocator.h"
#include <typeinfo> 

namespace tbb {

/** If an unhandled exception of the type derived from tbb::tbb_exception is intercepted
    by the TBB scheduler in one of the worker threads, it is delivered to and re-thrown in
    the root thread. The root thread is the thread that has started the outermost algorithm
    or root task sharing the same task_group_context with the guilty algorithm/task (the one
    that threw the exception first).

    Note: when documentation mentions workers with respect to exception handling,
    masters are implied as well, because they are completely equivalent in this context.
    Consequently a root thread can be master or worker thread.

    NOTE: In case of nested algorithms or complex task hierarchies when the nested
    levels share (explicitly or by means of implicit inheritance) the task group
    context of the outermost level, the exception may be (re-)thrown multiple times
    (ultimately - in each worker on each nesting level) before reaching the root
    thread at the outermost level. IMPORTANT: if you intercept an exception derived
    from this class on a nested level, you must re-throw it in the catch block by means
    of the "throw;" operator.

    TBB provides two implementations of this interface: tbb::captured_exception and
    template class tbb::movable_exception. See their declarations for more info. **/
class tbb_exception : public std::exception
{
    /** No operator new is provided because the TBB usage model assumes dynamic
        creation of the TBB exception objects only by means of applying move()
        operation on an exception thrown out of TBB scheduler. **/
    void* operator new ( size_t );

public:
#if __clang__

    ~tbb_exception() throw() { /* keep the semicolon! */ ; }
#endif

    /** Move semantics is allowed. **/
    virtual tbb_exception* move() throw() = 0;

    /** Frees memory and calls destructor for this exception object.
        Can and must be used only on objects created by the move method. **/
    virtual void destroy() throw() = 0;

    /** Make sure that if you have several levels of derivation from this interface
        you implement or override this method on the most derived level. The implementation
        is as simple as "throw *this;". Failure to do this will result in exception
        of a base class type being thrown. **/
    virtual void throw_self() = 0;

    virtual const char* name() const throw() = 0;

    virtual const char* what() const throw() __TBB_override = 0;

    /** Operator delete is provided only to allow using existing smart pointers
        with TBB exception objects obtained as the result of applying move()
        operation on an exception thrown out of TBB scheduler.

        When overriding method move() make sure to override operator delete as well
        if memory is allocated not by TBB's scalable allocator. **/
    void operator delete ( void* p ) {
        internal::deallocate_via_handler_v3(p);
    }
};

/** Exception of this type is thrown by TBB in the root thread (thread that started a parallel
    algorithm ) if an unhandled exception was intercepted during the algorithm execution in one
    of the workers.
    \sa tbb::tbb_exception **/
class captured_exception : public tbb_exception
{
public:
    captured_exception( const captured_exception& src )
        : tbb_exception(src), my_dynamic(false)
    {
        set(src.my_exception_name, src.my_exception_info);
    }

    captured_exception( const char* name_, const char* info )
        : my_dynamic(false)
    {
        set(name_, info);
    }

    __TBB_EXPORTED_METHOD ~captured_exception() throw();

    captured_exception& operator= ( const captured_exception& src ) {
        if ( this != &src ) {
            clear();
            set(src.my_exception_name, src.my_exception_info);
        }
        return *this;
    }

    captured_exception* __TBB_EXPORTED_METHOD move() throw() __TBB_override;

    void __TBB_EXPORTED_METHOD destroy() throw() __TBB_override;

    void throw_self() __TBB_override { __TBB_THROW(*this); }

    const char* __TBB_EXPORTED_METHOD name() const throw() __TBB_override;

    const char* __TBB_EXPORTED_METHOD what() const throw() __TBB_override;

    void __TBB_EXPORTED_METHOD set( const char* name, const char* info ) throw();
    void __TBB_EXPORTED_METHOD clear() throw();

private:

    captured_exception() {}

    static captured_exception* allocate( const char* name, const char* info );

    bool my_dynamic;
    const char* my_exception_name;
    const char* my_exception_info;
};

/** Code using TBB can instantiate this template with an arbitrary ExceptionData type
    and throw this exception object. Such exceptions are intercepted by the TBB scheduler
    and delivered to the root thread ().
    \sa tbb::tbb_exception **/
template<typename ExceptionData>
class movable_exception : public tbb_exception
{
    typedef movable_exception<ExceptionData> self_type;

public:
    movable_exception( const ExceptionData& data_ )
        : my_exception_data(data_)
        , my_dynamic(false)
        , my_exception_name(
#if TBB_USE_EXCEPTIONS
        typeid(self_type).name()
#else /* !TBB_USE_EXCEPTIONS */
        "movable_exception"
#endif /* !TBB_USE_EXCEPTIONS */
        )
    {}

    movable_exception( const movable_exception& src ) throw ()
        : tbb_exception(src)
        , my_exception_data(src.my_exception_data)
        , my_dynamic(false)
        , my_exception_name(src.my_exception_name)
    {}

    ~movable_exception() throw() {}

    const movable_exception& operator= ( const movable_exception& src ) {
        if ( this != &src ) {
            my_exception_data = src.my_exception_data;
            my_exception_name = src.my_exception_name;
        }
        return *this;
    }

    ExceptionData& data() throw() { return my_exception_data; }

    const ExceptionData& data() const throw() { return my_exception_data; }

    const char* name() const throw() __TBB_override { return my_exception_name; }

    const char* what() const throw() __TBB_override { return "tbb::movable_exception"; }

    movable_exception* move() throw() __TBB_override {
        void* e = internal::allocate_via_handler_v3(sizeof(movable_exception));
        if ( e ) {
            ::new (e) movable_exception(*this);
            ((movable_exception*)e)->my_dynamic = true;
        }
        return (movable_exception*)e;
    }
    void destroy() throw() __TBB_override {
        __TBB_ASSERT ( my_dynamic, "Method destroy can be called only on dynamically allocated movable_exceptions" );
        if ( my_dynamic ) {
            this->~movable_exception();
            internal::deallocate_via_handler_v3(this);
        }
    }
    void throw_self() __TBB_override { __TBB_THROW( *this ); }

protected:

    ExceptionData  my_exception_data;

private:

    bool my_dynamic;

    /** We rely on the fact that RTTI names are static string constants. **/
    const char* my_exception_name;
};

#if !TBB_USE_CAPTURED_EXCEPTION
namespace internal {

/** This class can be used only when the appropriate runtime support (mandated
    by C++11) is present **/
class tbb_exception_ptr {
    std::exception_ptr  my_ptr;

public:
    static tbb_exception_ptr* allocate();
    static tbb_exception_ptr* allocate( const tbb_exception& tag );

    static tbb_exception_ptr* allocate( captured_exception& src );

    /** Note that objects of this type can be created only by the allocate() method. **/
    void destroy() throw();

    void throw_self() { std::rethrow_exception(my_ptr); }

private:
    tbb_exception_ptr( const std::exception_ptr& src ) : my_ptr(src) {}
    tbb_exception_ptr( const captured_exception& src ) :
        #if __TBB_MAKE_EXCEPTION_PTR_PRESENT
            my_ptr(std::make_exception_ptr(src))  
        #else
            my_ptr(std::copy_exception(src))      
        #endif
    {}
}; 

} 
#endif /* !TBB_USE_CAPTURED_EXCEPTION */

} 

#endif /* __TBB_TASK_GROUP_CONTEXT */

#endif /* __TBB_exception_H */
