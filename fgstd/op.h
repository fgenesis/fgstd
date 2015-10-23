#pragma once

#include "platform.h"

#include "op_def.h"

#include "op_scalar.h"
#include "op_sse.h"


namespace fgstd {
namespace op {

template<typename T>
struct Load
{
    typedef Traits<T> Traits;
    enum { BLOCK_SIZE = Traits::BLOCK_SIZE };
    typedef typename Traits::value_type value_type;
    typedef typename Traits::vec_type vec_type;
    FGSTD_FORCE_INLINE static vec_type applyVec(const value_type *ptr)
        { return Operators<T>::Vec::load(ptr); }
    FGSTD_FORCE_INLINE static const value_type& applyScalar(const value_type *ptr)
        { return Operators<T>::Scalar::load(ptr); }
};

template<typename T>
struct Load1
{
    typedef Traits<T> Traits;
    enum { BLOCK_SIZE = Traits::BLOCK_SIZE };
    typedef typename Traits::value_type value_type;
    typedef typename Traits::vec_type vec_type;
    FGSTD_FORCE_INLINE static vec_type applyVec(const value_type *ptr)
        { return Operators<T>::Vec::load1(ptr); }
    FGSTD_FORCE_INLINE static const value_type& applyScalar(const value_type *ptr)
        { return Operators<T>::Scalar::load1(ptr); }
};

template<typename T>
struct Get1
{
    typedef Traits<T> Traits;
    enum { BLOCK_SIZE = Traits::BLOCK_SIZE };
    typedef typename Traits::value_type value_type;
    typedef typename Traits::vec_type vec_type;
    FGSTD_FORCE_INLINE static value_type applyVec(vec_type v)
        { return Operators<T>::Vec::get1(v); }
    FGSTD_FORCE_INLINE static const value_type& applyScalar(const value_type& v)
        { return Operators<T>::Scalar::get1(v); }
};

template<typename T>
struct Store
{
    typedef Traits<T> Traits;
    enum { BLOCK_SIZE = Traits::BLOCK_SIZE };
    typedef typename Traits::value_type value_type;
    typedef typename Traits::vec_type vec_type;
    FGSTD_FORCE_INLINE static void applyVec(value_type *ptr, vec_type x)
        { Operators<T>::Vec::store(ptr, x); }
    FGSTD_FORCE_INLINE static void applyScalar(value_type *ptr, const value_type& x)
        { Operators<T>::Scalar::store(ptr, x); }
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
            FGSTD_FORCE_INLINE static vec_type applyVec(vec_type a, vec_type b) \
                { return Operators<T>::Vec::mth(a, b); } \
            FGSTD_FORCE_INLINE static value_type applyScalar(const value_type& a, const value_type& b) \
                { return Operators<T>::Scalar::mth(a, b); } \
        }; \
    };
#define _FGSTD_MAKE_FUNC(cls, mth) _FGSTD_MAKE_OPR(cls, mth, ?)
#include "_op_builder.h"


// TODO: casts


}}
