#pragma once

#include "types.h"
#include "expr_def.h"


namespace fgstd {

class IAllocator;
extern IAllocator *g_defaultAlloc;

template<typename T>
class vector
{
public:
    typedef u32 size_type;
    typedef T value_type;
    typedef T& reference;

    vector(IAllocator *a = g_defaultAlloc);
    vector(u32 sz, const T& def = T(), IAllocator *a = g_defaultAlloc);
    vector(const T *mem, u32 n, IAllocator *a = g_defaultAlloc);

    template<u32 SZ>
    vector(T (&arr)[SZ], IAllocator *a = g_defaultAlloc);

    template<typename E>
    vector(const et::Expr<E>& e, IAllocator *a = g_defaultAlloc);

    vector(const vector<T>& v);
#ifdef FGSTD_USE_CPP11
    vector(vector<T>&& v);
#endif
    ~vector();

    // const
    u32 size() const;
    const T& operator[](u32 i) const;
    u32 remain() const;
    const T *data() const;

    // modifying
    void clear();
    void dealloc();
    void fill(const T& x);
    void resize(u32 n);
    void resize(u32 n, const T& val);
    void reserve(u32 n);
    T pop_back();
    bool try_pop_back(T& e);
    T& push_back(const T& e);
#ifdef FGSTD_USE_CPP11
    T& push_back(T&& e);
#endif
    //u32 shift_left(u32 n);
    T& operator[](u32 i);
    void swapElems(vector<T>& v);
    void swap(vector<T>& v);
    T *data();
    void *emplace_new();

    // ops
#ifdef FGSTD_USE_CPP11
    vector<T>& operator=(vector<T> v);
#else
    vector<T>& operator=(const vector<T>& v);
#endif
    template<typename E>
    vector<T>& operator=(const et::Expr<E>& e);


    IAllocator *get_alloc();

private:
    void _enlarge(u32 n);
    void _destroy(u32 from, u32 to);
    void _resize_noinit(u32 n);
    void _kill();

    static void _copyp(T *dst, const T *src, u32 n);
    static void _initcopyp(T *dst, const T *src, u32 n);
    static void _initmovep(T *dst, T *src, u32 n);
    static void _destroyp(T *p, u32 n);

private:
    T *_arr;
    u32 _sz;
    u32 _capacity;
    IAllocator *_alloc;


public:
    class Expr : public et::Expr<vector<T> >
    {
    public:
        typedef vector<T> container_type;
        typedef T value_type;
        typedef typename container_type::size_type size_type;

        FGSTD_FORCE_INLINE Expr(const container_type &v)
            : _v(v)
        {}

        FGSTD_FORCE_INLINE const value_type& operator[](size_type i) const { return _v[i]; }
        FGSTD_FORCE_INLINE size_type  size()                         const { return _v.size(); }
        FGSTD_FORCE_INLINE operator container_type const&() const { return _v; }

    private:
        const container_type& _v;
    };
};

}

