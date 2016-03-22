#include <stdio.h>
#include <assert.h>

#include <unordered_map>
#include <map>

#include <fgstd/hashmap_impl.h>
#include <fgstd/cppext.h>

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
/*#ifndef NDEBUG
    printf("malloc %u -> %p\n", (u32)size, p);
    c += !!p;
#endif*/
    return p;
}
void GlobalDefaultAlloc::Free(void *p)
{
    if (p)
    {
/*#ifndef NDEBUG
        printf("free %p - ", p);
        fflush(stdout);
        printf("size %u\n", (u32)_msize(p));
        fflush(stdout);
        --c;
#endif*/
        free(p);
    }
}

extern fgstd::IAllocator *fgstd::g_defaultAlloc;
fgstd::IAllocator *fgstd::g_defaultAlloc;

struct strequal
{
    FGSTD_FORCE_INLINE bool operator()(const char *a, const char *b) const
    {
        return !strcmp(a, b);
    }
};

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

int main()
{
    fgstd::VAllocator<GlobalDefaultAlloc> defaultAlloc;
    fgstd::g_defaultAlloc = &defaultAlloc;

    const u32 ITER = 10000;
    const u32 LOOPS = 500;

    printf("sizeof(fgstd::hashmap) = %u\n", (u32)sizeof(fgstd::hashmap<int, int>));
    printf("sizeof(std::unordered_map) = %u\n", (u32)sizeof(std::unordered_map<int, int>));
    printf("sizeof(std::map) = %u\n", (u32)sizeof(std::map<int, int>));

    for(u32 k = 0; k < 2; ++k)
    {
        Timer tm("fgstd::hashmap");
        fgstd::hashmap<u32, u32> m;

        for (u32 loop = 0; loop < LOOPS; ++loop)
        {
            for (u32 i = 0; i < ITER; ++i)
                m[i * 2] = i * 123;
            
            for (u32 i = 0; i < ITER*2; ++i)
            {
                u32 *p = m.getp(i);
                if (p)
                {
                    assert(!(i & 1));
                    volatile u32 x = *p;
                    (void)x;
                    assert(x == (i / 2) * 123);
                }
                else
                    assert(i & 1);

                m.remove(i);
            }

            assert(m.empty());
        }
    }

    {
        Timer tm("std::map");
        typedef std::map<u32, u32> themap;
        themap m;

        for (u32 loop = 0; loop < LOOPS; ++loop)
        {
            for (u32 i = 0; i < ITER; ++i)
                m[i * 2] = i * 123;

            for (u32 i = 0; i < ITER*2; ++i)
            {
                themap::const_iterator it = m.find(i);
                if (it != m.end())
                {
                    assert(!(i & 1));
                    volatile u32 x = it->second;
                    (void)x;
                    assert(x == (i / 2) * 123);
                }
                else
                    assert(i & 1);

                m.erase(i);
            }

            assert(m.empty());
        }
    }

    {
        Timer tm("std::unordered_map");
        typedef std::unordered_map<u32, u32> themap;
        themap m;

        for (u32 loop = 0; loop < LOOPS; ++loop)
        {
            for (u32 i = 0; i < ITER; ++i)
                m[i * 2] = i * 123;

            for (u32 i = 0; i < ITER*2; ++i)
            {
                themap::const_iterator it = m.find(i);
                if (it != m.end())
                {
                    assert(!(i & 1));
                    volatile u32 x = it->second;
                    assert(x == (i / 2) * 123);
                }
                else
                    assert(i & 1);

                m.erase(i);
            }

            assert(m.empty());
        }
    }


    return 0;
}

