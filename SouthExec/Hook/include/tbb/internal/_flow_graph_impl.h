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

#ifndef __TBB_flow_graph_impl_H
#define __TBB_flow_graph_impl_H

#include "../tbb_stddef.h"
#include "../task.h"
#include "../task_arena.h"
#include "../flow_graph_abstractions.h"

#if __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES
#include "../concurrent_priority_queue.h"
#endif

#include <list>

#if TBB_DEPRECATED_FLOW_ENQUEUE
#define FLOW_SPAWN(a) tbb::task::enqueue((a))
#else
#define FLOW_SPAWN(a) tbb::task::spawn((a))
#endif

#if __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES
#define __TBB_FLOW_GRAPH_PRIORITY_EXPR( expr ) expr
#define __TBB_FLOW_GRAPH_PRIORITY_ARG0( priority ) , priority
#define __TBB_FLOW_GRAPH_PRIORITY_ARG1( arg1, priority ) arg1, priority
#else
#define __TBB_FLOW_GRAPH_PRIORITY_EXPR( expr )
#define __TBB_FLOW_GRAPH_PRIORITY_ARG0( priority )
#define __TBB_FLOW_GRAPH_PRIORITY_ARG1( arg1, priority ) arg1
#endif 

namespace tbb {
namespace flow {

namespace internal {
static tbb::task * const SUCCESSFULLY_ENQUEUED = (task *)-1;
#if __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES
typedef unsigned int node_priority_t;
static const node_priority_t no_priority = node_priority_t(0);
#endif
}

namespace interface10 {

using tbb::flow::internal::SUCCESSFULLY_ENQUEUED;

#if __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES
using tbb::flow::internal::node_priority_t;
using tbb::flow::internal::no_priority;

struct graph_task : public task {
    graph_task( node_priority_t node_priority = no_priority ) : priority( node_priority ) {}
    node_priority_t priority;
};
#else
typedef task graph_task;
#endif /* __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES */

class graph;
class graph_node;

template <typename GraphContainerType, typename GraphNodeType>
class graph_iterator {
    friend class graph;
    friend class graph_node;
public:
    typedef size_t size_type;
    typedef GraphNodeType value_type;
    typedef GraphNodeType* pointer;
    typedef GraphNodeType& reference;
    typedef const GraphNodeType& const_reference;
    typedef std::forward_iterator_tag iterator_category;

    graph_iterator() : my_graph(NULL), current_node(NULL) {}

    graph_iterator(const graph_iterator& other) :
        my_graph(other.my_graph), current_node(other.current_node)
    {}

    graph_iterator& operator=(const graph_iterator& other) {
        if (this != &other) {
            my_graph = other.my_graph;
            current_node = other.current_node;
        }
        return *this;
    }

    reference operator*() const;

    pointer operator->() const;

    bool operator==(const graph_iterator& other) const {
        return ((my_graph == other.my_graph) && (current_node == other.current_node));
    }

    bool operator!=(const graph_iterator& other) const { return !(operator==(other)); }

    graph_iterator& operator++() {
        internal_forward();
        return *this;
    }

    graph_iterator operator++(int) {
        graph_iterator result = *this;
        operator++();
        return result;
    }

private:

    GraphContainerType *my_graph;

    pointer current_node;

    graph_iterator(GraphContainerType *g, bool begin);
    void internal_forward();
};  

enum reset_flags {
    rf_reset_protocol = 0,
    rf_reset_bodies = 1 << 0,  
    rf_clear_edges = 1 << 1   
};

namespace internal {

void activate_graph(graph& g);
void deactivate_graph(graph& g);
bool is_graph_active(graph& g);
void spawn_in_graph_arena(graph& g, tbb::task& arena_task);
void add_task_to_graph_reset_list(graph& g, tbb::task *tp);
template<typename F> void execute_in_graph_arena(graph& g, F& f);

#if __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES
struct graph_task_comparator {
    bool operator()(const graph_task* left, const graph_task* right) {
        return left->priority < right->priority;
    }
};

typedef tbb::concurrent_priority_queue<graph_task*, graph_task_comparator> graph_task_priority_queue_t;

class priority_task_selector : public task {
public:
    priority_task_selector(graph_task_priority_queue_t& priority_queue)
        : my_priority_queue(priority_queue) {}
    task* execute() __TBB_override {
        graph_task* t = NULL;
        bool result = my_priority_queue.try_pop(t);
        __TBB_ASSERT_EX( result, "Number of critical tasks for scheduler and tasks"
                         " in graph's priority queue mismatched" );
        __TBB_ASSERT( t && t != SUCCESSFULLY_ENQUEUED,
                      "Incorrect task submitted to graph priority queue" );
        __TBB_ASSERT( t->priority != tbb::flow::internal::no_priority,
                      "Tasks from graph's priority queue must have priority" );
        task* t_next = t->execute();
        task::destroy(*t);
        return t_next;
    }
private:
    graph_task_priority_queue_t& my_priority_queue;
};
#endif /* __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES */

}

/** This class serves as a handle to the graph */
class graph : tbb::internal::no_copy, public tbb::flow::graph_proxy {
    friend class graph_node;

