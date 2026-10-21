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

#ifndef __TBB_pipeline_H
#define __TBB_pipeline_H

#include "atomic.h"
#include "task.h"
#include "tbb_allocator.h"
#include <cstddef>

#if __TBB_CPP11_TYPE_PROPERTIES_PRESENT || __TBB_TR1_TYPE_PROPERTIES_IN_STD_PRESENT
#include <type_traits>
#endif

namespace tbb {

class pipeline;
class filter;

namespace internal {

#define __TBB_PIPELINE_VERSION(x) ((unsigned char)(x-2)<<1)

typedef unsigned long Token;
typedef long tokendiff_t;
class stage_task;
class input_buffer;
class pipeline_root_task;
class pipeline_cleaner;

} 

namespace interface6 {
    template<typename T, typename U> class filter_t;

    namespace internal {
        class pipeline_proxy;
    }
}

/** @ingroup algorithms */
class filter: internal::no_copy {
private:

    static filter* not_in_pipeline() {return reinterpret_cast<filter*>(intptr_t(-1));}
protected:

    static const unsigned char filter_is_serial = 0x1;

    /** The bit was not set for parallel filters in TBB 2.1 and earlier,
        but is_ordered() function always treats parallel filters as out of order. */
    static const unsigned char filter_is_out_of_order = 0x1<<4;

    static const unsigned char filter_is_bound = 0x1<<5;

    static const unsigned char filter_may_emit_null = 0x1<<6;

    static const unsigned char exact_exception_propagation =
#if TBB_USE_CAPTURED_EXCEPTION
            0x0;
#else
            0x1<<7;
#endif /* TBB_USE_CAPTURED_EXCEPTION */

    static const unsigned char current_version = __TBB_PIPELINE_VERSION(5);
    static const unsigned char version_mask = 0x7<<1; 
public:
    enum mode {

        parallel = current_version | filter_is_out_of_order,

        serial_in_order = current_version | filter_is_serial,

        serial_out_of_order = current_version | filter_is_serial | filter_is_out_of_order,

        serial = serial_in_order
    };
protected:
    explicit filter( bool is_serial_ ) :
        next_filter_in_pipeline(not_in_pipeline()),
        my_input_buffer(NULL),
        my_filter_mode(static_cast<unsigned char>((is_serial_ ? serial : parallel) | exact_exception_propagation)),
        prev_filter_in_pipeline(not_in_pipeline()),
        my_pipeline(NULL),
        next_segment(NULL)
    {}

    explicit filter( mode filter_mode ) :
        next_filter_in_pipeline(not_in_pipeline()),
        my_input_buffer(NULL),
        my_filter_mode(static_cast<unsigned char>(filter_mode | exact_exception_propagation)),
        prev_filter_in_pipeline(not_in_pipeline()),
        my_pipeline(NULL),
        next_segment(NULL)
    {}

    void __TBB_EXPORTED_METHOD set_end_of_input();

public:

    bool is_serial() const {
        return bool( my_filter_mode & filter_is_serial );
    }

    bool is_ordered() const {
        return (my_filter_mode & (filter_is_out_of_order|filter_is_serial))==filter_is_serial;
    }

    bool is_bound() const {
        return ( my_filter_mode & filter_is_bound )==filter_is_bound;
    }

    bool object_may_be_null() {
        return ( my_filter_mode & filter_may_emit_null ) == filter_may_emit_null;
    }

    /** Returns NULL if filter is a sink. */
    virtual void* operator()( void* item ) = 0;

    /** If the filter was added to a pipeline, the pipeline must be destroyed first. */
    virtual __TBB_EXPORTED_METHOD ~filter();

#if __TBB_TASK_GROUP_CONTEXT

    /** Required to prevent memory leaks.
        Note it can be called concurrently even for serial filters.*/
    virtual void finalize( void* /*item*/ ) {};
#endif

private:

    filter* next_filter_in_pipeline;

    bool has_more_work();

    /** The buffer is required if the filter is serial or follows a thread-bound one. */
    internal::input_buffer* my_input_buffer;

    friend class internal::stage_task;
    friend class internal::pipeline_root_task;
    friend class pipeline;
    friend class thread_bound_filter;

    const unsigned char my_filter_mode;

    filter* prev_filter_in_pipeline;

    pipeline* my_pipeline;

    /** In each segment, the first filter is not thread-bound but follows a thread-bound one. */
    filter* next_segment;
};

/** @ingroup algorithms */
class thread_bound_filter: public filter {
public:
    enum result_type {

        success,

        item_not_available,

        end_of_stream
    };
protected:
    explicit thread_bound_filter(mode filter_mode):
         filter(static_cast<mode>(filter_mode | filter::filter_is_bound))
    {
        __TBB_ASSERT(filter_mode & filter::filter_is_serial, "thread-bound filters must be serial");
    }
public:

