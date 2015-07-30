// To make CMake happy

#include <fgstd/vector_impl.h>
#include <fgstd/hashmap_impl.h>
#include <fgstd/cppext.h>

using namespace fgstd::types;

struct intpair
{
    int a, b;
};
struct int2
{
    int x;
};
struct nonpod
{
    nonpod() : p(0) {}
    ~nonpod() { p = 0; }
    void *p;
    void *x;
    operator void*() { return p; }
    operator u32() { return 0; }
};

#define isscalar(T) fgstd::is_scalar<T>::value

static void test()
{
    fgstd_static_assert(fgstd_is_pod(int));
    fgstd_static_assert(fgstd_is_pod(int[5]));
    fgstd_static_assert(fgstd_is_pod(int*));
    fgstd_static_assert(fgstd_is_pod(void*));
    fgstd_static_assert(fgstd_is_pod(intpair*));
    fgstd_static_assert(fgstd_is_pod(int[5][4]));
    fgstd_static_assert(fgstd_is_pod(int**[5][4]));

    fgstd_static_assert(!fgstd_is_pod(nonpod));

    fgstd_static_assert(isscalar(u8));
    fgstd_static_assert(isscalar(int));
    fgstd_static_assert(isscalar(u64));
    fgstd_static_assert(isscalar(int*));
    fgstd_static_assert(!isscalar(nonpod));
    fgstd_static_assert(!isscalar(intpair));
    fgstd_static_assert(isscalar(double));
    fgstd_static_assert(!isscalar(int2));
    fgstd_static_assert(isscalar(ptrdiff_t));
}