    template< typename Body >
    class run_task : public graph_task {
    public:
        run_task(Body& body
#if __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES
                 , node_priority_t node_priority = no_priority
        ) : graph_task(node_priority),
#else
        ) :
#endif
        my_body(body) { }
        tbb::task *execute() __TBB_override {
            my_body();
            return NULL;
        }
    private:
        Body my_body;
    };

    template< typename Receiver, typename Body >
    class run_and_put_task : public graph_task {
    public:
        run_and_put_task(Receiver &r, Body& body) : my_receiver(r), my_body(body) {}
        tbb::task *execute() __TBB_override {
            tbb::task *res = my_receiver.try_put_task(my_body());
            if (res == SUCCESSFULLY_ENQUEUED) res = NULL;
            return res;
        }
    private:
        Receiver &my_receiver;
        Body my_body;
    };
    typedef std::list<tbb::task *> task_list_type;

    class wait_functor {
        tbb::task* graph_root_task;
    public:
        wait_functor(tbb::task* t) : graph_root_task(t) {}
        void operator()() const { graph_root_task->wait_for_all(); }
    };

    class spawn_functor : tbb::internal::no_assign {
        tbb::task& spawn_task;
    public:
        spawn_functor(tbb::task& t) : spawn_task(t) {}
        void operator()() const {
            FLOW_SPAWN(spawn_task);
        }
    };

    void prepare_task_arena(bool reinit = false) {
        if (reinit) {
            __TBB_ASSERT(my_task_arena, "task arena is NULL");
            my_task_arena->terminate();
            my_task_arena->initialize(tbb::task_arena::attach());
        }
        else {
            __TBB_ASSERT(my_task_arena == NULL, "task arena is not NULL");
            my_task_arena = new tbb::task_arena(tbb::task_arena::attach());
        }
        if (!my_task_arena->is_active()) 
            my_task_arena->initialize(); 
        __TBB_ASSERT(my_task_arena->is_active(), "task arena is not active");
    }

public:

    graph();

    explicit graph(tbb::task_group_context& use_this_context);

    /** Calls wait_for_all, then destroys the root task and context. */
    ~graph();

#if TBB_PREVIEW_FLOW_GRAPH_TRACE
    void set_name(const char *name);
#endif

    void increment_wait_count() {
        reserve_wait();
    }

    void decrement_wait_count() {
        release_wait();
    }

    /** The graph will not return from wait_for_all until a matching number of decrement_wait_count calls
    is made. */
    void reserve_wait() __TBB_override;

    /** The graph will not return from wait_for_all until all the number of decrement_wait_count calls
    matches the number of increment_wait_count calls. */
    void release_wait() __TBB_override;

    /** The task is spawned as a child of the graph. This is useful for running tasks
    that need to block a wait_for_all() on the graph.  For example a one-off source. */
    template< typename Receiver, typename Body >
    void run(Receiver &r, Body body) {
        if (internal::is_graph_active(*this)) {
            task* rtask = new (task::allocate_additional_child_of(*root_task()))
                run_and_put_task< Receiver, Body >(r, body);
            my_task_arena->execute(spawn_functor(*rtask));
        }
    }

    /** The task is spawned as a child of the graph. This is useful for running tasks
    that need to block a wait_for_all() on the graph. For example a one-off source. */
    template< typename Body >
    void run(Body body) {
        if (internal::is_graph_active(*this)) {
            task* rtask = new (task::allocate_additional_child_of(*root_task())) run_task< Body >(body);
            my_task_arena->execute(spawn_functor(*rtask));
        }
    }

