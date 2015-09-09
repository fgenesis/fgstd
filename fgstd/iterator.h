#pragma once

#include "traits.h"

namespace fgstd {


template<typename IT>
FGSTD_FORCE_INLINE void iter_swap(IT a, IT b)
{
    fgstd::iter_swap(*a, *b);
}



}
