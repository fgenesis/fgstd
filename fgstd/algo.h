#pragma once

#include "traits.h"
#include "util.h"
#include "iterator.h"

namespace fgstd {

template<typename IT>
void reverse(IT a, IT b)
{
    while ((a != b) && (a != --b))
    {
        iter_swap(a, b);
        ++a;
    }
}


template <typename IT, typename CMP = less<typename get_value_type<IT>::type> > class heap
{
public:
    typedef typename get_value_type<IT>::type value_type;

private:
    IT _begin;
    u32 _size;
    CMP _less;

public:

    FGSTD_FORCE_INLINE heap(IT begin, IT end)
        : _begin(begin), _size(end - begin)
    {
    }

    FGSTD_FORCE_INLINE heap(IT begin, u32 sz)
        : _begin(begin), _size(sz)
    {
    }

    FGSTD_FORCE_INLINE void insert(const value_type& val)
    {
        if(_less(val, _array[0]))
            return;
        _begin[0] = val;
        trickleDown(0);
    }

private:

    FGSTD_FORCE_INLINE static u32 parent(u32 index)
    {
        return (index - 1) >> 1;
    }
    FGSTD_FORCE_INLINE static u32 leftOf(u32 index)
    {
         return (index << 1) + 1;
    }

    void bubbleUp(unsigned int index)
    {
        value_type item = FGSTD_MOVE(_begin[index]);
        u32 p = parent(index);
        while (index > 0 && _less(_begin[index], _begin[p]))
        {
            _begin[index] = _begin[p];
            index = p;
            p = parent(index);
        }
        _begin[index] = FGSTD_MOVE(item);
    }

    // Push low priority items from the top down
    void trickleDown(u32 index)
    {
        value_type item = FGSTD_MOVE(_begin[index]);
        u32 child = leftOf(index);
        while (child < _size)
        {
            // be sure we stay inside of the array, if so, choose the smaller element
            if (child + 1 < _size)
            {
                if (_less(_begin[child + 1], _begin[child]))
                    child++;
            }
            _begin[index] = _begin[child];
            index = child;
            child = leftOf(index);
        }
        // Put the former root in its correct place
        _begin[index] = FGSTD_MOVE(item);
        bubbleUp(index);
    }
};


}


