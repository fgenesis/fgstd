#pragma once

#include "platform.h"

namespace fgstd {
namespace op {

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



}}

