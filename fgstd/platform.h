#pragma once

#include <stdlib.h>

#define _FGSTD_CONCAT2(a, b) a##b
#define _FGSTD_CONCAT(a, b) _FGSTD_CONCAT2(a, b)


#if defined(_MSC_VER)
# define FGSTD_ALIGN(N) __declspec(align(N))
# include <malloc.h>
# define FGSTD_STACK_ALLOC(x) alloca(x)
# define FGSTD_STACK_FREE(x) /* nop */
#  if (_MSC_VER >= 1200)
#    define FGSTD_FORCE_INLINE __forceinline
#  else
#    define FGSTD_FORCE_INLINE __inline
#  endif
#elif defined(__GNUC__) || defined(__clang__)
# define FGSTD_ALIGN(N)  __attribute__ ((aligned(N)))
# define FGSTD_STACK_ALLOC(x) alloca(x)
# define FGSTD_STACK_FREE(x) /* nop */
#  ifdef NDEBUG
#    define FGSTD_FORCE_INLINE inline __attribute__((always_inline))
#  endif
#endif

#ifndef FGSTD_FORCE_INLINE
#  define FGSTD_FORCE_INLINE inline
#endif

#ifndef FGSTD_STACK_ALLOC
# error FGSTD_STACK_ALLOC: dont have alloca()
#endif

// from SDL headers
#if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define FGSTD_IS_BIG_ENDIAN 1
#endif

#ifndef FGSTD_IS_BIG_ENDIAN
#  define FGSTD_IS_BIG_ENDIAN 0
#endif

#ifndef FGSTD_USE_CPP11
#  if __cplusplus > 199711L || (defined(_MSC_VER) && _MSC_VER >= 1600)
#    define FGSTD_USE_CPP11 1
#  endif
#endif

#if defined(_MSC_VER)
# define FGSTD_NOEXCEPT __declspec(nothrow)
#elif defined(__GNUC__) || defined(__clang__)
# define FGSTD_NOEXCEPT __attribute__((nothrow))
#else
# define FGSTD_NOEXCEPT
#endif

#ifdef FGSTD_USE_CPP11
# define fgstd_static_assert(expr) static_assert(expr, #expr)
# define fgstd_static_assert_top(expr) static_assert(expr, #expr)
#else
# define fgstd_static_assert(expr) do{switch(0){case expr: case 0:;}}while(0)
# define fgstd_static_assert_top(expr) static void _FGSTD_CONCAT(_fgstd_static_assert_, __LINE__)() { fgstd_static_assert(expr); }
#endif


#if defined(_DEBUG) || !defined(NDEBUG)
# define FGSTD_INTERNAL_DEBUG
#endif

#ifdef FGSTD_INTERNAL_DEBUG
#  include <assert.h>
#  define FGSTD_INTERNAL_ASSERT(x) assert(x)
#else
#  define FGSTD_INTERNAL_ASSERT(x)
#endif

#undef FGSTD_FORCE_INLINE
#define FGSTD_FORCE_INLINE

