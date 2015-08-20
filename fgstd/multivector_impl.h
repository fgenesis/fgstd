#pragma once

#include "multivector_def.h"
#include "typelist.h"
#include "mem.h"

namespace fgstd {

namespace priv {

template<typename V>
struct MV_functor_base
{
    template<unsigned N>
    static FGSTD_FORCE_INLINE void apply2(void *p, u32 n, u32 o);

    template<unsigned N>
    static FGSTD_FORCE_INLINE void apply3(void *dst, void *src, u32 n, u32 o1, u32 o2);
};


#define _FGSTD_MV_FUNCTOR2(f) \
template<typename V> \
struct MV_##f : MV_functor_base<V> \
{ \
    template<unsigned N> \
    static FGSTD_FORCE_INLINE void apply2(void *p, u32 n, u32 o) \
    { \
        typedef typename V::template gettype<N>::type T; \
        T *t = V::_getptr<N>(p); \
        f(t + o, n); \
    } \
}

#define _FGSTD_MV_FUNCTOR3(f) \
template<typename V> \
struct MV_##f : MV_functor_base<V> \
{ \
    template<unsigned N> \
    static FGSTD_FORCE_INLINE void apply3(void *dst, void *src, u32 n, u32 o1, u32 o2) \
    { \
        typedef typename V::template gettype<N>::type T; \
        T *tdst = V::_getptr<N>(dst); \
        T *tsrc = V::_getptr<N>(src); \
        f(tdst + o1, tsrc + o2, n); \
    } \
}

_FGSTD_MV_FUNCTOR2(mem_destroy);
_FGSTD_MV_FUNCTOR2(mem_construct_default);

_FGSTD_MV_FUNCTOR3(mem_construct_move);
_FGSTD_MV_FUNCTOR3(mem_construct_move_destroy);
_FGSTD_MV_FUNCTOR3(mem_construct_copy);
_FGSTD_MV_FUNCTOR3(mem_move_distinct);
_FGSTD_MV_FUNCTOR3(mem_copy_distinct);

#undef _FGSTD_MV_FUNCTOR2
#undef _FGSTD_MV_FUNCTOR3

template<typename V, template<typename V> class F, unsigned N = 0, bool rec = true>
struct MV_apply;

template<typename V, template<typename V> class F, unsigned N>
struct MV_apply<V, F, N, true>
{
    typedef F<V> FF;

    static FGSTD_FORCE_INLINE void apply2(void *p, u32 n, u32 o)
    {
        FF::apply2<N>(p, n, o);
        MV_apply<V, F, N+1, ((N+1) < V::ntypes)>::apply2(p, n, o);
    }