    /** The waiting thread will go off and steal work while it is block in the wait_for_all. */
    void wait_for_all() {
        cancelled = false;
        caught_exception = false;
        if (my_root_task) {
#if TBB_USE_EXCEPTIONS
            try {
#endif
                my_task_arena->execute(wait_functor(my_root_task));
                cancelled = my_context->is_group_execution_cancelled();
#if TBB_USE_EXCEPTIONS
            }
            catch (...) {
                my_root_task->set_ref_count(1);
                my_context->reset();
                caught_exception = true;
                cancelled = true;
                throw;
            }
#endif

            if (!(my_context->traits() & tbb::task_group_context::concurrent_wait)) {
                my_context->reset();  
                my_root_task->set_ref_count(1);
            }
        }
    }

    tbb::task * root_task() {
        return my_root_task;
    }

    template<typename C, typename N>
    friend class graph_iterator;

    typedef graph_iterator<graph, graph_node> iterator;
    typedef graph_iterator<const graph, const graph_node> const_iterator;

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    const_iterator cbegin() const;

    const_iterator cend() const;

    bool is_cancelled() { return cancelled; }
    bool exception_thrown() { return caught_exception; }

    void reset(reset_flags f = rf_reset_protocol);

private:
    tbb::task *my_root_task;
    tbb::task_group_context *my_context;
    bool own_context;
    bool cancelled;
    bool caught_exception;
    bool my_is_active;
    task_list_type my_reset_task_list;

    graph_node *my_nodes, *my_nodes_last;

    tbb::spin_mutex nodelist_mutex;
    void register_node(graph_node *n);
    void remove_node(graph_node *n);

    tbb::task_arena* my_task_arena;

#if __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES
    internal::graph_task_priority_queue_t my_priority_queue;
#endif

    friend void internal::activate_graph(graph& g);
    friend void internal::deactivate_graph(graph& g);
    friend bool internal::is_graph_active(graph& g);
    friend void internal::spawn_in_graph_arena(graph& g, tbb::task& arena_task);
    friend void internal::add_task_to_graph_reset_list(graph& g, tbb::task *tp);
    template<typename F> friend void internal::execute_in_graph_arena(graph& g, F& f);
#if __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES
    template<typename Input, typename Output, typename Policy, typename Allocator>
    friend class async_node;
#endif

    friend class tbb::interface7::internal::task_arena_base;

};  

class graph_node : tbb::internal::no_copy {
    friend class graph;
    template<typename C, typename N>
    friend class graph_iterator;
protected:
    graph& my_graph;
    graph_node *next, *prev;
public:
    explicit graph_node(graph& g);

    virtual ~graph_node();

#if TBB_PREVIEW_FLOW_GRAPH_TRACE
    virtual void set_name(const char *name) = 0;
#endif

#if TBB_DEPRECATED_FLOW_NODE_EXTRACTION
    virtual void extract() = 0;
#endif

protected:

    virtual void reset_node(reset_flags f = rf_reset_protocol) = 0;
};  

namespace internal {

inline void activate_graph(graph& g) {
    g.my_is_active = true;
}

inline void deactivate_graph(graph& g) {
    g.my_is_active = false;
}

inline bool is_graph_active(graph& g) {
    return g.my_is_active;
}

template<typename F>
inline void execute_in_graph_arena(graph& g, F& f) {
    if (is_graph_active(g)) {
        __TBB_ASSERT(g.my_task_arena && g.my_task_arena->is_active(), NULL);
        g.my_task_arena->execute(f);
    }
}

inline void spawn_in_graph_arena(graph& g, tbb::task& arena_task) {
    task* task_to_spawn = &arena_task;
#if __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES

    graph_task* t = static_cast<graph_task*>(&arena_task);
    if( t->priority != no_priority ) {

        task_to_spawn = new( t->allocate_continuation() ) priority_task_selector(g.my_priority_queue);
        tbb::internal::make_critical( *task_to_spawn );
        g.my_priority_queue.push(t);
    }
#endif /* __TBB_PREVIEW_FLOW_GRAPH_PRIORITIES */
    graph::spawn_functor s_fn(*task_to_spawn);
    execute_in_graph_arena(g, s_fn);
}

inline void add_task_to_graph_reset_list(graph& g, tbb::task *tp) {
    g.my_reset_task_list.push_back(tp);
}

} 

} 
} 
} 

#endif 
