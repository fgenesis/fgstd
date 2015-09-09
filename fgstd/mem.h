#pragma once

#include <string.h> // for memcpy
#include "util.h" // for FGSTD_MOVE

namespace fgstd {

// --------------
namespace priv {

template<typename T, bool usememset>
struct mem_set_helper;

template<typename T>
struct mem_set_helper<T, true>
{
    FGSTD_FORCE_INLINE static void doit(T *p, u32 n, T val)
    {
        memset(p, val, n);
    }
};

template<typename T>
struct mem_set_helper<T, false>
{
    FGSTD_FORCE_INLINE static void doit(T *p, u32 n, const T& obj)
    {
        for(u32 i = 0; i < n; ++i)
            p[i] = obj;
    }
};

}
// ------------


template<typename T>
void mem_copy_distinct(T *dst, const T *src, u32 n)
{
    if(is_pod<T>::value)
        memcpy(dst, src, n * sizeof(T));
    else
        for(u32 i = 0; i < n; ++i)
            dst[i] = src[i];
}

template<typename T>
void mem_move_distinct(T *dst, const T *src, u32 n)
{
    if(is_pod<T>::value)
        memcpy(dst, src, n * sizeof(T));
    else
        for(u32 i = 0; i < n; ++i)
            dst[i] = FGSTD_MOVE(src[i]);
}


template<typename T>
void mem_construct_default(T *p, u32 n)
{
    if(!is_pod<T>::value)
         for(u32 i = 0; i < n; ++i)
            new (addressof(p[i])) T();
}

template<typename T>
void mem_construct_from(T *p, u32 n, const T& obj)
{
    if(is_pod<T>::value)
        priv::mem_set_helper<T, sizeof(T) == 1 && is_integral<T>::value>::doit(p, n, obj);
    else
        for(u32 i = 0; i < n; ++i)
            new (addressof(p[i])) T(obj);
}

template<typename T>
void mem_construct_copy(T *dst, const T *src, u32 n)
{
    if(is_pod<T>::value)
        memcpy(dst, src, n * sizeof(T));
    else
        for(u32 i = 0; i < n; ++i)
            new (addressof(dst[i])) T(src[i]);
}

template<typename T>
void mem_construct_move(T *dst, T *src, u32 n)
{
    if(is_pod<T>::value)
        memcpy(dst, src, n * sizeof(T));
    else
        for(u32 i = 0; i < n; ++i)
            new (addressof(dst[i])) T(FGSTD_MOVE(src[i]));
}

template<typename T>
void mem_construct_move_destroy(T *dst, T *src, u32 n)
{
    if(is_pod<T>::value)
        memcpy(dst, src, n * sizeof(T));
    else
        for(u32 i = 0; i < n; ++i)
        {
#ifdef FGSTD_USE_CPP11
            new (addressof(dst[i])) T(FGSTD_MOVE(src[i]));
#else
            new (addressof(dst[i])) T();
            swap(dst[i], src[i]);
#endif
            src[i].~T();
        }
#ifdef FGSTD_INTERNAL_DEBUG
    memset(src, 0xaf, n * sizeof(T));
#endif
}

template<typename T>
void mem_fill(T *dst, u32 n, const T& obj)
{
    if(is_pod<T>::value)
        priv::mem_set_helper<T, sizeof(T) == 1 && is_integral<T>::value>::doit(p, n, obj);
    else
        for(u32 i = 0; i < n; ++i)
            dst[i] = obj;
}


template<typename T>
void mem_destroy(T *p, u32 n)
{
    if(!is_pod<T>::value)
        for(u32 i = 0; i < n; ++i)
            p[i].~T();
#ifdef FGSTD_INTERNAL_DEBUG
    memset(p, 0xae, n * sizeof(T));
#endif
}



}
