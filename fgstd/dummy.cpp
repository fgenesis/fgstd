// To make CMake happy

#include <fgstd/vector_impl.h>
#include <fgstd/hashmap_impl.h>
#include <fgstd/typelist.h>
#include <fgstd/cppext.h>

#include <stdio.h>
#include <iostream>

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

void fgstd_test()
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

    fgstd_static_assert((fgstd::roundToNearestMultiple<0, 8>::value == 0));
    fgstd_static_assert((fgstd::roundToNearestMultiple<1, 8>::value == 8));
    fgstd_static_assert((fgstd::roundToNearestMultiple<8, 8>::value == 8));
    fgstd_static_assert((fgstd::roundToNearestMultiple<0, 3>::value == 0));
    fgstd_static_assert((fgstd::roundToNearestMultiple<2, 3>::value == 3));
    fgstd_static_assert((fgstd::roundToNearestMultiple<3, 3>::value == 3));


    fgstd_static_assert((fgstd::gap_end<u64, u32>::value == 4));
    fgstd_static_assert((fgstd::gap_between<u64, u32>::value == 0));

    fgstd_static_assert((fgstd::gap_end<u32, u64>::value == 0));
    fgstd_static_assert((fgstd::gap_between<u32, u64>::value == 4));


    {
        typedef FGSTD_TYPELIST(u64, u32, u16, u8) tl;

        fgstd_static_assert(tl::offset<0>::value == 0);
        fgstd_static_assert(tl::offset<1>::value == 8);
        fgstd_static_assert(tl::offset<2>::value == 12);
        fgstd_static_assert(tl::offset<3>::value == 14);
        fgstd_static_assert((fgstd::is_same<tl::get<2>::type, u16>::value));
        fgstd_static_assert(tl::packedsize == 15);
        fgstd_static_assert(sizeof(tl) == 16); // due to alignment, it's 16
    }

    {
        typedef FGSTD_TYPELIST(u8, u8, u8, u8, u8) tl;

        fgstd_static_assert(tl::packedsize == 5);
        fgstd_static_assert(sizeof(tl) == 5);
    }

    {
        typedef FGSTD_TYPELIST(u8, u16, u32) tl;
        struct aa { u8 a; u16 b; u32 c; };

        fgstd_static_assert(sizeof(aa) == 8);
        fgstd_static_assert(tl::packedsize == 1+2+4);
        fgstd_static_assert(sizeof(tl) == 8);
    }

    {
        typedef FGSTD_TYPELIST(u8, u32, u16, u64) tl;

        fgstd_static_assert(tl::offset<0>::value == 0);
        fgstd_static_assert(tl::offset<1>::value == 4);
        fgstd_static_assert(tl::offset<2>::value == 8);
        fgstd_static_assert(tl::offset<3>::value == 16);

        fgstd_static_assert((fgstd::is_same<tl::get<2>::type, u16>::value));
        fgstd_static_assert(tl::packedsize == 15);
        fgstd_static_assert(sizeof(tl) == 24);
    }

}

