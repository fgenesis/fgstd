#pragma once

#include "types.h"


namespace fgstd {

class IAllocator;
extern IAllocator *g_defaultAlloc;

template<typename TL>
class multivector
{
public:
    typedef u32 size_type;
    typedef TL tl;

    multivector(IAllocator *a = g_defaultAlloc);
    multivector(u32 sz, IAllocator *a = g_defaultAlloc);
    //multivector(const T *mem, u32 n, IAllocator *a = g_defaultAlloc);


    multivector(const multivector<TL>& v);
#ifdef FGSTD_USE_CPP11
    multivector(multivector<TL>&& v);
#endif
    ~multivector();

    // const
    u32 size() const;
    bool empty() const;
    //const T& operator[](u32 i) const;
    u32 remain() const;
    //const T *data() const;

    // modifying
    void clear();
    void dealloc();
    //void fill(const T& x);
    void resize(u32 n);
    //void resize(u32 n, const T& val);
    void reserve(u32 n);
    void pop_back();
    //T get_pop_back();
    //bool try_pop_back(T& e);
//T& push_back(const T& e);
#ifdef FGSTD_USE_CPP11
    //T& push_back(T&& e);
#endif
    //u32 shift_left(u32 n);
    //T& operator[](u32 i);
    //void swapElems(vector<T>& v);
    void swap(multivector<TL>& v);
    //T *data();
    //void *emplace_new();
    //T& back();

    // ops
    multivector<TL>& operator=(multivector<TL> v);
    //template<typename E>
    //vector<T>& operator=(const et::Expr<E>& e);


    IAllocator *get_alloc();

    template<unsigned N>
    struct gettype
    {
        typedef typename TL::template get<N>::type type;
    };

    template<unsigned N, typename TCHECK = typedef typename gettype<N>::type>
    class view
    {
    public:
        typedef typename gettype<N>::type type;
        view(multivector<TL>& v) : ptr(v._ptr<N>()) {}

        FGSTD_FORCE_INLINE const type& operator[](u32 i) const
        {
            return ptr[i];
        }
        FGSTD_FORCE_INLINE type& operator[](u32 i)
        {
            return ptr[i];
        }

        type * const ptr;
    private:
        static void _compile_check() { fgstd_static_assert((is_same<type, TCHECK>::value)); }
    };

    template<unsigned N> const typename view<N> getview() const;
    template<unsigned N>       typename view<N> getview();
    template<unsigned N> const typename gettype<N>::type& at(u32 i) const;
    template<unsigned N>       typename gettype<N>::type& at(u32 i);
    template<unsigned N> const typename gettype<N>::type *data() const;
    template<unsigned N>       typename gettype<N>::type *data();

    template<unsigned N> static typename gettype<N>::type *_getptr(void *p);

    enum { ntypes = TL::length, maxalign = TL::maxalign };

private:
    void _enlarge(u32 n);
    void _destroy(u32 from, u32 to);
    void _resize_noinit(u32 n);
    void _kill();

    template<unsigned N>
    typename gettype<N>::type *_ptr();

    void *_base;
    u32 _sz;
    u32 _capacity;
    IAllocator *_alloc;
};


}

