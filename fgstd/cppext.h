#pragma once

#include "types.h"
#include "traits.h"

#define _FGSTD_CONCAT2(a, b) a##b
#define _FGSTD_CONCAT(a, b) _FGSTD_CONCAT2(a, b)

#define fgstd_alignof(T) (fgstd::Alignof<T>::value)
#define fgstd_static_assert(expr) do{switch(0){case expr: case 0:;}}while(0)
#define fgstd_static_assert_top(expr) static void _FGSTD_CONCAT(_fgstd_static_assert_, __LINE__)() { fgstd_static_assert(expr); }

#define fgstd_is_pod(x) (fgstd::is_pod<x>::value)

#ifndef FGSTD_USE_CPP11

#define alignof(T) fgstd_alignof(T)


#endif
