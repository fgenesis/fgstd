#pragma once

#include "platform.h"
#include "types.h"

namespace fgstd {


namespace priv {

template <typename T, T v>
struct IntegralConstant
{
    typedef T value_type;
    typedef IntegralConstant<T,v> type;
    enum { value = v };
};

typedef IntegralConstant<bool, true>  CompileTrue;
typedef IntegralConstant<bool, false> CompileFalse;
template<bool V> struct CompileCheck : IntegralConstant<bool, V>{};
}

template <bool B, typename T = void> struct enable_if { };
template <typename T>                struct enable_if<true, T> { typedef T type; };

template<typename T, typename U> struct is_same;
template <typename T, typename U> struct is_same      : priv::CompileFalse { };
template <typename T>             struct is_same<T,T> : priv::CompileTrue  { };

template<bool COND, typename A, typename B> struct TypeSwitch{};
template <typename A, typename B> struct TypeSwitch<true, A, B> { typedef A type; };
template <typename A, typename B> struct TypeSwitch<false, A, B> { typedef B type; };



namespace priv {

template <typename T> struct is_integral        : CompileFalse{};
template<> struct is_integral<bool>             : CompileTrue{};
template<> struct is_integral<char>             : CompileTrue{};
template<> struct is_integral<unsigned char>    : CompileTrue{};
template<> struct is_integral<signed char>      : CompileTrue{};
template<> struct is_integral<short>            : CompileTrue{};
template<> struct is_integral<unsigned short>   : CompileTrue{};
template<> struct is_integral<int>              : CompileTrue{};
template<> struct is_integral<unsigned int>     : CompileTrue{};
template<> struct is_integral<types::s64>       : CompileTrue{};
template<> struct is_integral<types::u64>       : CompileTrue{};

struct _dummyIntegralType1 {};
struct _dummyIntegralType2 {};

// The definition of long depends on OS
template<> struct is_integral<
    typename TypeSwitch<
        is_same<int, long>::value || is_same<types::s64, long>::value,
        _dummyIntegralType1,
        long
    >::type
> : CompileTrue{};

template<> struct is_integral<
    typename TypeSwitch<
        is_same<unsigned int, unsigned long>::value || is_same<types::u64, unsigned long>::value,
        _dummyIntegralType2,
        unsigned long
    >::type
> : CompileTrue{};



template <typename T> struct is_float           : CompileFalse {};
template<> struct is_float<float>               : CompileTrue{};
template<> struct is_float<double>              : CompileTrue{};
template<> struct is_float<long double>         : CompileTrue{};

template <typename T> struct is_array           : CompileFalse {};
template <typename T> struct is_array<T[]>      : CompileTrue {};
template <typename T, size_t N> struct is_array<T[N]> : CompileTrue{};

template <typename T> struct is_pointer         : CompileFalse {};
template <typename T> struct is_pointer<T*>     : CompileTrue {};

template<typename T> struct is_lvalue_reference : CompileFalse {};
template<typename T> struct is_lvalue_reference<T&> : CompileTrue{};
template<typename T> struct is_rvalue_reference : CompileFalse {};
template<typename T> struct is_reference        : CompileFalse {};
template<typename T> struct is_reference<T&>    : CompileTrue{};
#ifdef FGSTD_USE_CPP11
template<typename T> struct is_rvalue_reference<T&&> : CompileTrue{};
template<typename T> struct is_reference<T&&>   : CompileTrue{};
#endif

template <typename T>               struct is_member_pointer : CompileFalse { };
template <typename T, typename C>   struct is_member_pointer<T C::*> : CompileTrue{};

template<typename T> struct is_pod_basic : CompileCheck<
    is_integral<T>::value || is_pointer<T>::value || is_float<T>::value
> {};

template <typename T>
struct has_value_type
{
    typedef char yes[1];
    typedef char no[2];

    template <typename C> static yes& test(typename C::value_type*);
    template <typename> static no& test(...);

    enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};

template <typename T> struct is_scalar : CompileCheck<
    is_integral<T>::value || is_float<T>::value || is_pointer<T>::value || is_member_pointer<T>::value
> {};

template<bool B, typename T> struct get_value_type {};
template<typename T> struct get_value_type<true, T> { typedef typename T::value_type type; };
template<typename T> struct get_value_type<false, T> { typedef T type; };

template <typename T, size_t N>
char (&_ArraySizeHelper( T (&a)[N]))[N];


} // -----------------

template <typename T> struct remove_ref            { typedef T type; };
template <typename T> struct remove_ref<T&>        { typedef T type; };
#ifdef FGSTD_USE_CPP11
template <typename T> struct remove_ref<T&&>       { typedef T type; };
#endif

template <typename T> struct remove_pointer            { typedef T type; };
template <typename T> struct remove_pointer<T*>        { typedef typename remove_pointer<T>::type type; };

template <typename T> struct remove_array { typedef T type; };
template <typename T> struct remove_array<T[]> { typedef typename remove_array<T>::type type; };
template <typename T, size_t N> struct remove_array<T[N]> { typedef typename remove_array<T>::type type; };

template <typename T> struct remove_const                { typedef T  type; };
template <typename T> struct remove_const<const T&>      { typedef T& type; };
template <typename T> struct remove_volatile             { typedef T  type; };
template <typename T> struct remove_volatile<volatile T> { typedef T type; };

template <typename T> struct remove_cv;
template <typename T> struct remove_cv
{
    typedef
        typename remove_volatile <
            typename remove_const<T>::type
        >::type
        type;
};

template <typename T> struct is_integral : priv::is_integral<typename remove_cv<T>::type> {};
template <typename T> struct is_float : priv::is_float<typename remove_cv<T>::type> {};
template <typename T> struct is_array : priv::is_array<typename remove_cv<T>::type> {};
template <typename T> struct is_reference : priv::is_reference<typename remove_cv<T>::type> {};
template <typename T> struct is_scalar : priv::is_scalar<typename remove_cv<T>::type> {};

template <typename T> struct is_lvalue_reference : priv::is_lvalue_reference<typename remove_cv<T>::type> {};
template <typename T> struct is_rvalue_reference : priv::is_rvalue_reference<typename remove_cv<T>::type> {};

/*template <typename T>
struct get_value_type_ex
{
    typedef typename remove_cv<T>::type _B;
    enum { hasvt = priv::has_value_type<_B>::value };
    typedef typename remove_cv<
        typename TypeSwitch<
            hasvt,
            typename priv::get_value_type<hasvt, _B>::type,
            typename remove_pointer<_B>::type
        >::type
    >::type type;
};*/

template<typename T>
struct get_value_type
{
    enum { hasvt = priv::has_value_type<T>::value };
    typedef typename priv::get_value_type<hasvt, T>::type type;
};


template<bool need_more, typename T> struct _get_basic_type_rec {};
template<typename T> struct _get_basic_type_rec<false, T> { typedef T type; };
template<typename T> struct _get_basic_type_rec<true, T> {
    typedef
        typename remove_array<
            typename remove_ref<
                typename remove_cv<
                    T
                >::type
            >::type
        >::type
    X;

    typedef typename _get_basic_type_rec<
        is_array<X>::value || is_reference<X>::value,
        X
    >::type type;
};

template<typename T> struct get_basic_type : _get_basic_type_rec<true, T> {};


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
struct Alignof
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

template<typename T> struct is_pod : priv::CompileCheck<
#if defined(_MSC_VER) || defined(__GNUC__) || defined(__clang__)
    __is_pod(T) || // MSVC 2008-ish's version of this is broken, and does not report POD for trivial/integral types,
                   // -> need to add the custom stuff below as well.
#else
# pragma message("__is_pod() not provided, check this")
#endif
    // Alternative, very basic is_pod implementation that at least considers basic types and pointers to be POD.
    priv::is_pod_basic<
        typename get_basic_type<T>::type
    >::value
> {};


#define fgstd_countof(a) (sizeof(fgstd::priv::_ArraySizeHelper(a)))


}
