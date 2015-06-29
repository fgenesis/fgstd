#pragma once

#include "platform.h"

namespace fgstd {

namespace priv {

template <typename T, T v>
struct IntegralConstant
{
    typedef T ValueType;
    typedef IntegralConstant<T,v> type;
    enum { value = v };
};

typedef IntegralConstant<bool, true>  CompileTrue;
typedef IntegralConstant<bool, false> CompileFalse;

}

template <bool B, typename T = void> struct enable_if          { };
template <typename T>                struct enable_if<true, T> { typedef T type; };

template<typename T, typename U> struct is_same;

template <typename T> struct remove_ref            { typedef T  type; };
template <typename T> struct remove_ref<T&>        { typedef T  type; };
#ifdef STDFG_USE_CPP11
template <typename T> struct remove_ref<T&&>       { typedef T  type; };
#endif



#ifndef STDFG_USE_CPP11
namespace _align {
// via http://www.wambold.com/Martin/writings/alignof.html

// alignof (T) must be a power of two which is a factor of sizeof (T).
template <typename U>
struct _Alignof1
{
    enum { s = sizeof(U), value = s ^ (s & (s - 1)) };
};
// Put T in a struct, keep adding chars until a "quantum jump" in
// the size occurs.
template <typename U> struct _Alignof2;

template <int size_diff>
struct helper
{
    template <typename U> struct Val { enum { value = size_diff }; };
};

template <>
struct helper<0>
{
    template <typename U> struct Val { enum { value = _Alignof2<U>::value }; };
};

template <typename U>
struct _Alignof2
{
    struct Big { U x; char c; };

    enum
    {
        diff = sizeof (Big) - sizeof (U),
        value = helper<diff>::template Val<Big>::value
    };
};
}
#endif

template<typename T>
struct AlignedSize
{
#ifdef FGSTD_USE_CPP11
    enum { value = alignof(T) };
#else
    enum
    {
        _1 = _align::_Alignof1<T>::value,
        _2 = _align::_Alignof2<T>::value,
        value = (unsigned)_1 < (unsigned)_2 ? _1 : _2
    };
#endif
};

template <typename T, typename U> struct is_same      : priv::CompileFalse { };
template <typename T>             struct is_same<T,T> : priv::CompileTrue  { };

template<typename T> struct is_pod : priv::CompileFalse {};



}