    static FGSTD_FORCE_INLINE void apply3(void *p1, void *p2, u32 n, u32 o1, u32 o2)
    {
        FF::apply3<N>(p1, p2, n, o1, o2);
        MV_apply<V, F, N+1, ((N+1) < V::ntypes)>::apply3(p1, p2, n, o1, o2);
    }
};

template<typename V, template<typename V> class F, unsigned N>
struct MV_apply<V, F, N, false>
{
    static FGSTD_FORCE_INLINE void apply2(void *p, u32 n, u32 o) {}
    static FGSTD_FORCE_INLINE void apply3(void *dst, void *src, u32 n, u32 o1, u32 o2) {}
};

}


template<typename TL>
multivector<TL>::multivector(IAllocator *a)
    : _base(NULL), _sz(0), _capacity(0), _alloc(a)
{
}

template<typename TL>
multivector<TL>::multivector(u32 sz, IAllocator *a)
    : _base(NULL), _sz(0), _capacity(0), _alloc(a)
{
    if(sz)
        resize(sz);
}

#ifdef FGSTD_USE_CPP11
template<typename TL>
multivector<TL>::multivector(multivector<TL>&& v)
    : _base(v._base), _sz(v._sz), _capacity(v._capacity), _alloc(v._alloc)
{
    v._kill();
}
#endif

template<typename TL>
FGSTD_FORCE_INLINE multivector<TL>& multivector<TL>::operator=(multivector<TL> v) // copy-and-swap
{
    v.swap(*this);
    return *this;
}

template<typename TL>
FGSTD_FORCE_INLINE void multivector<TL>::swap(multivector<TL>& v)
{
    fgstd::swap(_sz, v._sz);
    fgstd::swap(_capacity, v._capacity);
    fgstd::swap(_base, v._base);
    fgstd::swap(_alloc, v._alloc);
}


template<typename TL>
multivector<TL>::~multivector()
{
    dealloc();
}

template<typename TL>
void multivector<TL>::_kill()
{
    _base = NULL;
    _sz = 0;
    _capacity = 0;
}

template<typename TL>
void multivector<TL>::clear()
{
    priv::MV_apply<multivector<TL>, priv::MV_mem_destroy>::apply2(_base, _sz, 0);
    _sz = 0;
}

template<typename TL>
void multivector<TL>::dealloc()
{
    clear();
    _alloc->Free(_base);
    _base = NULL;
    _capacity = 0;
}

template<typename TL> template<unsigned N>
FGSTD_FORCE_INLINE const typename multivector<TL>::view<N>
multivector<TL>::getview() const
{
    return view<N>(*this);
}

template<typename TL> template<unsigned N>
FGSTD_FORCE_INLINE typename multivector<TL>::view<N>
multivector<TL>::getview()
{
    return view<N>(*this);
}

template<typename TL> template<unsigned N>
FGSTD_FORCE_INLINE const typename multivector<TL>::gettype<N>::type&
multivector<TL>::at(u32 i) const
{
    return _ptr<N>()[i];
}

template<typename TL> template<unsigned N>
FGSTD_FORCE_INLINE typename multivector<TL>::gettype<N>::type&
multivector<TL>::at(u32 i)
{
    return _ptr<N>()[i];
}

template<typename TL> template<unsigned N>
FGSTD_FORCE_INLINE const typename multivector<TL>::gettype<N>::type *
multivector<TL>::data() const
{
    return _ptr<N>();
}

template<typename TL> template<unsigned N>
FGSTD_FORCE_INLINE typename multivector<TL>::gettype<N>::type *
multivector<TL>::data()
{
    return _ptr<N>();
}

template<typename TL>
FGSTD_FORCE_INLINE u32 multivector<TL>::size() const
{
    return _sz;
}

template<typename TL>
FGSTD_FORCE_INLINE bool multivector<TL>::empty() const
{
    return !_sz;
}

template<typename TL>
void multivector<TL>::resize(u32 n)
{
    const u32 oldsz = _sz;
    _resize_noinit(n);
    if(n > oldsz)
        priv::MV_apply<multivector<TL>, priv::MV_mem_destroy>::apply2(_base, n - oldsz, oldsz);
}

template<typename TL>
void multivector<TL>::_resize_noinit(u32 n)
{
    const u32 oldsz = _sz;
    if(n < oldsz)
        priv::MV_apply<multivector<TL>, priv::MV_mem_destroy>::apply2(_base, oldsz - n, oldsz);
    reserve(n);
    _sz = n;
}

template<typename TL>
FGSTD_FORCE_INLINE void multivector<TL>::reserve(u32 n)
{
    if(_capacity < n)
        _enlarge(n);
}

template<typename TL>
FGSTD_FORCE_INLINE unsigned multivector<TL>::remain() const
{
    return _capacity - _sz;
}

template<typename TL>
FGSTD_FORCE_INLINE void multivector<TL>::_enlarge(u32 n)
{
    size_t bytes = TL::array_req_bytes(n) + TL::array_offsets_bytes;
    void *p = _alloc->Alloc(bytes);
    TL::array_offs_write(n, (u32*)p, TL::array_offsets_bytes);
    if (_base)
    {
        priv::MV_apply<multivector<TL>, priv::MV_mem_construct_move_destroy>::apply3(p, _base, _sz, 0, 0);
        _alloc->Free(_base);
    }
    _base = p;
    _capacity = n;
}

template<typename TL> template<unsigned N>
FGSTD_FORCE_INLINE typename multivector<TL>::template gettype<N>::type * multivector<TL>::_ptr()
{
    return _getptr<N>(_base);
}

template<typename TL> template<unsigned N>
FGSTD_FORCE_INLINE typename multivector<TL>::template gettype<N>::type * multivector<TL>::_getptr(void *p)
{
    const u32 *offs = static_cast<u32*>(p);
    return reinterpret_cast<gettype<N>::type*>( ((char*)p) + offs[N] );
}

template<typename T>
FGSTD_FORCE_INLINE IAllocator *multivector<T>::get_alloc()
{
    return _alloc;
}

template<typename T>
FGSTD_FORCE_INLINE void multivector<T>::pop_back()
{
    priv::MV_apply<multivector<TL>, priv::MV_mem_destroy>::apply2(_base, 1, --_sz);
}






}
