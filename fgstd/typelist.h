#pragma once

#include "traits.h"

namespace fgstd {
namespace priv {

struct typelist_end
{
    enum { length = 0, packedsize = 0, maxalign = 0 };
};


template<typename A, typename B, int E, bool rec>
struct gap_end_expand;

template<typename A, typename B, int E>
struct gap_end_expand<A, B, E, false>
{
    enum { value = E-1 };
};

template<typename A, typename B, int E>
struct gap_end_expand<A, B, E, true>
{
    template<int End> 
    struct _Gap
    {
        A a;
        B b;
        char e[End];
    };
    template<>
    struct _Gap<0>
    {
        A a;
        B b;
    };

    typedef _Gap<E> Gap1;
    typedef _Gap<E+1> Gap2;
    enum
    {
        same = sizeof(Gap1) == sizeof(Gap2),
        value = gap_end_expand<A, B, E+1, same>::value
    };
};

template<typename A, typename B>
struct gap_end
{
    enum
    {
        value = gap_end_expand<A, B, 0, true>::value
    };
};


template<typename A, typename B, int N, bool rec>
struct gap_between_expand;

template<typename A, typename B, int N>
struct gap_between_expand<A, B, N, false>
{
    enum { value = N-1 };
};

template<typename A, typename B, int N>
struct gap_between_expand<A, B, N, true>
{
    template<int Mid> 
    struct _Gap
    {
        A a;
        char n[Mid];
        B b;
    };
    template<>
    struct _Gap<0>
    {
        A a;
        B b;
    };

    typedef _Gap<N> Gap1;
    typedef _Gap<N+1> Gap2;

    enum
    {
        same = sizeof(Gap1) == sizeof(Gap2),
        value = gap_between_expand<A, B,  N+1, same>::value
    };
};

template<typename A, typename B>
struct gap_between
{
    enum
    {
        value = gap_between_expand<A, B, 0, true>::value - gap_end<A, B>::value
    };
};


template<typename HEAD, typename TAIL = typelist_end>
class typelist
{
public:

    typedef HEAD Head;
    typedef TAIL Tail;

    template<unsigned N>
    struct get
    {
        typedef typename Tail::template get<N-1>::type type;
    };

    template<>
    struct get<0>
    {
        typedef Head type;
    };

    enum
    {
        length = 1 + Tail::length,
        packedsize = sizeof(Head) + Tail::packedsize,
        _headalign = Alignof<Head>::value,
        maxalign = _headalign > Tail::maxalign ? _headalign : Tail::maxalign
    };

    template<unsigned N>
    struct offset
    {
        typedef typename Tail::Head Next;
        typedef typename Tail::template offset<N-1> NextOffset;
        enum
        {
            value = sizeof(Head) + gap_between<Head, Next>::value + NextOffset::value
        };
    };

    template<>
    struct offset<0>
    {
        enum { value = 0 };
    };

    enum
    {
        bytes = roundToNearestMultiple<
            offset<length-1>::value + sizeof(get<length-1>),
            maxalign
        >::value,
        byteswasted = bytes - packedsize
    };


private:
    
    // to make sizeof(typelist<...>) work
    FGSTD_ALIGN(maxalign) char _sizer[bytes];

    // forbid instantiation
    typelist();


};

#ifdef XX_FGSTD_USE_CPP11
template<typename... Ts>
struct typelist_cons {};

template<>
struct typelist_cons<>
{
    typedef typelist_end type;
};

template<typename HEAD, typename... Ts>
struct typelist_cons<HEAD, Ts...>
{
    typedef typelist<HEAD,
        typename typelist_cons<Ts...>::type
    > type;
};

# define FGSTD_TYPELIST(...) fgstd::typelist_cons<__VA_ARGS__>::type

#else // -------------------
// No C++11 support

template <typename T>
struct typelist_cons;

template <typename T1>
struct typelist_cons<void (*)(T1)>
{
    typedef typelist<T1, typelist_end> type;
};

#define _CAT(x, y) x##y
#define T(x) P _CAT(T, x)

#define TL T(2)
#include "_typelist_builder.h"
#define TL T(2), T(3)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(14)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23), T(24)
#include "_typelist_builder.h"
// hopefully enough...

#undef T
#undef _CAT

#define FGSTD_TYPELIST(...) fgstd::typelist_cons<void(*)(__VA_ARGS__)>::type

// ------------
#endif

}

using priv::typelist;
using priv::typelist_cons;

template<typename A, typename B>
struct gap_between
{
    enum { value = priv::gap_between<A, B>::value };
};

template<typename A, typename B>
struct gap_end
{
    enum { value = priv::gap_end<A, B>::value };
};

}


