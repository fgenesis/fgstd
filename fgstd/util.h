#pragma once

#include "platform.h"

namespace fgstd {

#ifdef FGSTD_USE_CPP11
template <class T>
typename remove_reference<T>::type&& move (T&& arg) noexcept
{
    return static_cast<remove_reference<decltype(x)>::type&&>(x);
}
#define FGSTD_MOVE(x) fgstd::move(x)
#else
#define FGSTD_MOVE(x) x
#endif


template<typename T>
void swap(T& a, T& b)
{
    T c(FGSTD_MOVE(a));
    a = FGSTD_MOVE(b);
    b = FGSTD_MOVE(c);
}




}

