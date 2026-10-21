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

#ifndef __TBB_blocked_range_H
#define __TBB_blocked_range_H

#include "tbb_stddef.h"

namespace tbb {

namespace internal {

template<typename Value, unsigned int N, typename>
class blocked_rangeNd_impl;

} 

/** \page range_req Requirements on range concept
    Class \c R implementing the concept of range must define:
    - \code R::R( const R& ); \endcode               Copy constructor
    - \code R::~R(); \endcode                        Destructor
    - \code bool R::is_divisible() const; \endcode   True if range can be partitioned into two subranges
    - \code bool R::empty() const; \endcode          True if range is empty
    - \code R::R( R& r, split ); \endcode            Split range \c r into two subranges.
**/

/** @ingroup algorithms */
template<typename Value>
class blocked_range {
public:

    /** Called a const_iterator for sake of algorithms that need to treat a blocked_range
        as an STL container. */
    typedef Value const_iterator;

    typedef std::size_t size_type;

#if __TBB_DEPRECATED_BLOCKED_RANGE_DEFAULT_CTOR

    /** Requires that Value have a default constructor. */
    blocked_range() : my_end(), my_begin(), my_grainsize() {}
#endif

    blocked_range( Value begin_, Value end_, size_type grainsize_=1 ) :
        my_end(end_), my_begin(begin_), my_grainsize(grainsize_)
    {
        __TBB_ASSERT( my_grainsize>0, "grainsize must be positive" );
    }

    const_iterator begin() const {return my_begin;}

    const_iterator end() const {return my_end;}

    /** Unspecified if end()<begin(). */
    size_type size() const {
        __TBB_ASSERT( !(end()<begin()), "size() unspecified if end()<begin()" );
        return size_type(my_end-my_begin);
    }

    size_type grainsize() const {return my_grainsize;}

    bool empty() const {return !(my_begin<my_end);}

    /** Unspecified if end()<begin(). */
    bool is_divisible() const {return my_grainsize<size();}

    /** The new Range *this has the second part, the old range r has the first part.
        Unspecified if end()<begin() or !is_divisible(). */
    blocked_range( blocked_range& r, split ) :
        my_end(r.my_end),
        my_begin(do_split(r, split())),
        my_grainsize(r.my_grainsize)
    {

        __TBB_ASSERT( !(my_begin < r.my_end) && !(r.my_end < my_begin), "blocked_range has been split incorrectly" );
    }

#if __TBB_USE_PROPORTIONAL_SPLIT_IN_BLOCKED_RANGES

    static const bool is_splittable_in_proportion = true;

    /** The new Range *this has the second part split according to specified proportion, the old range r has the first part.
        Unspecified if end()<begin() or !is_divisible(). */
    blocked_range( blocked_range& r, proportional_split& proportion ) :
        my_end(r.my_end),
        my_begin(do_split(r, proportion)),
        my_grainsize(r.my_grainsize)
    {

        __TBB_ASSERT( !(my_begin < r.my_end) && !(r.my_end < my_begin), "blocked_range has been split incorrectly" );
    }
#endif /* __TBB_USE_PROPORTIONAL_SPLIT_IN_BLOCKED_RANGES */

private:
    /** NOTE: my_end MUST be declared before my_begin, otherwise the splitting constructor will break. */
    Value my_end;
    Value my_begin;
    size_type my_grainsize;

    static Value do_split( blocked_range& r, split )
    {
        __TBB_ASSERT( r.is_divisible(), "cannot split blocked_range that is not divisible" );
        Value middle = r.my_begin + (r.my_end - r.my_begin) / 2u;
        r.my_end = middle;
        return middle;
    }

#if __TBB_USE_PROPORTIONAL_SPLIT_IN_BLOCKED_RANGES
    static Value do_split( blocked_range& r, proportional_split& proportion )
    {
        __TBB_ASSERT( r.is_divisible(), "cannot split blocked_range that is not divisible" );

        size_type right_part = size_type(float(r.size()) * float(proportion.right())
                                         / float(proportion.left() + proportion.right()) + 0.5f);
        return r.my_end = Value(r.my_end - right_part);
    }
#endif /* __TBB_USE_PROPORTIONAL_SPLIT_IN_BLOCKED_RANGES */

    template<typename RowValue, typename ColValue>
    friend class blocked_range2d;

    template<typename RowValue, typename ColValue, typename PageValue>
    friend class blocked_range3d;

    template<typename DimValue, unsigned int N, typename>
    friend class internal::blocked_rangeNd_impl;
};

} 

#endif /* __TBB_blocked_range_H */
