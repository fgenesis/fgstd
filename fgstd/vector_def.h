#pragma once

#include "types.h"

namespace fgstd {

class IAllocator;
extern IAllocator *g_defaultAlloc;

template<typename T>
class vector
{
public:
    typedef u32 size_type;
    typedef T value_type;
    typedef T& reference;

    vector(IAllocator *a = g_defaultAlloc);
    vector(u32 sz, const T& def = T(), IAllocator *a = g_defaultAlloc);
    vector(const T *mem, u32 n, IAllocator *a = g_defaultAlloc);

    template<u32 SZ>
    vector(T (&arr)[SZ], IAllocator *a = g_defaultAlloc);

    vector(const vector<T>& v);
#ifdef FGSTD_USE_CPP11
    vector(vector<T>&& v);
#endif
    ~vector();

    // const
    u32 size() const;
    bool empty() const;
    const T& operator[](u32 i) const;
    u32 remain() const;
    const T *data() const;

    // modifying
    void clear();
    void dealloc();
    void fill(const T& x);
    void resize(u32 n);
    void resize(u32 n, const T& val);
    void reserve(u32 n);
    void pop_back();
    T get_pop_back();
    bool try_pop_back(T& e);
    T& push_back(const T& e);
#ifdef FGSTD_USE_CPP11
    T& push_back(T&& e);
#endif
    //u32 shift_left(u32 n);
    T& operator[](u32 i);
    void swapElems(vector<T>& v);
    void swap(vector<T>& v);
    T *data();
    void *emplace_new();
    T& back();

    // ops
#ifdef FGSTD_USE_CPP11
    vector<T>& operator=(vector<T> v);
#else
    vector<T>& operator=(const vector<T>& v);
#endif

    IAllocator *get_alloc();


    /*class const_iterator
    {
        public:
        const_iterator();
        ~const_iterator();
        const_iterator(const const_iterator&);
        const_iterator& operator=(const_iterator);
        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator +=(i32);
        const_iterator operator+(i32) const;
        const_iterator& operator--();
        const_iterator operator--(int);
        const_iterator& operator -=(i32);
        const_iterator operator-(i32) const;
        const reference operator*() const;
        const i32 operator-(const_iterator&) const;
        const reference operator[](i32) const;
        bool operator==(const_iterator&) const;
        bool operator!=(const_iterator&) const;
        bool operator<(const_iterator&) const;
        bool operator>(const_iterator&) const;
        bool operator<=(const_iterator&) const;
        bool operator>=(const_iterator&) const;
    };*/

private:
    void _enlarge(u32 n);
    void _resize_noinit(u32 n);
    void _kill();


private:
    T *_arr;
    u32 _sz;
    u32 _capacity;
    IAllocator *_alloc;
};

}

