#pragma once

#include "platform.h"

namespace fgstd {
namespace op {

/*
#define _FGSTD_MAKE_BINOP(opr, cls) \
struct cls \
{ \
    template<typename T> \
    struct Op \
    { \
        typedef T value_type; \
        template<typename A, typename B> \
        static FGSTD_FORCE_INLINE T apply(const A& a, const B& b) \
        { \
            return a opr b; \
        } \
    }; \
};

_FGSTD_MAKE_BINOP(+, Add)
_FGSTD_MAKE_BINOP(-, Sub)
_FGSTD_MAKE_BINOP(*, Mul)
_FGSTD_MAKE_BINOP(/, Div)
_FGSTD_MAKE_BINOP(^, Xor)
_FGSTD_MAKE_BINOP(%, Mod)
_FGSTD_MAKE_BINOP(&, BitAnd)
_FGSTD_MAKE_BINOP(|, BitOr)
_FGSTD_MAKE_BINOP(<<, Lsh)
_FGSTD_MAKE_BINOP(>>, Rsh)

#undef _FGSTD_MAKE_BINOP
*/

template<typename T>
struct Traits
{
    typedef T value_type;
    typedef T vec_type;
    enum { BLOCK_SIZE = 1 };
};

template<typename T>
struct Operators
{
    typedef T value_type;
    typedef T vec_type;
    typedef Traits<T> Traits;
    enum { BLOCK_SIZE = 1 };

    FGSTD_FORCE_INLINE static vec_type load(const value_type *ptr)             { return *ptr; }
    FGSTD_FORCE_INLINE static vec_type load1(const value_type& v)              { return v; }
    FGSTD_FORCE_INLINE static vec_type store(value_type *ptr, const vec_type x){ return (*ptr = x); }
    FGSTD_FORCE_INLINE static vec_type add(vec_type a, vec_type b)             { return a + b; }
    FGSTD_FORCE_INLINE static vec_type sub(vec_type a, vec_type b)             { return a - b; }
    FGSTD_FORCE_INLINE static vec_type mul(vec_type a, vec_type b)             { return a * b; }
    FGSTD_FORCE_INLINE static vec_type div(vec_type a, vec_type b)             { return a / b; }
    FGSTD_FORCE_INLINE static vec_type lsh(vec_type a, vec_type b)             { return a << b; }
    FGSTD_FORCE_INLINE static vec_type rsh(vec_type a, vec_type b)             { return a >> b; }
    FGSTD_FORCE_INLINE static vec_type bor(vec_type a, vec_type b)             { return a | b; }
    FGSTD_FORCE_INLINE static vec_type band(vec_type a, vec_type b)            { return a & b; }
    FGSTD_FORCE_INLINE static vec_type bxor(vec_type a, vec_type b)            { return a ^ b; }
    FGSTD_FORCE_INLINE static vec_type min(vec_type a, vec_type b)             { return fgstd::vmin(a, b); }
    FGSTD_FORCE_INLINE static vec_type max(vec_type a, vec_type b)             { return fgstd::vmax(a, b); }
};

template<typename T>
struct Load
{
    typedef Traits<T> Traits;
    enum { BLOCK_SIZE = Traits::BLOCK_SIZE };
    typedef typename Traits::value_type value_type;
    typedef typename Traits::vec_type vec_type;
    FGSTD_FORCE_INLINE static vec_type apply(const value_type *ptr)
        { return Operators<T>::load(a); }
};

template<typename T>
struct Load1
{
    typedef Traits<T> Traits;
    enum { BLOCK_SIZE = Traits::BLOCK_SIZE };
    typedef typename Traits::value_type value_type;
    typedef typename Traits::vec_type vec_type;
    FGSTD_FORCE_INLINE static vec_type apply(const value_type& v)
        { return Operators<T>::load1(v); }
};

template<typename T>
struct Store
{
    typedef Traits<T> Traits;
    enum { BLOCK_SIZE = Traits::BLOCK_SIZE };
    typedef typename Traits::value_type value_type;
    typedef typename Traits::vec_type vec_type;
    FGSTD_FORCE_INLINE static vec_type apply(const value_type *ptr, vec_type x)
        { return Operators<T>::store(a); }
};

#define _FGSTD_MAKE_OPR(cls, mth, _) \
    struct cls \
    { \
        template<typename T> struct Op \
        { \
            typedef Traits<T> Traits; \
            enum { BLOCK_SIZE = Traits::BLOCK_SIZE }; \
            typedef typename Traits::value_type value_type; \
            typedef typename Traits::vec_type vec_type; \
            FGSTD_FORCE_INLINE static vec_type apply(vec_type a, vec_type b) \
                { return Operators<T>::mth(a, b); } \
        }; \
    };
#define _FGSTD_MAKE_FUNC(cls, mth) _FGSTD_MAKE_OPR(cls, mth, ?)
#include "_op_builder.h"


// TODO: casts


}}
