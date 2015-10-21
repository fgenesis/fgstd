#pragma once

#include "vector_def.h"
#include "platform.h"
#include "traits.h"
//#include "scalar.h"
#include "allocator.h"
#include "util.h"
#include "mem.h"
#include <stdlib.h>

namespace fgstd {

template<typename T>
vector<T>::vector(IAllocator *a)
    : _arr(NULL), _sz(0), _capacity(0), _alloc(a)
{
}

template<typename T>
vector<T>::vector(u32 n, const T& def, IAllocator *a)
: _arr(NULL), _sz(0), _capacity(0), _alloc(a)
{
    if(n)
        resize(n, def);
}

template<typename T>
vector<T>::vector(const T *mem, u32 n, IAllocator *a)
: _arr(NULL), _sz(0), _capacity(0), _alloc(a)
{
    if(n)
    {
        _resize_noinit(n);
        mem_construct_copy(_arr, mem, n);
    }
}

template<typename T> template<u32 SZ>
vector<T>::vector(T (&arr)[SZ], IAllocator *a)
#ifdef FGSTD_USE_CPP11
: vector(&arr[0], SZ, a) {}
#else
: _arr(NULL), _sz(0), _capacity(0), _alloc(a)
{
    if(SZ)
    {
        _resize_noinit(SZ);
        mem_construct_copy(_arr, &arr[0], SZ);
    }
}
#endif

template<typename T>
vector<T>::vector(const vector<T>& v)
: _arr(NULL), _sz(0), _capacity(0), _alloc(v._alloc)
{
    if(v._sz)
    {
        _resize_noinit(v._sz);
        mem_construct_copy(_arr, v._arr, v._sz);
    }
}

template<typename T>
void vector<T>::_kill()
{
    _arr = NULL;
    _sz = 0;
    _capacity = 0;
}

#ifdef FGSTD_USE_CPP11
template<typename T>
vector<T>::vector(vector<T>&& v)
    : _arr(v._arr), _sz(v._sz), _capacity(v._capacity), _alloc(v._alloc)
{
    v._kill();
}
#endif

template<typename T>
vector<T>::~vector()
{
    dealloc();
}

template<typename T>
void vector<T>::clear()
{
    mem_destroy(_arr, _sz);
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
FGSTD_FORCE_INLINE bool vector<T>::empty() const
{
    return !_sz;
}


template<typename T>
void vector<T>::resize(u32 n)
{
    const u32 oldsz = _sz;
    _resize_noinit(n);
    if(n > oldsz)
        mem_construct_default(_arr + oldsz, n - oldsz);
}

template<typename T>
void vector<T>::resize(u32 n, const T& val)
{
    const u32 oldsz = _sz;
    _resize_noinit(n);
    if(n > oldsz)
        mem_construct_from(_arr + oldsz, n - oldsz, val);
}

template<typename T>
void vector<T>::_resize_noinit(u32 n)
{
    const u32 oldsz = _sz;
    if(n < oldsz)
        mem_destroy(_arr + oldsz, oldsz - n);
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
FGSTD_FORCE_INLINE void vector<T>::pop_back()
{
    if(is_pod<T>::value)
        _arr[--_sz];
    else
        _arr[--_sz].~T();
}

template<typename T>
FGSTD_FORCE_INLINE T vector<T>::get_pop_back()
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
    const u32 cap = _capacity;
    if(_sz >= cap)
        reserve(cap + (cap >> 1) + 4);

    T& ref = _arr[_sz++];
    if(!is_pod<T>::value)
        new (&ref) T(e);
    else
        ref = e;
    return ref;
}

#ifdef FGSTD_USE_CPP11
template<typename T>
T& vector<T>::push_back(T&& e)
{
    const u32 cap = _capacity;
    if (_sz >= cap)
        reserve(cap + (cap >> 1) + 4);

    T& ref = _arr[_sz++];
    if (!is_pod<T>::value)
        new (&ref) T(e);
    else
        ref = e;
    return ref;
}
#endif


template<typename T>
void *vector<T>::emplace_new()
{
    const u32 cap = _capacity;
    if (_sz >= cap)
        reserve(cap + (cap >> 1) + 4);

    return &_arr[_sz++];
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
    u32 bytes = n * sizeof(T);
    T* p = (T*)_alloc->Alloc(bytes);
    if (_arr)
    {
        mem_construct_move_destroy(p, _arr, _sz);
        _alloc->Free(_arr);
    }
    _arr = p;
    _capacity = n;
}

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::fill(const T& x)
{
    mem_fill(_arr, _sz ,x);
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

        swap(_sz, v._sz);
    }
}

#ifdef FGSTD_USE_CPP11
template<typename T>
FGSTD_FORCE_INLINE vector<T>& vector<T>::operator=(vector<T> v)
{
    v.swap(*this);
    return *this;
}
#else
template<typename T>
FGSTD_FORCE_INLINE vector<T>&  vector<T>::operator=(const vector<T> &v)
{
    if(&v != this)
    {
        _resize_noinit(v._sz);
        mem_copy_distinct(_arr, v._arr, v._sz);
    }
    return *this;
}
#endif

template<typename T>
FGSTD_FORCE_INLINE void vector<T>::swap(vector<T> &v)
{
    fgstd::swap(_sz, v._sz);
    fgstd::swap(_capacity, v._capacity);
    fgstd::swap(_arr, v._arr);
    fgstd::swap(_alloc, v._alloc);
}

template<typename T>
FGSTD_FORCE_INLINE IAllocator *vector<T>::get_alloc()
{
    return _alloc;
}

template<typename T>
FGSTD_FORCE_INLINE T& vector<T>::back()
{
    return _arr[_sz - 1];
}

template<typename T> template<typename E>
vector<T>::vector(const E& e, IAllocator *a, typename E::is_expr_tag)
: _arr(NULL), _sz(0), _capacity(0), _alloc(a)
{
    const u32 sz = e.size();
    _resize_noinit(sz);
    et::expr_store(_arr, e);
}

// ---- Expression template support ----
template<typename T> template<typename E>
FGSTD_FORCE_INLINE typename enable_if<et::is_usable_expr<E>::value, vector<T>&>::type
vector<T>::operator=(const E& e)
{
    const u32 sz = e.size();
    _resize_noinit(sz);
    et::expr_store(_arr, e);
    return *this;
}


// -------------------------------------



}
