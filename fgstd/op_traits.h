#pragma once

namespace fgstd {
namespace op {
namespace detail {

/*
#define _FGSTD_MAKE_OPR(_, mth, __) \
template<typename A> \
struct has_ ## mth ## _1 \
{ \
    typedef char yes[1]; \
    typedef char no[2]; \
    typedef typename A::Traits::vec_type VT; \
    template<typename A, VT (*)(VT, VT)> struct SFINAE {}; \
    template<typename A> static yes& Test(SFINAE<A, &A::mth>*); \
    template<typename A> static no& Test(...); \
    enum { value = sizeof(Test<A>(0)) == sizeof(yes) }; \
}; \
template<typename A> \
struct has_ ## mth ## _2 \
{ \
    typedef char yes[1]; \
    typedef char no[2]; \
    typedef typename A::Traits::vec_type VT; \
    template<typename A, const VT& (*)(const VT&, const VT&)> struct SFINAE {}; \
    template<typename A> static yes& Test(SFINAE<A, &A::mth>*); \
    template<typename A> static no& Test(...); \
    enum { value = sizeof(Test<A>(0)) == sizeof(yes) }; \
}; \
template<typename A> \
struct has_ ## mth ## _3 \
{ \
    typedef char yes[1]; \
    typedef char no[2]; \
    typedef typename A::Traits::vec_type VT; \
    template<typename A, VT (*)(const VT&, const VT&)> struct SFINAE {}; \
    template<typename A> static yes& Test(SFINAE<A, &A::mth>*); \
    template<typename A> static no& Test(...); \
    enum { value = sizeof(Test<A>(0)) == sizeof(yes) }; \
}; \
template<typename A> \
struct has_ ## mth { enum { value = has_ ## mth ## _1<A>::value || has_ ## mth ## _2<A>::value || has_ ## mth ## _3<A>::value };  typedef typename A::Traits::vec_type VT; };
*/

/*#define _FGSTD_MAKE_OPR(_, mth, __) \
template<typename A> \
struct has_ ## mth { enum { value = 1 }; typedef typename A::Traits::vec_type VT; };*/

#define _FGSTD_MAKE_OPR(_, mth, __) \
template<typename A> \
struct has_ ## mth \
{ \
    typedef char yes[1]; \
    typedef char no[2]; \
    typedef typename A::Traits::vec_type VT; \
    template<typename A, void*> struct SFINAE {}; \
    template<typename A> static yes& Test(SFINAE<A, &A::mth>*); \
    template<typename A> static no& Test(...); \
    enum { value = sizeof(Test<A>(0)) == sizeof(yes) }; \
}; \

#define _FGSTD_MAKE_FUNC(_, mth) _FGSTD_MAKE_OPR(?, mth, ?)
#include "_op_builder.h"




}}}
