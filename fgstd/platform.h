#pragma once

#include <stdlib.h>

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

// FIXME: use global alloc instead
#ifndef FGSTD_STACK_ALLOC
# define FGSTD_STACK_ALLOC(x) malloc(x);
# define FGSTD_STACK_FREE(x) free(x);
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

#undef FGSTD_FORCE_INLINE
#define FGSTD_FORCE_INLINE

