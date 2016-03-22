#pragma once

#include <intrin.h>

namespace fgstd {
namespace op {

template<>
struct Traits<float>
{
    typedef float value_type;
    typedef __m128 vec_type;
    enum { BLOCK_SIZE = 4 };
};

template<>
struct VecOperators<float>
{
private:
    typedef float value_type;
    typedef __m128 vec_type;
public:
    typedef Traits<float> Traits;
    FGSTD_FORCE_INLINE static vec_type load(const value_type *ptr)             { return _mm_load_ps(ptr); }
    FGSTD_FORCE_INLINE static vec_type load1(const value_type *ptr)            { return _mm_load1_ps(ptr); }
    FGSTD_FORCE_INLINE static value_type get1(vec_type v)                      { return _mm_cvtss_f32(v); }
    FGSTD_FORCE_INLINE static void store(value_type *ptr, vec_type x)          { _mm_store_ps(ptr, x); }
    FGSTD_FORCE_INLINE static vec_type add(vec_type a, vec_type b)             { return _mm_add_ps(a, b); }
    FGSTD_FORCE_INLINE static vec_type sub(vec_type a, vec_type b)             { return _mm_sub_ps(a, b); }
    FGSTD_FORCE_INLINE static vec_type mul(vec_type a, vec_type b)             { return _mm_mul_ps(a, b); }
    FGSTD_FORCE_INLINE static vec_type div(vec_type a, vec_type b)             { return _mm_div_ps(a, b); }
    FGSTD_FORCE_INLINE static vec_type min(vec_type a, vec_type b)             { return _mm_min_ps(a, b); }
    FGSTD_FORCE_INLINE static vec_type max(vec_type a, vec_type b)             { return _mm_max_ps(a, b); }

    FGSTD_FORCE_INLINE static vec_type sqrt(vec_type& a)                       { return _mm_sqrt_ps(a); }
    FGSTD_FORCE_INLINE static vec_type isqrt(vec_type a, vec_type b)           { return _mm_rsqrt_ps(a); }
};

template<>
struct Operators<float>
{
    typedef float value_type;
    typedef __m128 vec_type;
    typedef Traits<float> Traits;
    enum { BLOCK_SIZE = 4 };

    typedef ScalarOperators<float> Scalar;
    typedef VecOperators<float> Vec;
};

}}
