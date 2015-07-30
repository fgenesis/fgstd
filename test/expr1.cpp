#include <stdio.h>

#include <fgstd/expr.h>
#include <fgstd/containers.h>


#include <fgstd/vector_impl.h>
#include <fgstd/hashmap_impl.h>

#include <fgstd/bit.h>

#include <malloc.h>
#include <assert.h>

#include <fgstd/cppext.h>

//#include <vector>

using namespace fgstd::types;

class GlobalDefaultAlloc 
{
public:
    GlobalDefaultAlloc();
    virtual ~GlobalDefaultAlloc();
    virtual void *Alloc(size_t size);
    virtual void Free(void *p);
private:
    u32 c;
};
GlobalDefaultAlloc::GlobalDefaultAlloc()
: c(0)
{
}
GlobalDefaultAlloc::~GlobalDefaultAlloc()
{
    assert(!c);
}
void *GlobalDefaultAlloc::Alloc(size_t size)
{
    void *p = size ? malloc(size) : NULL;
    printf("malloc %u -> %p\n", (u32)size, p);
    c += !!p;
    return p;
}
void GlobalDefaultAlloc::Free(void *p)
{
    if (p)
    {
        printf("free %p - ", p);
        fflush(stdout);
        printf("size %u\n", (u32)_msize(p));
        fflush(stdout);
        --c;
        free(p);
    }
}


class Tester
{
public:
    Tester() : a(1, 42) { printf("Tester ctor\n"); }
    ~Tester() { printf("~Tester()\n"); }
    Tester(const Tester& o) : a(o.a) { printf("Tester copy ctor\n"); }
    Tester& operator=(const Tester& o)
    {
        if (this != &o)
        {
            a = o.a;
            printf("Tester copy assign\n");
        }
        return *this;
    }

#ifdef FGSTD_USE_CPP11
    Tester(const Tester&& o) : a(fgstd::forward(o.a)) { printf("Tester copy ctor\n"); }
    Tester& operator=(const Tester&& o) { a = FGSTD_MOVE(o.a);  printf("Tester move assign\n"); return *this; }
#endif

    fgstd::vector<int> a;
};

extern fgstd::IAllocator *fgstd::g_defaultAlloc;
fgstd::IAllocator *fgstd::g_defaultAlloc;

__declspec(align(32)) struct alntest
{
    u64 x;
    char a[5];
};

int main()
{
    fgstd::VAllocator<GlobalDefaultAlloc> defaultAlloc;
    fgstd::g_defaultAlloc = &defaultAlloc;
/*
    int a[] = {1,2,3,4,5};
    fgstd::vector<int> v(a);
    fgstd::vector<int> c = (v + v + 1) + (v * 100);

    for(u32 i = 0; i < c.size(); ++i)
        printf("[%u] = %u\n", i, c[i]);

    printf("%u -> %u\n", 24, fgstd::nextPowerOf2(24));
    printf("%u -> %u\n", 0, fgstd::nextPowerOf2(0));
    printf("%u -> %u\n", 1, fgstd::nextPowerOf2(1));
    printf("%u -> %u\n", 8, fgstd::nextPowerOf2(8));
*/

    //printf("size %u aln %u\n", sizeof(alntest), fgstd::Alignof<alntest>::value);

    /*{
        fgstd::vector<Tester> vt(1);
        puts("-----------------");
        vt.clear();
        puts("=====================");
    }*/

    //fgstd::vector<fgstd::vector<int> > vv(3);

    


    fgstd::hashmap<int, int> hi;

    hi[5] = 42;

    printf("has 4: %u\n", hi.has(4));
    printf("has 5: %u\n", hi.has(5));
    printf("[5]: %u\n", hi[5]);
    printf("[8]: %u\n", hi[8]);

    hi.remove(5);
    printf("has 5: %u\n", hi.has(5));
    hi[5] = 123;
    printf("has 5: %u\n", hi.has(5));
    printf("[5]: %u\n", hi[5]);


    return 0;
}

