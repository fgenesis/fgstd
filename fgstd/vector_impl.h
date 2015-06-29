#pragma once

#include "vector_def.h"
#include "platform.h"
#include "traits.h"
//#include "scalar.h"
#include "allocator.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

namespace fgstd {

template<typename T>
vector<T>::vector(u32 n, const T& def, IAllocator *a)
: _arr(NULL), _sz(0), _capacity(0), _alloc(a)
{
    if(n)
    {
        resize(n);
        fill(def);
    }
}

template<typename T>
vector<T>::vector(const T *mem, u32 n, IAllocator *a)
: _arr(NULL), _sz(0), _capacity(0), _alloc(a)
{
    if(n)
    {
        _resize_noinit(n);
        _initcopyp(_arr, mem, n);
    }
}

template<typename T> template<u32 SZ>
vector<T>::vector(T (&arr)[SZ], IAllocator *a)
: _arr(NULL), _sz(0), _capacity(0), _alloc(a)
{
    if(SZ)
    {
        _resize_noinit(SZ);
        _initcopyp(_arr, &arr[0], SZ);
    }
}

template<typename T>
vector<T>::vector(const vector<T>& v)
: _arr(NULL), _sz(0), _capacity(0), _alloc(v._alloc)
{
    if(v._sz)
    {
        _resize_noinit(v._sz);
        _initcopyp(_arr, v._arr, v._sz);
    }
}

template<typename T> template<typename E>
vector<T>::vector(const E& e, IAllocator *a)
: _arr(NULL), _sz(0), _capacity(0), _alloc(a)
{
    if(const u32 sz = e.size())
    {
        _resize_noinit(sz);
        if(is_pod<T>::value)
            for(u32 i = 0; i < sz; ++i)
                _arr[i] = e[i];
        else
            for(u32 i = 0; i < sz; ++i)
                new (&_arr[i]) T(e[i]);
    }
}

template<typename T>
vector<T>::~vector()
{
    dealloc();
}

template<typename T>
void vector<T>::clear()
{
    _destroy(0, _sz);
    _sz = 0;
}
template<typename T>
void vector<T>::dealloc()
{
    clear();
    _alloc->Free(_arr);
    _arr = NULL;
    _capacity = 0;
}

template<typename T>
FGSTD_FORCE_INLINE u32 vector<T>::size() const
{
    return _sz;
}

template<typename T>
void vector<T>::resize(u32 n)
{
    const u32 oldsz = _sz;
    _resize_noinit(n);
    if(!is_pod<T>::value)
        for(u32 i = oldsz; i < n; ++i)
            new (&_arr[i]) T();
}

template<typename T>
void vector<T>::_resize_noinit(u32 n)
{
    if(!is_pod<T>::value)
    {
        const u32 sz = _sz;
        for(u32 i = n; i < sz; ++i)
            _arr[i].~T();
    }
    reserve(n);
    _sz = n;
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::reserve(u32 n)
{
    if(_capacity < n)
        _enlarge(n);
}

template<typename T>
FGSTD_FORCE_INLINE unsigned vector<T>::remain() const
{
    return _capacity - _sz;
}

template<typename T>
FGSTD_FORCE_INLINE T vector<T>::pop_back()
{
    if(is_pod<T>::value)
        return FGSTD_MOVE(_arr[--_sz]);
    else
    {
        T ret = FGSTD_MOVE(_arr[--_sz]);
        _arr[_sz].~T();
        return ret;
    }
}

template<typename T>
bool vector<T>::try_pop_back(T& e)
{
    if(!_sz)
        return false;
    e = FGSTD_MOVE(_arr[--_sz]);
    if(!is_pod<T>::value)
        _arr[_sz].~T();
    return true;
}

template<typename T>
T& vector<T>::push_back(const T& e)
{
    if(_sz >= _capacity)
        reserve(_capacity + (_capacity >> 1) + 4);
    if(!is_pod<T>::value)
        new (&_arr[_sz]) T(e);
    else
        _arr[_sz] = e;
    return _arr[_sz++];
}

template<typename T>
FGSTD_FORCE_INLINE T& vector<T>::operator[](u32 i)
{
    return _arr[i];
}

template<typename T>
FGSTD_FORCE_INLINE const T& vector<T>::operator[](u32 i) const
{
    return _arr[i];
}

template<typename T>
FGSTD_FORCE_INLINE T *vector<T>::data()
{
    return _arr;
}

template<typename T>
FGSTD_FORCE_INLINE const T *vector<T>::data() const
{
    return _arr;
}


template<typename T>
FGSTD_FORCE_INLINE void vector<T>::_enlarge(u32 n)
{
    size_t bytes = n * (u32)AlignedSize<T>::value;
    T* p = (T*)_alloc->Alloc(bytes);
    if(_arr)
        _movep(p, _arr, _sz);
    _arr = p;
    _capacity = n;
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::_copyp(T *dst, const T *src, u32 n)
{
    if(is_pod<T>::value)
    {
        const size_t bytes = n * (u32)AlignedSize<T>::value;
        memcpy(dst, src, bytes);
    }
    else
    {
        for(u32 i = 0; i < n; ++i)
            dst[i] = src[i];
    }
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::_initcopyp(T *dst, const T *src, u32 n)
{
    if(is_pod<T>::value)
    {
        const size_t bytes = n * AlignedSize<T>::value;
        memcpy(dst, src, bytes);
    }
    else
    {
        for(u32 i = 0; i < n; ++i)
            new (&dst[i]) T(src[i]);
    }
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::fill(const T& x)
{
    const u32 sz = _sz;
    T *arr = _arr;
    for(u32 i = 0; i < sz; ++i)
        arr[i] = x;
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::_movep(T *dst, T *src, u32 n)
{
    // TODO: optimize for C++11
    _copyp(dst, src, n);
    _destroyp(src, n);
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::_destroyp(T *p, u32 n)
{
    for(u32 i = 0; i < n; ++i)
        p[i].~T();
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::_destroy(u32 from, u32 to)
{
    for(u32 i = from; i < to; ++i)
        _arr[i].~T();
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::swapElems(vector<T> &v)
{
    if(_alloc == v._alloc)
    {
        swap(_sz, v._sz);
        swap(_capacity, v._capacity);
        swap(_arr, v._arr);
    }
    else
    {
        const u32 mycap = _capacity;
        const u32 mysz = _sz;
        const u32 copysz = vmax(mysz, v._sz);
        reserve(v._capacity);
        resize(v._sz);
        v.reserve(mycap);
        v.resize(mysz);

        for(u32 i = 0; i < copysz; ++i)
            swap(_arr[i], v._arr[i]);
    }
}

template<typename T>
FGSTD_FORCE_INLINE vector<T>&  vector<T>::operator=(const vector<T> &v)
{
    if(&v != this)
    {
        _resize_noinit(v._sz);
        _initcopyp(_arr, v._arr, v._sz);
    }
    return *this;
}

template<typename T> template<typename E>
FGSTD_FORCE_INLINE vector<T>&  vector<T>::operator=(const E& e)
{
    u32 sz = e.size();
    _resize_noinit(sz);
    if(is_pod<T>::value)
        for(u32 i = 0; i < sz; ++i)
            _arr[i] = e[i];
    else
        for(u32 i = 0; i < sz; ++i)
            new (&_arr[i]) T(e[i]);
    return *this;
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::swapAll(vector<T> &v)
{
    swap(_sz, v._sz);
    swap(_capacity, v._capacity);
    swap(_arr, v._arr);
    swap(_alloc, v._alloc);
}

template<typename T>
FGSTD_FORCE_INLINE void swap(vector<T> &a, vector<T> &b)
{
    a.swapAll(b);
}

}