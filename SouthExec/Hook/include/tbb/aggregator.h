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

#ifndef __TBB__aggregator_H
#define __TBB__aggregator_H

#if !TBB_PREVIEW_AGGREGATOR
#error Set TBB_PREVIEW_AGGREGATOR before including aggregator.h
#endif

#include "atomic.h"
#include "tbb_profiling.h"

namespace tbb {
namespace interface6 {

using namespace tbb::internal;

class aggregator_operation {
    template<typename handler_type> friend class aggregator_ext;
    uintptr_t status;
    aggregator_operation* my_next;
public:
    enum aggregator_operation_status { agg_waiting=0, agg_finished };
    aggregator_operation() : status(agg_waiting), my_next(NULL) {}

    void start() { call_itt_notify(acquired, &status); }

    /** The operation will be released to its originating thread, and possibly deleted. */
    void finish() { itt_store_word_with_release(status, uintptr_t(agg_finished)); }
    aggregator_operation* next() { return itt_hide_load_word(my_next);}
    void set_next(aggregator_operation* n) { itt_hide_store_word(my_next, n); }
};

namespace internal {

class basic_operation_base : public aggregator_operation {
    friend class basic_handler;
    virtual void apply_body() = 0;
public:
    basic_operation_base() : aggregator_operation() {}
    virtual ~basic_operation_base() {}
};

template<typename Body>
class basic_operation : public basic_operation_base, no_assign {
    const Body& my_body;
    void apply_body() __TBB_override { my_body(); }
public:
    basic_operation(const Body& b) : basic_operation_base(), my_body(b) {}
};

class basic_handler {
public:
    basic_handler() {}
    void operator()(aggregator_operation* op_list) const {
        while (op_list) {

            basic_operation_base& request = static_cast<basic_operation_base&>(*op_list);

            op_list = op_list->next();
            request.start();
            request.apply_body();
            request.finish();
        }
    }
};

} 

/** An aggregator for collecting operations coming from multiple sources and executing
    them serially on a single thread. */
template <typename handler_type>
class aggregator_ext : tbb::internal::no_copy {
public:
    aggregator_ext(const handler_type& h) : handler_busy(0), handle_operations(h) { mailbox = NULL; }

    /** Details of user-made operations must be handled by user-provided handler */
    void process(aggregator_operation *op) { execute_impl(*op); }

protected:
    /** Place operation in mailbox, then either handle mailbox or wait for the operation
        to be completed by a different thread. */
    void execute_impl(aggregator_operation& op) {
        aggregator_operation* res;

        call_itt_notify(releasing, &(op.status));

        do {

            op.my_next = res = mailbox; 
        } while (mailbox.compare_and_swap(&op, res) != res);
        if (!res) { 

            call_itt_notify(acquired, &mailbox);
            start_handle_operations();
            __TBB_ASSERT(op.status, NULL);
        }
        else { 
            call_itt_notify(prepare, &(op.status));
            spin_wait_while_eq(op.status, uintptr_t(aggregator_operation::agg_waiting));
            itt_load_word_with_acquire(op.status);
        }
    }

private:

    atomic<aggregator_operation *> mailbox;

    /** Behaves as boolean flag where 0=false, 1=true */
    uintptr_t handler_busy;

    handler_type handle_operations;

    void start_handle_operations() {
        aggregator_operation *pending_operations;

        call_itt_notify(prepare, &handler_busy);

        spin_wait_until_eq(handler_busy, uintptr_t(0));
        call_itt_notify(acquired, &handler_busy);

        __TBB_store_with_release(handler_busy, uintptr_t(1));

        call_itt_notify(releasing, &mailbox);

        pending_operations = mailbox.fetch_and_store(NULL);

        handle_operations(pending_operations);

        itt_store_word_with_release(handler_busy, uintptr_t(0));
    }
};

class aggregator : private aggregator_ext<internal::basic_handler> {
public:
    aggregator() : aggregator_ext<internal::basic_handler>(internal::basic_handler()) {}

    /** The calling thread stores the function object in a basic_operation and
        places the operation in the aggregator's mailbox */
    template<typename Body>
    void execute(const Body& b) {
        internal::basic_operation<Body> op(b);
        this->execute_impl(op);
    }
};

} 

using interface6::aggregator;
using interface6::aggregator_ext;
using interface6::aggregator_operation;

} 

#endif  
