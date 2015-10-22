#pragma once

#include "types.h"
#include "traits.h"

namespace fgstd {
namespace detail {

template<typename T, u32 sh>
struct shifthelper
{
    static const T tsh = sh;
    FGSTD_FORCE_INLINE static T next(T v)
    {
        v = shifthelper<T, sh / 2>::next(v);
        v |= v >> tsh;
        return v;
    }
};

template<typename T>
struct shifthelper<T, 0u>
{
    FGSTD_FORCE_INLINE static T next(T v)
    {
        return v;
    }
};

template<typename T> 
struct pow2builder
{
    FGSTD_FORCE_INLINE static T next(T v)
    {
        return shifthelper<T, sizeof(T) * 4>::next(v);
    }
};


} // end namespace detail

template <typename T>
typename enable_if<
    is_integral<T>::value,
    T
>::type
FGSTD_FORCE_INLINE nextPowerOf2(T v)
{
    v--;
    v = detail::pow2builder<T>::next(v);
    v++;
    v += !v;
    return v;
}


}
