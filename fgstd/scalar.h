#pragma once

#include "platform.h"

namespace stdfg {

template<typename T> FGSTD_FORCE_INLINE T vmin(T a, T b) { return a < b ? a : b; }
template<typename T> FGSTD_FORCE_INLINE T vmax(T a, T b) { return a < b ? b : a; }


}
