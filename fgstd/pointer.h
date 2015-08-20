#pragma once

#include "util.h" // for roundUp()

namespace fgstd {

/*template<unsigned A>
FGSTD_FORCE_INLINE char *alignPointer(void *ptr)
{
    fgstd_static_assert(sizeof(intptr_t) == sizeof(void*));

    intptr_t p = static_cast<intptr_t>(ptr);
    return static_cast<void*>(roundUp<A>(p));
}

template<unsigned A>
FGSTD_FORCE_INLINE bool pointerIsAligned(void *ptr)
{
    return !(static_cast<intptr_t>(ptr) % A);
}*/




}
