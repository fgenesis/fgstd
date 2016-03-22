
#ifndef _FGSTD_MAKE_OPR
#error _FGSTD_MAKE_OPR undefined
#endif

#ifndef _FGSTD_MAKE_FUNC2
#error _FGSTD_MAKE_FUNC undefined
#endif

#ifndef _FGSTD_MAKE_FUNC
#error _FGSTD_MAKE_UNOP undefined
#endif


_FGSTD_MAKE_OPR(Add, add, +)
_FGSTD_MAKE_OPR(Sub, sub, -)
_FGSTD_MAKE_OPR(Mul, mul, *)
_FGSTD_MAKE_OPR(Div, div, /)
_FGSTD_MAKE_OPR(Lsh, lsh, <<)
_FGSTD_MAKE_OPR(Rsh, rsh, >>)
_FGSTD_MAKE_OPR(BOr, bor, |)
_FGSTD_MAKE_OPR(BAnd, band, &)
_FGSTD_MAKE_OPR(BXor, bxor, ^)
_FGSTD_MAKE_FUNC2(Min, min)
_FGSTD_MAKE_FUNC2(Max, max)
_FGSTD_MAKE_FUNC(Sqrt, sqrt)
_FGSTD_MAKE_FUNC(Rsqrt, rsqrt)

#undef _FGSTD_MAKE_OPR
#undef _FGSTD_MAKE_FUNC
#undef _FGSTD_MAKE_FUNC2