    /** This interface is non-blocking.
        Returns 'success' if an item was processed.
        Returns 'item_not_available' if no item can be processed now
        but more may arrive in the future, or if token limit is reached.
        Returns 'end_of_stream' if there are no more items to process. */
    result_type __TBB_EXPORTED_METHOD try_process_item();

    /** This interface is blocking.
        Returns 'success' if an item was processed.
        Returns 'end_of_stream' if there are no more items to process.
        Never returns 'item_not_available', as it blocks until another return condition applies. */
    result_type __TBB_EXPORTED_METHOD process_item();

private:

    result_type internal_process_item(bool is_blocking);
};

/** @ingroup algorithms */
class pipeline {
public:

    __TBB_EXPORTED_METHOD pipeline();

    /** Though the current implementation declares the destructor virtual, do not rely on this
        detail.  The virtualness is deprecated and may disappear in future versions of TBB. */
    virtual __TBB_EXPORTED_METHOD ~pipeline();

    void __TBB_EXPORTED_METHOD add_filter( filter& filter_ );

    void __TBB_EXPORTED_METHOD run( size_t max_number_of_live_tokens );

#if __TBB_TASK_GROUP_CONTEXT

    void __TBB_EXPORTED_METHOD run( size_t max_number_of_live_tokens, tbb::task_group_context& context );
#endif

    void __TBB_EXPORTED_METHOD clear();

private:
    friend class internal::stage_task;
    friend class internal::pipeline_root_task;
    friend class filter;
    friend class thread_bound_filter;
    friend class internal::pipeline_cleaner;
    friend class tbb::interface6::internal::pipeline_proxy;

    filter* filter_list;

    filter* filter_end;

    task* end_counter;

    atomic<internal::Token> input_tokens;

    atomic<internal::Token> token_counter;

    bool end_of_input;

    bool has_thread_bound_filters;

    void remove_filter( filter& filter_ );

    void __TBB_EXPORTED_METHOD inject_token( task& self );

#if __TBB_TASK_GROUP_CONTEXT

    void clear_filters();
#endif
};

namespace interface6 {

namespace internal {
    template<typename T, typename U, typename Body> class concrete_filter;
}

class flow_control {
    bool is_pipeline_stopped;
    flow_control() { is_pipeline_stopped = false; }
    template<typename T, typename U, typename Body> friend class internal::concrete_filter;
public:
    void stop() { is_pipeline_stopped = true; }
};

namespace internal {

template<typename T> struct tbb_large_object {enum { value = sizeof(T) > sizeof(void *) }; };

#if   __TBB_CPP11_TYPE_PROPERTIES_PRESENT
template<typename T> struct tbb_trivially_copyable { enum { value = std::is_trivially_copyable<T>::value }; };
#elif __TBB_TR1_TYPE_PROPERTIES_IN_STD_PRESENT
template<typename T> struct tbb_trivially_copyable { enum { value = std::has_trivial_copy_constructor<T>::value }; };
#else

template<typename T> struct tbb_trivially_copyable { enum { value = false }; };
template<typename T> struct tbb_trivially_copyable <T*> { enum { value = true }; };
template<> struct tbb_trivially_copyable <short> { enum { value = true }; };
template<> struct tbb_trivially_copyable <unsigned short> { enum { value = true }; };
template<> struct tbb_trivially_copyable <int> { enum { value = !tbb_large_object<int>::value }; };
template<> struct tbb_trivially_copyable <unsigned int> { enum { value = !tbb_large_object<int>::value }; };
template<> struct tbb_trivially_copyable <long> { enum { value = !tbb_large_object<long>::value }; };
template<> struct tbb_trivially_copyable <unsigned long> { enum { value = !tbb_large_object<long>::value }; };
template<> struct tbb_trivially_copyable <float> { enum { value = !tbb_large_object<float>::value }; };
template<> struct tbb_trivially_copyable <double> { enum { value = !tbb_large_object<double>::value }; };
#endif 

template<typename T> struct is_large_object {enum { value = tbb_large_object<T>::value || !tbb_trivially_copyable<T>::value }; };

template<typename T, bool> class token_helper;

template<typename T>
class token_helper<T, true> {
    public:
    typedef typename tbb::tbb_allocator<T> allocator;
    typedef T* pointer;
    typedef T value_type;
    static pointer create_token(const value_type & source) {
        pointer output_t = allocator().allocate(1);
        return new (output_t) T(source);
    }
    static value_type & token(pointer & t) { return *t;}
    static void * cast_to_void_ptr(pointer ref) { return (void *) ref; }
    static pointer cast_from_void_ptr(void * ref) { return (pointer)ref; }
    static void destroy_token(pointer token) {
        allocator().destroy(token);
        allocator().deallocate(token,1);
    }
};

template<typename T>
class token_helper<T*, false > {
    public:
    typedef T* pointer;
    typedef T* value_type;
    static pointer create_token(const value_type & source) { return source; }
    static value_type & token(pointer & t) { return t;}
    static void * cast_to_void_ptr(pointer ref) { return (void *)ref; }
    static pointer cast_from_void_ptr(void * ref) { return (pointer)ref; }
    static void destroy_token( pointer /*token*/) {}
};

template<typename T>
class token_helper<T, false> {
    typedef union {
        T actual_value;
        void * void_overlay;
    } type_to_void_ptr_map;
    public:
    typedef T pointer;  
    typedef T value_type;
    static pointer create_token(const value_type & source) {
        return source; }
    static value_type & token(pointer & t) { return t;}
    static void * cast_to_void_ptr(pointer ref) {
        type_to_void_ptr_map mymap;
        mymap.void_overlay = NULL;
        mymap.actual_value = ref;
        return mymap.void_overlay;
    }
    static pointer cast_from_void_ptr(void * ref) {
        type_to_void_ptr_map mymap;
        mymap.void_overlay = ref;
        return mymap.actual_value;
    }
    static void destroy_token( pointer /*token*/) {}
};

template<typename T, typename U, typename Body>
class concrete_filter: public tbb::filter {
    const Body& my_body;
    typedef token_helper<T,is_large_object<T>::value > t_helper;
    typedef typename t_helper::pointer t_pointer;
    typedef token_helper<U,is_large_object<U>::value > u_helper;
    typedef typename u_helper::pointer u_pointer;

