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

    u32 a[] = {1,2,3,4,5};
    fgstd::vector<u32> c;
    fgstd::vector<u32> v(a);

    /*
    printf("v sz: %u\n", v.size());
    printf("c sz: %u\n", c.size());
    puts("----");

    fgstd::swap(v, c);

    printf("v sz: %u\n", v.size());
    printf("c sz: %u\n", c.size());
    */

    //c = v + v;

    /*typedef fgstd::vector<u32>::Expr exprtype;
    typedef fgstd::op::Add<u32> optype;

    typedef exprtype::value_type vt1;
    typedef optype::value_type vt2;

    typedef fgstd::et::BinOpExpr<optype, exprtype, exprtype>::value_type testtype;


    exprtype x = fgstd::expr(v);
    fgstd::et::ScalarExpr<u32> n(1);*/

    //fgstd::et::BinOpExpr<optype, exprtype, exprtype> y = fgstd::et::BinOpExpr<optype, exprtype, exprtype>(x, x);
    //fgstd::et::BinOpExpr<optype, exprtype, exprtype> y = x+x;

   // fgstd::et::makeexpr<fgstd::vector<u32> >::type x = fgstd::expr(v);
    //c = x;
    //puts("x set");
    //fgstd::et::makeexpr<fgstd::et::BinOpOverload<fgstd::op::Add, fgstd::vector<u32>, fgstd::vector<u32> >::expr_type>::type xx = v+v;
    printf("sizeof sum: %u\n", (u32)sizeof(fgstd::et::makeexpr<fgstd::et::BinOpOverload<fgstd::op::Add, fgstd::vector<u32>, fgstd::vector<u32> >::expr_type>::type ));
    //c = v+v;
     //puts("xx set");
    c = (((v+v) + 1u) * 10u);
    //c = v + v;
    //int z = v+v;

    //u32 z = fgstd::expr(1) + 2;
    //printf("z = %u\n", z);

    for(u32 i = 0; i < c.size(); ++i)
        printf("[%u] = %u\n", i, c[i]);


    return 0;
}

