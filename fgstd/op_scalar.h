#pragma once

namespace fgstd {
namespace op {

template<typename T>
struct ScalarOperators
{
    typedef Traits<T> Traits;
    FGSTD_FORCE_INLINE static const T& load(const T *ptr)               { return *ptr; }
    FGSTD_FORCE_INLINE static const T& load1(const T *ptr)              { return *ptr; }
    FGSTD_FORCE_INLINE static const T& get1(const T& v)                 { return v; }
    FGSTD_FORCE_INLINE static void store(T *ptr, const T& x)            { *ptr = x; }
    FGSTD_FORCE_INLINE static T add(const T& a, const T& b)             { return a + b; }
    FGSTD_FORCE_INLINE static T sub(const T& a, const T& b)             { return a - b; }
    FGSTD_FORCE_INLINE static T mul(const T& a, const T& b)             { return a * b; }
    FGSTD_FORCE_INLINE static T div(const T& a, const T& b)             { return a / b; }

    FGSTD_FORCE_INLINE static const T& min(const T& a, const T& b)      { return a < b ? a : b; }
    FGSTD_FORCE_INLINE static const T& max(const T& a, const T& b)      { return a < b ? b : a; }

    FGSTD_FORCE_INLINE static T lsh(const T& a, const T& b)             { return a << b; }
    FGSTD_FORCE_INLINE static T rsh(const T& a, const T& b)             { return a >> b; }
    FGSTD_FORCE_INLINE static T bor(const T& a, const T& b)             { return a | b; }
    FGSTD_FORCE_INLINE static T band(const T& a, const T& b)            { return a & b; }
    FGSTD_FORCE_INLINE static T bxor(const T& a, const T& b)            { return a ^ b; }
};

template<typename T>
struct VecOperators;

template<typename T>
struct Operators
{
    typedef T value_type;
    typedef T vec_type;
    typedef Traits<T> Traits;
    enum { BLOCK_SIZE = 1 };
    typedef ScalarOperators<T> Scalar;
    typedef ScalarOperators<T> Vec;
};

}}

