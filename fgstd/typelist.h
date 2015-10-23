#pragma once

#include "traits.h"
#include "pointer.h"

namespace fgstd {
namespace priv {

struct typelist_end
{
    enum { length = 0, packedsize = 0, maxalign = 0, _head_align = 1 };
    typedef void maxalign_type;
    typedef void Head;
    static size_t array_req_bytes(size_t) { return 0; }
};


template<typename A, typename B, int E, bool rec>
struct gap_end_expand;

template<typename A, typename B, int E>
struct gap_end_expand<A, B, E, false>
{
    enum { value = E-1 };
};

template<typename A, typename B, int End> 
struct _GapEnd
{
    A a;
    B b;
    char e[End];
};
template<typename A, typename B>
struct _GapEnd<A, B, 0>
{
    A a;
    B b;
};

template<typename A, typename B, int E>
struct gap_end_expand<A, B, E, true>
{
    typedef _GapEnd<A, B, E> Gap1;
    typedef _GapEnd<A, B, E+1> Gap2;
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

template<typename A, typename B, int Mid> 
struct _GapBetween
{
    A a;
    char n[Mid];
    B b;
};
template<typename A, typename B>
struct _GapBetween<A, B, 0>
{
    A a;
    B b;
};

template<typename A, typename B, int N>
struct gap_between_expand<A, B, N, true>
{
    typedef _GapBetween<A, B, N> Gap1;
    typedef _GapBetween<A, B, N+1> Gap2;

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

template<int N, typename HEAD, typename TAIL>
struct total_gap_between2;

template<int N, typename TL>
struct total_gap_between
{
    enum { value = total_gap_between2<N, typename TL::Head, typename TL::Tail>::value };
};

template<int N, typename HEAD, typename TAIL>
struct total_gap_between2
{
    enum { value = gap_between<HEAD, typename TAIL::Head>::value + total_gap_between<N-1, TAIL>::value };
};

template<int N, typename HEAD>
struct total_gap_between2<N, HEAD, typelist_end>
{
    enum { value = 0 };
};

template<typename HEAD, typename TAIL>
struct total_gap_between2<0, HEAD, TAIL>
{
    enum { value = 0 };
};

template<unsigned N, typename HEAD, typename TAIL>
struct _array_offs_bytes_helper;

template<unsigned N, typename TL>
struct _array_offs_bytes_helper1
{
    static FGSTD_FORCE_INLINE size_t calc(size_t n)
    {
        return _array_offs_bytes_helper<N, typename TL::Head, typename TL::Tail>::calc(n);
    }
    static FGSTD_FORCE_INLINE void writeOffs(size_t n, u32 *out, u32 start)
    {
        _array_offs_bytes_helper<N, typename TL::Head, typename TL::Tail>::writeOffs(n, out, start);
    }
};

template<unsigned N>
struct _array_offs_bytes_helper1<N, typelist_end>
{
    static FGSTD_FORCE_INLINE size_t calc(size_t)
    {
        return 0;
    }
    static FGSTD_FORCE_INLINE void writeOffs(size_t, u32*, u32)
    {
    }
};

template<unsigned N, typename HEAD, typename TAIL>
struct _array_offs_bytes_helper
{
    static FGSTD_FORCE_INLINE size_t calc(size_t n)
    {
        return roundUp<TAIL::_head_align>::round(n * sizeof(HEAD))
            + _array_offs_bytes_helper1<N-1, TAIL>::calc(n);
    }
    static FGSTD_FORCE_INLINE void writeOffs(size_t n, u32 *out, u32 start)
    {
        const u32 next = (u32)roundUp<TAIL::_head_align>::round(start + (n * sizeof(HEAD)));
        *out = start;
        _array_offs_bytes_helper1<N-1, TAIL>::writeOffs(n, out + 1, next);
    }
};


template<typename HEAD, typename TAIL>
struct _array_offs_bytes_helper<0, HEAD, TAIL>
{
    static FGSTD_FORCE_INLINE size_t calc(size_t)
    {
        return 0;
    }

