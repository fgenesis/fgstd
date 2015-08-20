#pragma once

#include "types.h"
#include "traits.h"

#define _FGSTD_CONCAT2(a, b) a##b
#define _FGSTD_CONCAT(a, b) _FGSTD_CONCAT2(a, b)

#define fgstd_alignof(T) (fgstd::Alignof<T>::value)
#define fgstd_is_pod(x) (fgstd::is_pod<x>::value)

#ifndef FGSTD_USE_CPP11
# define alignof(T) fgstd_alignof(T)
#endif
