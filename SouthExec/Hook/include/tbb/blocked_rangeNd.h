// Leaked By Cry Service & Siraj
/*
    Copyright (c) 2017-2018 Intel Corporation

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

#ifndef __TBB_blocked_rangeNd_H
#define __TBB_blocked_rangeNd_H

#if ! TBB_PREVIEW_BLOCKED_RANGE_ND
    #error Set TBB_PREVIEW_BLOCKED_RANGE_ND to include blocked_rangeNd.h
#endif

#include "tbb_config.h"

#if __TBB_CPP11_PRESENT && __TBB_CPP11_ARRAY_PRESENT && __TBB_CPP11_TEMPLATE_ALIASES_PRESENT

#include "internal/_template_helpers.h" 

#include <array>
#include <algorithm>    
#include <type_traits>  

#include "tbb/blocked_range.h"

namespace tbb {
namespace internal {

/*
    The blocked_rangeNd_impl uses make_index_sequence<N> to automatically generate a ctor with
    exactly N arguments of the type tbb::blocked_range<Value>. Such ctor provides an opportunity
    to use braced-init-list parameters to initialize each dimension.
    Use of parameters, whose representation is a braced-init-list, but they're not
    std::initializer_list or a reference to one, produces a non-deduced context
    within template argument deduction.

    NOTE: blocked_rangeNd must be exactly a templated alias to the blocked_rangeNd_impl
    (and not e.g. a derived class), otherwise it would need to declare its own ctor
    facing the same problem that the impl class solves.
*/

template<typename Value, unsigned int N, typename = make_index_sequence<N>>
class blocked_rangeNd_impl;

template<typename Value, unsigned int N, std::size_t... Is>
class blocked_rangeNd_impl<Value, N, index_sequence<Is...>> {
public:

    using value_type = Value;

private:

    template<std::size_t>
    using dim_type_helper = tbb::blocked_range<value_type>;

public:
    blocked_rangeNd_impl() = delete;

    blocked_rangeNd_impl(const dim_type_helper<Is>&... args) : my_dims{ {args...} } {}

    static constexpr unsigned int ndims() { return N; }

    const tbb::blocked_range<value_type>& dim(unsigned int dimension) const {
        __TBB_ASSERT(dimension < N, "out of bound");
        return my_dims[dimension];
    }

    bool empty() const {
        return std::any_of(my_dims.begin(), my_dims.end(), [](const tbb::blocked_range<value_type>& d) {
            return d.empty();
        });
    }

    bool is_divisible() const {
        return std::any_of(my_dims.begin(), my_dims.end(), [](const tbb::blocked_range<value_type>& d) {
            return d.is_divisible();
        });
    }

#if __TBB_USE_PROPORTIONAL_SPLIT_IN_BLOCKED_RANGES

    static const bool is_splittable_in_proportion = true;

    blocked_rangeNd_impl(blocked_rangeNd_impl& r, proportional_split proportion) : my_dims(r.my_dims) {
        do_split(r, proportion);
    }
#endif

    blocked_rangeNd_impl(blocked_rangeNd_impl& r, split proportion) : my_dims(r.my_dims) {
        do_split(r, proportion);
    }

private:
    __TBB_STATIC_ASSERT(N != 0, "zero dimensional blocked_rangeNd can't be constructed");

    std::array<tbb::blocked_range<value_type>, N> my_dims;

    template<typename split_type>
    void do_split(blocked_rangeNd_impl& r, split_type proportion) {
        __TBB_STATIC_ASSERT((is_same_type<split_type, split>::value
                            || is_same_type<split_type, proportional_split>::value),
                            "type of split object is incorrect");
        __TBB_ASSERT(r.is_divisible(), "can't split not divisible range");

        auto my_it = std::max_element(my_dims.begin(), my_dims.end(), [](const tbb::blocked_range<value_type>& first, const tbb::blocked_range<value_type>& second) {
            return (first.size() * second.grainsize() < second.size() * first.grainsize());
        });

        auto r_it = r.my_dims.begin() + (my_it - my_dims.begin());

        my_it->my_begin = tbb::blocked_range<value_type>::do_split(*r_it, proportion);

        __TBB_ASSERT(!(my_it->my_begin < r_it->my_end) && !(r_it->my_end < my_it->my_begin),
                     "blocked_range has been split incorrectly");
    }
};

} 

template<typename Value, unsigned int N>
using blocked_rangeNd = internal::blocked_rangeNd_impl<Value, N>;

} 

#endif /* __TBB_CPP11_PRESENT && __TBB_CPP11_ARRAY_PRESENT && __TBB_CPP11_TEMPLATE_ALIASES_PRESENT */
#endif /* __TBB_blocked_rangeNd_H */
