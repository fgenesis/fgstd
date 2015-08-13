#pragma once

#include "types.h"
#include "traits.h"

namespace fgstd {
namespace detail {

template<typename T> 
struct pow2builder
{
    template<u32 sh>
    struct shift
    {
        static const T tsh = sh;
        FGSTD_FORCE_INLINE static T next(T v)
        {
            v = shift<sh / 2>::next(v);
            v |= v >> tsh;
            return v;
        }
    };

    template<>
    struct shift<0u>
    {
        FGSTD_FORCE_INLINE static T next(T v)
        {
            return v;
        }
    };
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
    v = detail::pow2builder<T>::shift<T(sizeof(T) * 4)>::next(v);
    v++;
    v += !v;
    return v;
}


}
