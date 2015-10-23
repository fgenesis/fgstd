#include <stdio.h>

#include <fgstd/expr.h>
#include <fgstd/containers.h>


#include <fgstd/vector_impl.h>
#include <fgstd/hashmap_impl.h>
#include <fgstd/multivector_impl.h>

#include <fgstd/bit.h>

#include <malloc.h>
#include <assert.h>

#include <fgstd/cppext.h>

//#include <vector>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#define WIN32_NOMINMAX
#include <Windows.h>

class CHRTimer
{
protected:

    LARGE_INTEGER m_liStart;
    LARGE_INTEGER m_liStop;

public:

    CHRTimer(void)
    {
        m_liStart.QuadPart = m_liStop.QuadPart = 0;
    }

    ~CHRTimer(void)
    {
    }

    // Starts the timer
    void Start()
    {
        ::QueryPerformanceCounter(&m_liStart);
    }

    // Stops the timer
    void Stop()
    {
        ::QueryPerformanceCounter(&m_liStop);
    }

    // Returns the counter at the last Start()
    LONGLONG GetStartCounter()
    {
        return m_liStart.QuadPart;
    }

    // Returns the counter at the last Stop()
    LONGLONG GetStopCounter()
    {
        return m_liStop.QuadPart;
    }

    // Returns the interval between the last Start() and Stop()
    LONGLONG GetElapsed()
    {
        return (m_liStop.QuadPart - m_liStart.QuadPart);
    }

    // Returns the interval between the last Start() and Stop() in seconds
    double GetElapsedAsSeconds()
    {
        LARGE_INTEGER liFrequency;
        ::QueryPerformanceFrequency(&liFrequency);
        return ((double)GetElapsed() / (double)liFrequency.QuadPart);
    }
};

class Timer
{
public:
    Timer(const char *s) : str(s) { t.Start(); }
    ~Timer() {
        t.Stop();
        printf("[%s] Time: %f\n", str, t.GetElapsedAsSeconds());
    }
    const char *str;
    CHRTimer t;
};

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

struct FGSTD_ALIGN(32) alntest
{
    u64 x;
    char a[5];
};

extern void fgstd_test();


void setfl(float *p, u32 sz, float v)
{
    fgstd::et::expr_store(p, fgstd::expr(v));
}

void speed1(float *dst, float *a, float *b, u32 sz)
{
    fgstd::et::expr_store(dst, (fgstd::expr(a, sz) * fgstd::expr(b, sz) + 1.0f) * 5.0f);
}

void speed2(float *dst, float *a, float *b, u32 sz)
{
    for(u32 i = 0; i < sz; ++i)
        dst[i] = ((a[i] * b[i] + 1.0f) * 5.0f);
}


int main(int argc, char **argv)
{
    fgstd::VAllocator<GlobalDefaultAlloc> defaultAlloc;
    fgstd::g_defaultAlloc = &defaultAlloc;


     {
        float FGSTD_ALIGN(16) a[] = {1,2,3,4,5,6,7,8};
        fgstd::vector<float> v(a);
        fgstd::vector<float> c(fgstd_countof(a));
        c = (fgstd::expr(a) * 10.0f + 100.0f); // v + 100; //(v + v + 1) + (v * 100);
        for(u32 i = 0; i < c.size(); ++i)
            printf("[%u] = %f\n", i, c[i]);
    }




    puts("----");

    const u32 SZ = 1024*1012;
    float *a = new float[SZ];
    float *b = new float[SZ];
    float *dst = new float[SZ];

    setfl(a, SZ, 2+argc);
    setfl(a, SZ, 3+argc);
    {
        Timer tm("Vec");
        for(int i = 0; i < 10000+argc; ++i)
            speed1(dst, a, b, SZ);
    }

    setfl(a, SZ, 2+argc);
    setfl(a, SZ, 3+argc);
    {
        Timer tm("Normal");
        for(int i = 0; i < 10000+argc; ++i)
            speed2(dst, a, b, SZ);
    }

    return dst[0] * argc;

    //fgstd_test();

    puts("---");

    /*{
        fgstd::vector<fgstd::vector<u32> > vv(1);
        vv[0].resize(1);
    }*/


    //printf("size %u aln %u\n", sizeof(alntest), fgstd::Alignof<alntest>::value);

    /*{
        fgstd::vector<Tester> vt(1);
        puts("-----------------");
        vt.clear();
        puts("=====================");
    }*/

    //fgstd::vector<fgstd::vector<int> > vv(3);

    

    /*
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
    */

    /*
    typedef FGSTD_TYPELIST((u32, f32)) tl;
    typedef fgstd::multivector<tl> mvt;
    mvt mv;
    mv.resize(3);
    mvt::view<0, u32> vu = mv.getview<0>();
    mvt::view<1, f32> vf = mv.getview<1>();
    u32 *pu = mv.data<0>();
    float *pf = mv.data<1>();
    vu[0] = 0x12345678;
    vf[0] = 0.5f;
    vu[2] = 0x98765432;
    vf[2] = 3.141596f;
    */

    return 0;
}

