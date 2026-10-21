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

#ifndef __TBB__flow_graph_item_buffer_impl_H
#define __TBB__flow_graph_item_buffer_impl_H

#ifndef __TBB_flow_graph_H
#error Do not #include this internal file directly; use public TBB headers instead.
#endif

#include "tbb/internal/_flow_graph_types_impl.h"  

    using internal::aligned_pair;
    using internal::alignment_of;

namespace internal {

    template <typename T, typename A=cache_aligned_allocator<T> >
    class item_buffer {
    public:
        typedef T item_type;
        enum buffer_item_state { no_item=0, has_item=1, reserved_item=2 };
    protected:
        typedef size_t size_type;
        typedef typename aligned_pair<item_type, buffer_item_state>::type buffer_item_type;
        typedef typename A::template rebind<buffer_item_type>::other allocator_type;

        buffer_item_type *my_array;
        size_type my_array_size;
        static const size_type initial_buffer_size = 4;
        size_type my_head;
        size_type my_tail;

        bool buffer_empty() const { return my_head == my_tail; }

        buffer_item_type &item(size_type i) {
            __TBB_ASSERT(!(size_type(&(my_array[i&(my_array_size-1)].second))%alignment_of<buffer_item_state>::value),NULL);
            __TBB_ASSERT(!(size_type(&(my_array[i&(my_array_size-1)].first))%alignment_of<item_type>::value), NULL);
            return my_array[i & (my_array_size - 1) ];
        }

        const buffer_item_type &item(size_type i) const {
            __TBB_ASSERT(!(size_type(&(my_array[i&(my_array_size-1)].second))%alignment_of<buffer_item_state>::value), NULL);
            __TBB_ASSERT(!(size_type(&(my_array[i&(my_array_size-1)].first))%alignment_of<item_type>::value), NULL);
            return my_array[i & (my_array_size-1)];
        }

        bool my_item_valid(size_type i) const { return (i < my_tail) && (i >= my_head) && (item(i).second != no_item); }
        bool my_item_reserved(size_type i) const { return item(i).second == reserved_item; }

        const item_type &get_my_item(size_t i) const {
            __TBB_ASSERT(my_item_valid(i),"attempt to get invalid item");
            item_type *itm = (tbb::internal::punned_cast<item_type *>(&(item(i).first)));
            return *(const item_type *)itm;
        }

        void set_my_item(size_t i, const item_type &o) {
            if(item(i).second != no_item) {
                destroy_item(i);
            }
            new(&(item(i).first)) item_type(o);
            item(i).second = has_item;
        }

        void fetch_item(size_t i, item_type &o) {
            __TBB_ASSERT(my_item_valid(i), "Trying to fetch an empty slot");
            o = get_my_item(i);  
            destroy_item(i);
        }

        void move_item(size_t to, size_t from) {
            __TBB_ASSERT(!my_item_valid(to), "Trying to move to a non-empty slot");
            __TBB_ASSERT(my_item_valid(from), "Trying to move from an empty slot");
            set_my_item(to, get_my_item(from));   
            destroy_item(from);

        }

        bool place_item(size_t here, const item_type &me) {
#if !TBB_DEPRECATED_SEQUENCER_DUPLICATES
            if(my_item_valid(here)) return false;
#endif
            set_my_item(here, me);
            return true;
        }

        void swap_items(size_t i, size_t j) {
            __TBB_ASSERT(my_item_valid(i) && my_item_valid(j), "attempt to swap invalid item(s)");
            item_type temp = get_my_item(i);
            set_my_item(i, get_my_item(j));
            set_my_item(j, temp);
        }

        void destroy_item(size_type i) {
            __TBB_ASSERT(my_item_valid(i), "destruction of invalid item");
            (tbb::internal::punned_cast<item_type *>(&(item(i).first)))->~item_type();
            item(i).second = no_item;
        }

        const item_type& front() const
        {
            __TBB_ASSERT(my_item_valid(my_head), "attempt to fetch head non-item");
            return get_my_item(my_head);
        }