    void* operator()(void* input) __TBB_override {
        t_pointer temp_input = t_helper::cast_from_void_ptr(input);
        u_pointer output_u = u_helper::create_token(my_body(t_helper::token(temp_input)));
        t_helper::destroy_token(temp_input);
        return u_helper::cast_to_void_ptr(output_u);
    }

    void finalize(void * input) __TBB_override {
        t_pointer temp_input = t_helper::cast_from_void_ptr(input);
        t_helper::destroy_token(temp_input);
    }

public:
    concrete_filter(tbb::filter::mode filter_mode, const Body& body) : filter(filter_mode), my_body(body) {}
};

template<typename U, typename Body>
class concrete_filter<void,U,Body>: public filter {
    const Body& my_body;
    typedef token_helper<U, is_large_object<U>::value > u_helper;
    typedef typename u_helper::pointer u_pointer;

    void* operator()(void*) __TBB_override {
        flow_control control;
        u_pointer output_u = u_helper::create_token(my_body(control));
        if(control.is_pipeline_stopped) {
            u_helper::destroy_token(output_u);
            set_end_of_input();
            return NULL;
        }
        return u_helper::cast_to_void_ptr(output_u);
    }

public:
    concrete_filter(tbb::filter::mode filter_mode, const Body& body) :
        filter(static_cast<tbb::filter::mode>(filter_mode | filter_may_emit_null)),
        my_body(body)
    {}
};

template<typename T, typename Body>
class concrete_filter<T,void,Body>: public filter {
    const Body& my_body;
    typedef token_helper<T, is_large_object<T>::value > t_helper;
    typedef typename t_helper::pointer t_pointer;

    void* operator()(void* input) __TBB_override {
        t_pointer temp_input = t_helper::cast_from_void_ptr(input);
        my_body(t_helper::token(temp_input));
        t_helper::destroy_token(temp_input);
        return NULL;
    }
    void finalize(void* input) __TBB_override {
        t_pointer temp_input = t_helper::cast_from_void_ptr(input);
        t_helper::destroy_token(temp_input);
    }

public:
    concrete_filter(tbb::filter::mode filter_mode, const Body& body) : filter(filter_mode), my_body(body) {}
};

template<typename Body>
class concrete_filter<void,void,Body>: public filter {
    const Body& my_body;

    /** Override privately because it is always called virtually */
    void* operator()(void*) __TBB_override {
        flow_control control;
        my_body(control);
        void* output = control.is_pipeline_stopped ? NULL : (void*)(intptr_t)-1;
        return output;
    }
public:
    concrete_filter(filter::mode filter_mode, const Body& body) : filter(filter_mode), my_body(body) {}
};

/** It primarily serves as RAII class that deletes heap-allocated filter instances. */
class pipeline_proxy {
    tbb::pipeline my_pipe;
public:
    pipeline_proxy( const filter_t<void,void>& filter_chain );
    ~pipeline_proxy() {
        while( filter* f = my_pipe.filter_list )
            delete f; 
    }
    tbb::pipeline* operator->() { return &my_pipe; }
};

/** These nodes are always heap-allocated and can be shared by filter_t objects. */
class filter_node: tbb::internal::no_copy {
    /** Count must be atomic because it is hidden state for user, but might be shared by threads. */
    tbb::atomic<intptr_t> ref_count;
protected:
    filter_node() {
        ref_count = 0;
#ifdef __TBB_TEST_FILTER_NODE_COUNT
        ++(__TBB_TEST_FILTER_NODE_COUNT);
#endif
    }
public:

