#pragma once

#include "platform.h"
#include "traits.h"

namespace fgstd {

#ifdef FGSTD_USE_CPP11
template <class T>
typename remove_ref<T>::type&& move (T&& x) FGSTD_NOEXCEPT
{
    return static_cast<remove_ref<T>::type&&>(x);
}
#define FGSTD_MOVE(x) fgstd::move(x)
#else
#define FGSTD_MOVE(x) x
#endif

namespace detail
{

template<typename A, typename B>
struct has_swap_method
{
    typedef char yes[1];
    typedef char no[2];
    template<typename A, void (A::*)(B&) const> struct SFINAE {};
    template<typename A> static yes& Test(SFINAE<A, &A::swap>*);
    template<typename A> static no& Test(...);
    enum { value = sizeof(Test<A>(0)) == sizeof(yes) };
};

template<bool>
struct swapper;


template<>
struct swapper<true>
{
    template<typename T>
    static FGSTD_FORCE_INLINE void swap(T& a, T& b)
    {
        a.swap(b);
    }
};

template<>
struct swapper<false>
{
    template<typename T>
    static FGSTD_FORCE_INLINE void swap(T& a, T& b)
    {
        T c(FGSTD_MOVE(a));
        a = FGSTD_MOVE(b);
        b = FGSTD_MOVE(c);
    }
};

} // end namespace detail


template<typename T>
FGSTD_FORCE_INLINE void swap(T& a, T& b)
{
    detail::swapper<!!detail::has_swap_method<T, T>::value>::swap(a, b);
}

template<typename T>
struct less
{
    FGSTD_FORCE_INLINE bool operator()(const T& a, const T& b) const
    {
        return a < b;
    }
};

template<typename T>
struct equal
{
    FGSTD_FORCE_INLINE bool operator()(const T& a, const T& b) const
    {
        return a == b;
    }
};

template<typename T> struct is_simple_op : priv::CompileFalse {};
template<typename T> struct is_simple_op<less<T> > : priv::CompileTrue {};
template<typename T> struct is_simple_op<equal<T> > : priv::CompileTrue{};

template<typename T>
FGSTD_FORCE_INLINE T *addressof(T& x) FGSTD_NOEXCEPT
{
    return (reinterpret_cast<T*>(
        &const_cast<char&>(
            reinterpret_cast<const volatile char&>(x))));
}

#ifdef FGSTD_USE_CPP11
template<typename T>
FGSTD_FORCE_INLINE T&& forward(const T& x) FGSTD_NOEXCEPT
{
    return static_cast<remove_ref<T>::type>(x);
}

template<typename T>
FGSTD_FORCE_INLINE T&& forward(T&& x) FGSTD_NOEXCEPT
{
    return static_cast<remove_ref<T>::type>(x);
}

template<class T>
FGSTD_FORCE_INLINE T&& forward(typename remove_ref<T>::type& x) FGSTD_NOEXCEPT
{
    return (static_cast<T&&>(x));
}

template<class T>
FGSTD_FORCE_INLINE T&& forward(typename remove_ref<T>::type&& x) FGSTD_NOEXCEPT
{
    static_assert(!is_lvalue_reference<T>::value, "bad forward call");
    return (static_cast<T&&>(x));
}
#endif


template<typename T> FGSTD_FORCE_INLINE T vmin(T a, T b)
{
    return a < b ? a : b;
}

template<typename T> FGSTD_FORCE_INLINE T vmax(T a, T b)
{
    return a < b ? b : a;
}

template<unsigned N, unsigned M>
struct roundToNearestMultiple
{
    enum
    {
        _rem = N % M,
        value = (!_rem) ? N : (N + M - _rem)
    };
};

template<unsigned N>
struct roundToNearestMultiple<N, 0>
{
    enum { value = N };
};


template<unsigned M>
struct roundUp
{
    template<typename T>
    static FGSTD_FORCE_INLINE T round(T n) 
    {
        return ((n + (M - 1)) / M) * M;
    }
};

template<>
struct roundUp<1>
{
    template<typename T>
    static FGSTD_FORCE_INLINE T round(T n)
    {
       return n;
    }
};

template<>
struct roundUp<0>
{};


}
