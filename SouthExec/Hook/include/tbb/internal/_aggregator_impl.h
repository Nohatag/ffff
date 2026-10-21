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

#ifndef __TBB__aggregator_impl_H
#define __TBB__aggregator_impl_H

#include "../atomic.h"
#if !__TBBMALLOC_BUILD
#include "../tbb_profiling.h"
#endif

namespace tbb {
namespace interface6 {
namespace internal {

using namespace tbb::internal;

template <typename Derived>
class aggregated_operation {
 public:

    uintptr_t status;

    Derived *next;
    aggregated_operation() : status(0), next(NULL) {}
};

/** An aggregator for collecting operations coming from multiple sources and executing
    them serially on a single thread.  operation_type must be derived from
    aggregated_operation. The parameter handler_type is a functor that will be passed the
    list of operations and is expected to handle each operation appropriately, setting the
    status of each operation to non-zero.*/
template < typename operation_type >
class aggregator_generic {
public:
    aggregator_generic() : handler_busy(false) { pending_operations = NULL; }

    /** Places an operation into the waitlist (pending_operations), and either handles the list,
        or waits for the operation to complete, or returns.
        The long_life_time parameter specifies the life time of the given operation object.
        Operations with long_life_time == true may be accessed after execution.
        A "short" life time operation (long_life_time == false) can be destroyed
        during execution, and so any access to it after it was put into the waitlist,
        including status check, is invalid. As a consequence, waiting for completion
        of such operation causes undefined behavior.
    */
    template < typename handler_type >
    void execute(operation_type *op, handler_type &handle_operations, bool long_life_time = true) {
        operation_type *res;

        const uintptr_t status = op->status;

        call_itt_notify(releasing, &(op->status));

        do {

            op->next = res = pending_operations; 
        } while (pending_operations.compare_and_swap(op, res) != res);
        if (!res) { 

            call_itt_notify(acquired, &pending_operations);
            start_handle_operations(handle_operations);

            if (long_life_time)
                __TBB_ASSERT(op->status, NULL);
        }

        else if (!status) { 
            __TBB_ASSERT(long_life_time, "Waiting for an operation object that might be destroyed during processing.");
            call_itt_notify(prepare, &(op->status));
            spin_wait_while_eq(op->status, uintptr_t(0));
            itt_load_word_with_acquire(op->status);
        }
    }

 private:

    atomic<operation_type *> pending_operations;

    uintptr_t handler_busy;

    template < typename handler_type >
    void start_handle_operations( handler_type &handle_operations ) {
        operation_type *op_list;

        call_itt_notify(prepare, &handler_busy);

        spin_wait_until_eq(handler_busy, uintptr_t(0));
        call_itt_notify(acquired, &handler_busy);

        __TBB_store_with_release(handler_busy, uintptr_t(1));

        call_itt_notify(releasing, &pending_operations);

        op_list = pending_operations.fetch_and_store(NULL);

        handle_operations(op_list);

        itt_store_word_with_release(handler_busy, uintptr_t(0));
    }
};

template < typename handler_type, typename operation_type >
class aggregator : public aggregator_generic<operation_type> {
    handler_type handle_operations;
public:
    aggregator() {}
    explicit aggregator(handler_type h) : handle_operations(h) {}

    void initialize_handler(handler_type h) { handle_operations = h; }

    void execute(operation_type *op) {
        aggregator_generic<operation_type>::execute(op, handle_operations);
    }
};

template<typename aggregating_class, typename operation_list>
class aggregating_functor {
    aggregating_class *fi;
public:
    aggregating_functor() {}
    aggregating_functor(aggregating_class *fi_) : fi(fi_) {}
    void operator()(operation_list* op_list) { fi->handle_operations(op_list); }
};

} 
} 

namespace internal {
    using interface6::internal::aggregated_operation;
    using interface6::internal::aggregator_generic;
    using interface6::internal::aggregator;
    using interface6::internal::aggregating_functor;
} 

} 

#endif  