    virtual void add_to( pipeline& ) = 0;

    void add_ref() {++ref_count;}

    void remove_ref() {
        __TBB_ASSERT(ref_count>0,"ref_count underflow");
        if( --ref_count==0 )
            delete this;
    }
    virtual ~filter_node() {
#ifdef __TBB_TEST_FILTER_NODE_COUNT
        --(__TBB_TEST_FILTER_NODE_COUNT);
#endif
    }
};

template<typename T, typename U, typename Body>
class filter_node_leaf: public filter_node  {
    const tbb::filter::mode mode;
    const Body body;
    void add_to( pipeline& p ) __TBB_override {
        concrete_filter<T,U,Body>* f = new concrete_filter<T,U,Body>(mode,body);
        p.add_filter( *f );
    }
public:
    filter_node_leaf( tbb::filter::mode m, const Body& b ) : mode(m), body(b) {}
};

class filter_node_join: public filter_node {
    friend class filter_node; 
    filter_node& left;
    filter_node& right;
    ~filter_node_join() {
       left.remove_ref();
       right.remove_ref();
    }
    void add_to( pipeline& p ) __TBB_override {
        left.add_to(p);
        right.add_to(p);
    }
public:
    filter_node_join( filter_node& x, filter_node& y ) : left(x), right(y) {
       left.add_ref();
       right.add_ref();
    }
};

} 

template<typename T, typename U, typename Body>
filter_t<T,U> make_filter(tbb::filter::mode mode, const Body& body) {
    return new internal::filter_node_leaf<T,U,Body>(mode, body);
}

template<typename T, typename V, typename U>
filter_t<T,U> operator& (const filter_t<T,V>& left, const filter_t<V,U>& right) {
    __TBB_ASSERT(left.root,"cannot use default-constructed filter_t as left argument of '&'");
    __TBB_ASSERT(right.root,"cannot use default-constructed filter_t as right argument of '&'");
    return new internal::filter_node_join(*left.root,*right.root);
}

template<typename T, typename U>
class filter_t {
    typedef internal::filter_node filter_node;
    filter_node* root;
    filter_t( filter_node* root_ ) : root(root_) {
        root->add_ref();
    }
    friend class internal::pipeline_proxy;
    template<typename T_, typename U_, typename Body>
    friend filter_t<T_,U_> make_filter(tbb::filter::mode, const Body& );
    template<typename T_, typename V_, typename U_>
    friend filter_t<T_,U_> operator& (const filter_t<T_,V_>& , const filter_t<V_,U_>& );
public:

    filter_t() : root(NULL) {}
    filter_t( const filter_t<T,U>& rhs ) : root(rhs.root) {
        if( root ) root->add_ref();
    }
    template<typename Body>
    filter_t( tbb::filter::mode mode, const Body& body ) :
        root( new internal::filter_node_leaf<T,U,Body>(mode, body) ) {
        root->add_ref();
    }

    void operator=( const filter_t<T,U>& rhs ) {

        filter_node* old = root;
        root = rhs.root;
        if( root ) root->add_ref();
        if( old ) old->remove_ref();
    }
    ~filter_t() {
        if( root ) root->remove_ref();
    }
    void clear() {

        if( root ) {
            filter_node* old = root;
            root = NULL;
            old->remove_ref();
        }
    }
};

inline internal::pipeline_proxy::pipeline_proxy( const filter_t<void,void>& filter_chain ) : my_pipe() {
    __TBB_ASSERT( filter_chain.root, "cannot apply parallel_pipeline to default-constructed filter_t"  );
    filter_chain.root->add_to(my_pipe);
}

inline void parallel_pipeline(size_t max_number_of_live_tokens, const filter_t<void,void>& filter_chain
#if __TBB_TASK_GROUP_CONTEXT
    , tbb::task_group_context& context
#endif
    ) {
    internal::pipeline_proxy pipe(filter_chain);

    pipe->run(max_number_of_live_tokens
#if __TBB_TASK_GROUP_CONTEXT
              , context
#endif
    );
}

#if __TBB_TASK_GROUP_CONTEXT
inline void parallel_pipeline(size_t max_number_of_live_tokens, const filter_t<void,void>& filter_chain) {
    tbb::task_group_context context;
    parallel_pipeline(max_number_of_live_tokens, filter_chain, context);
}
#endif 

} 

using interface6::flow_control;
using interface6::filter_t;
using interface6::make_filter;
using interface6::parallel_pipeline;

} 

#endif /* __TBB_pipeline_H */
