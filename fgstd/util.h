#pragma once

#include "platform.h"
#include "traits.h"

namespace fgstd {

#ifdef FGSTD_USE_CPP11
template <class T>
typename remove_ref<T>::type&& move (T&& x) noexcept
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

template<typename T>
FGSTD_FORCE_INLINE T *adressof(T& x)
{
    return (reinterpret_cast<T*>(
        &const_cast<char&>(
            reinterpret_cast<const volatile char&>(x))));
}

template<typename T>
FGSTD_FORCE_INLINE T&& forward(const T& x)
{
    return static_cast<remove_ref<T>::type>(x);
}

template<typename T>
FGSTD_FORCE_INLINE T&& forward(T&& x)
{
    return static_cast<remove_ref<T>::type>(x);
}

#ifdef FGSTD_USE_CPP11
template<class T>
FGSTD_FORCE_INLINE T&& forward(typename remove_ref<T>::type& x)
{
    return (static_cast<T&&>(x));
}

template<class T>
FGSTD_FORCE_INLINE T&& forward(typename remove_ref<T>::type&& x)
{
    static_assert(!is_lvalue_reference<T>::value, "bad forward call");
    return (static_cast<T&&>(x));
}
#endif



}

