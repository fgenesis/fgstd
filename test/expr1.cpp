#include <stdio.h>

#include <fgstd/expr.h>
#include <fgstd/containers.h>


#include <fgstd/vector_impl.h>

using namespace fgstd::types;

class GlobalDefaultAlloc 
{
public:
    virtual ~GlobalDefaultAlloc() {}
    virtual void *Alloc(size_t size);
    virtual void Free(void *p);
};
void *GlobalDefaultAlloc::Alloc(size_t size)
{
    printf("malloc %u\n", (u32)size);
    return malloc(size);
}
void GlobalDefaultAlloc::Free(void *p)
{
    free(p);
}

extern fgstd::IAllocator *fgstd::g_defaultAlloc;
fgstd::IAllocator *fgstd::g_defaultAlloc;

int main()
{
    fgstd::VAllocator<GlobalDefaultAlloc> defaultAlloc;
    fgstd::g_defaultAlloc = &defaultAlloc;

    int a[] = {1,2,3,4,5};
    fgstd::vector<int> v(a);
    fgstd::vector<int> c = (v + v + 1) + (v * 100);

    for(int i = 0; i < c.size(); ++i)
        printf("[%u] = %u\n", i, c[i]);

    return 0;
}