        const item_type& back() const
        {
            __TBB_ASSERT(my_item_valid(my_tail - 1), "attempt to fetch head non-item");
            return get_my_item(my_tail - 1);
        }

        void reserve_item(size_type i) { __TBB_ASSERT(my_item_valid(i) && !my_item_reserved(i), "item cannot be reserved"); item(i).second = reserved_item; }
        void release_item(size_type i) { __TBB_ASSERT(my_item_reserved(i), "item is not reserved"); item(i).second = has_item; }

        void destroy_front() { destroy_item(my_head); ++my_head; }
        void destroy_back() { destroy_item(my_tail-1); --my_tail; }

        size_type size(size_t new_tail = 0) { return (new_tail ? new_tail : my_tail) - my_head; }
        size_type capacity() { return my_array_size; }

        bool buffer_full() { return size() >= capacity(); }

        void grow_my_array( size_t minimum_size ) {

            __TBB_ASSERT(capacity() >= my_tail - my_head, "total items exceed capacity");
            size_type new_size = my_array_size ? 2*my_array_size : initial_buffer_size;
            while( new_size<minimum_size )
                new_size*=2;

            buffer_item_type* new_array = allocator_type().allocate(new_size);

            for( size_type i=0; i<new_size; ++i ) { new_array[i].second = no_item; }

            for( size_type i=my_head; i<my_tail; ++i) {
                if(my_item_valid(i)) {  

                    char *new_space = (char *)&(new_array[i&(new_size-1)].first);
                    (void)new(new_space) item_type(get_my_item(i));
                    new_array[i&(new_size-1)].second = item(i).second;
                }
            }

            clean_up_buffer(/*reset_pointers*/false);

            my_array = new_array;
            my_array_size = new_size;
        }

        bool push_back(item_type &v) {
            if(buffer_full()) {
                grow_my_array(size() + 1);
            }
            set_my_item(my_tail, v);
            ++my_tail;
            return true;
        }

        bool pop_back(item_type &v) {
            if (!my_item_valid(my_tail-1)) {
                return false;
            }
            v = this->back();
            destroy_back();
            return true;
        }

        bool pop_front(item_type &v) {
            if(!my_item_valid(my_head)) {
                return false;
            }
            v = this->front();
            destroy_front();
            return true;
        }

        void clean_up_buffer(bool reset_pointers) {
            if (my_array) {
                for( size_type i=my_head; i<my_tail; ++i ) {
                    if(my_item_valid(i))
                        destroy_item(i);
                }
                allocator_type().deallocate(my_array,my_array_size);
            }
            my_array = NULL;
            if(reset_pointers) {
                my_head = my_tail = my_array_size = 0;
            }
        }

    public:

        item_buffer( ) : my_array(NULL), my_array_size(0),
            my_head(0), my_tail(0) {
            grow_my_array(initial_buffer_size);
        }

        ~item_buffer() {
            clean_up_buffer(/*reset_pointers*/true);
        }

        void reset() { clean_up_buffer(/*reset_pointers*/true); grow_my_array(initial_buffer_size); }

    };

    template<typename T, typename A=cache_aligned_allocator<T> >
    class reservable_item_buffer : public item_buffer<T, A> {
    protected:
        using item_buffer<T, A>::my_item_valid;
        using item_buffer<T, A>::my_head;

    public:
        reservable_item_buffer() : item_buffer<T, A>(), my_reserved(false) {}
        void reset() {my_reserved = false; item_buffer<T,A>::reset(); }
    protected:

        bool reserve_front(T &v) {
            if(my_reserved || !my_item_valid(this->my_head)) return false;
            my_reserved = true;

            v = this->front();
            this->reserve_item(this->my_head);
            return true;
        }

        void consume_front() {
            __TBB_ASSERT(my_reserved, "Attempt to consume a non-reserved item");
            this->destroy_front();
            my_reserved = false;
        }

        void release_front() {
            __TBB_ASSERT(my_reserved, "Attempt to release a non-reserved item");
            this->release_item(this->my_head);
            my_reserved = false;
        }

        bool my_reserved;
    };

}  

#endif 
