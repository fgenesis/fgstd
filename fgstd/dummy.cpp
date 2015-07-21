// To make CMake happy

#include <fgstd/vector_impl.h>
#include <fgstd/hashmap_impl.h>
#include <fgstd/cppext.h>

struct intpair
{
    int a, b;
};

static void test()
{
    fgstd_static_assert(fgstd_is_pod(int));
    fgstd_static_assert(fgstd_is_pod(int[5]));
    fgstd_static_assert(fgstd_is_pod(int*));
    fgstd_static_assert(fgstd_is_pod(void*));
    fgstd_static_assert(fgstd_is_pod(intpair*));
}