    static FGSTD_FORCE_INLINE void writeOffs(size_t, u32 *, u32)
    {
    }
};

// pre-decl
template<typename HEAD, typename TAIL>
class typelist;


template<typename HEAD, typename TAIL, unsigned N>
struct _typelist_get
{
    typedef typename _typelist_get<typename TAIL::Head, typename TAIL::Tail, N-1>::type type;
};

template<typename HEAD, typename TAIL>
struct _typelist_get<HEAD, TAIL, 0u>
{
    typedef HEAD type;
};

/*
template<typename HEAD, typename TAIL, unsigned N>
struct _typelist_first_as_list
{
    typedef typelist<HEAD, typename _typelist_first_as_list<TAIL::Head, TAIL::Tail, N-1>::type> type;
};

template<typename HEAD, typename TAIL>
struct _typelist_first_as_list<HEAD, TAIL, 0u>
{
    typedef typelist_end type;
};
*/

template<typename HEAD, typename TAIL, unsigned N>
struct _typelist_offset
{
    typedef typename TAIL::Head Next;
    typedef _typelist_offset<Next, typename TAIL::Tail, N-1> NextOffset;
    enum
    {
        value = sizeof(HEAD) + gap_between<HEAD, Next>::value + NextOffset::value
    };
};

template<typename HEAD, typename TAIL>
struct _typelist_offset<HEAD, TAIL, 0u>
{
    enum { value = 0 };
};


template<typename HEAD, typename TAIL = typelist_end>
class typelist
{
public:
    typedef HEAD Head;
    typedef TAIL Tail;

    template<unsigned N>
    struct offset
    {
        enum { value = _typelist_offset<HEAD, TAIL, N>::value };
    };

    template<unsigned N>
    struct get
    {
        typedef typename  _typelist_get<HEAD, TAIL, N>::type type;
    };


    enum
    {
        length = 1 + Tail::length,
        packedsize = sizeof(Head) + Tail::packedsize,
        _headalign = Alignof<Head>::value,
        maxalign = _headalign > Tail::maxalign ? _headalign : Tail::maxalign
    };

    typedef typename TypeSwitch<
        (maxalign == _headalign),
        Head,
        typename Tail::maxalign_type
    >::type maxalign_type;


    enum
    {
        bytes = roundToNearestMultiple<
            offset<length-1>::value + sizeof(get<length-1>),
            maxalign
        >::value,
        byteswasted = bytes - packedsize,
        _fillsize = bytes > maxalign ? bytes : maxalign,
        total_gap_bytes = total_gap_between2<length, Head, Tail>::value,
        _head_align = Alignof<Head>::value,
    };

    // bytes required to pack n elements into a contiguous array,
    // respecting sizeof() and alignment.
    // e.g. for <A, B, C>, n=4: AAAABBBBCCCC
    static FGSTD_FORCE_INLINE size_t array_req_bytes(size_t n)
    {
        return roundUp<Tail::_head_align>::round(n * sizeof(Head))
            + Tail::array_req_bytes(n);
    }

    enum { array_offsets_bytes = roundToNearestMultiple<length * sizeof(u32), _head_align>::value };

    // offset in bytes of a pointer to the start of type N in a contiguous array of n elements per type,
    // respecting alignment and etc
    template<unsigned N>
    static FGSTD_FORCE_INLINE size_t array_offs(u32 n)
    {
        return _array_offs_bytes_helper<N, Head, Tail>::calc(n);
    }

    static FGSTD_FORCE_INLINE void array_offs_write(u32 n, u32 *out, u32 start = 0)
    {
        _array_offs_bytes_helper<length, Head, Tail>::writeOffs(n, out, start);
    }



private:

    
    // to make sizeof(typelist<...>) and alignof() work
    union Filler
    {
        maxalign_type _aligner;
        char _sizer[_fillsize];
    };
    Filler filler;

    // forbid instantiation
    //typelist();


};

#ifdef FGSTD_USE_CPP11
template<typename... Ts>
struct typelist_cons_v {};

template<>
struct typelist_cons_v<>
{
    typedef typelist_end type;
};

template<typename HEAD, typename... Ts>
struct typelist_cons_v<HEAD, Ts...>
{
    typedef typelist<HEAD,
        typename typelist_cons_v<Ts...>::type
    > type;
};

template<typename... Ts>
struct typelist_cons {};

template<typename... Ts>
struct typelist_cons<void (*) (Ts...)>
{
    typedef typename typelist_cons_v<Ts...>::type type;
};


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
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23), T(24), T(25)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23), T(24), T(25), T(26)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23), T(24), T(25), T(26), T(27)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23), T(24), T(25), T(26), T(27), T(28)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23), T(24), T(25), T(26), T(27), T(28), T(29)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23), T(24), T(25), T(26), T(27), T(28), T(29), T(30)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23), T(24), T(25), T(26), T(27), T(28), T(29), T(30), T(31)
#include "_typelist_builder.h"
#define TL T(2), T(3), T(4), T(5), T(6), T(7), T(8), T(9), T(10), T(11), T(12), T(13), T(15), T(16), T(17), T(18), T(19), T(20), T(21), T(22), T(23), T(24), T(25), T(26), T(27), T(28), T(29), T(30), T(31), T(32)
#include "_typelist_builder.h"
// hopefully enough...

#undef T
#undef _CAT

// ------------
#endif

#define FGSTD_TYPELIST(x) fgstd::typelist_cons<void(*) x>::type

} // end namespace priv


using priv::typelist;
using priv::typelist_cons;
using priv::typelist_end;

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


