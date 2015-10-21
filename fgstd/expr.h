#pragma once

#include "platform.h"
#include "traits.h"
#include "types.h"
#include "op.h"
#include "expr_def.h"

namespace fgstd {
namespace et {

template<typename E>
class ExprTraits
{
public:
    typedef typename E::value_type value_type;
    typedef typename E::vec_type vec_type;
    typedef typename E::size_type size_type;
};

template<typename T, typename VT, typename ST>
struct ExprTraits2
{
public:
    typedef T value_type;
    typedef VT vec_type;
    typedef ST size_type;
};

template<typename T>
struct _GetVec
{
    typedef typename op::Traits<T>::vec_type type;
};


template<typename T>
class ScalarExpr : public Expr<ScalarExpr<T>,  ExprTraits2<typename op::Traits<T>::value_type, typename op::Traits<T>::vec_type, u32> >
{
public:
    typedef op::Traits<T> Tr;
    typedef typename u32 size_type;
    typedef typename Tr::vec_type vec_type;
    typedef typename Tr::value_type value_type;

    FGSTD_FORCE_INLINE ScalarExpr(const T& val) : _vec(op::Load1<T>::applyVec(&val)) {}

    FGSTD_FORCE_INLINE const value_type operator[](size_type) const { return op::Get1<T>::applyVec(_vec); }
    FGSTD_FORCE_INLINE const vec_type& block(size_type)        const { return _vec; }
    FGSTD_FORCE_INLINE       size_type  size()                 const { return 1; }
    //FGSTD_FORCE_INLINE operator T const&() const { return _val; }

private:
    const vec_type _vec;
};

template<typename T, u32 SZ>
class ArrayExpr : public Expr<ArrayExpr<T, SZ>, ExprTraits2<typename op::Traits<T>::value_type, typename op::Traits<T>::vec_type, u32> >
{
public:
    typedef bool is_expr_source_tag;

    typedef op::Traits<T> Tr;
    typedef typename u32 size_type;
    typedef typename Tr::vec_type vec_type;
    typedef typename Tr::value_type value_type;

    FGSTD_FORCE_INLINE ArrayExpr(const T (&arr)[SZ]) : _arr(arr) {}

    typedef const T (&_arrayRef)[SZ];

    FGSTD_FORCE_INLINE const value_type& operator[](size_type i) const { return op::Load<T>::applyScalar(addressof(_arr[i])); }
    FGSTD_FORCE_INLINE const vec_type block(size_type i)         const { return op::Load<T>::applyVec   (addressof(_arr[i])); }
    FGSTD_FORCE_INLINE       size_type  size()                   const { return SZ; }
    // Must NOT be back-convertible to array! Otherwise it will do so when applying operator+
    //FGSTD_FORCE_INLINE operator _arrayRef () const { return _arr; }

private:
    _arrayRef _arr;
};

template<typename T>
class PointerExpr : public Expr<PointerExpr<T>, ExprTraits2<typename op::Traits<T>::value_type, typename op::Traits<T>::vec_type, u32> >
{
public:
    typedef bool is_expr_source_tag;

    typedef op::Traits<T> Tr;
    typedef typename u32 size_type;
    typedef typename Tr::vec_type vec_type;
    typedef typename Tr::value_type value_type;

    FGSTD_FORCE_INLINE PointerExpr(const T *p, u32 sz) : _ptr(p), _sz(sz) {}

    FGSTD_FORCE_INLINE const value_type& operator[](size_type i) const { return op::Load<T>::applyScalar(addressof(_ptr[i])); }
    FGSTD_FORCE_INLINE const vec_type block(size_type i)         const { return op::Load<T>::applyVec   (addressof(_ptr[i])); }
    FGSTD_FORCE_INLINE       size_type  size()                   const { return _sz; }
    // Must NOT be back-convertible to array! Otherwise it will do so when applying operator+
    //FGSTD_FORCE_INLINE operator _arrayRef () const { return _arr; }

private:
    const T *_ptr;
    const u32 _sz;
};

template<typename E>
class LoadWrapExpr : public Expr<LoadWrapExpr<E>, ExprTraits2<typename op::Traits<typename E::value_type>::value_type, typename op::Traits<typename E::value_type>::vec_type, typename E::size_type> >
{
public:
    typedef typename E::value_type T;
    typedef op::Traits<T> Tr;
    typedef typename E::size_type size_type;
    typedef typename Tr::vec_type vec_type;
    typedef typename Tr::value_type value_type;

    FGSTD_FORCE_INLINE LoadWrapExpr(const E& e) : _e(e) {}

    FGSTD_FORCE_INLINE const value_type& operator[](size_type i)  const { return op::Load<T>::applyScalar(addressof(_e[i])); }
    FGSTD_FORCE_INLINE const vec_type block(size_type i)          const { return op::Load<T>::applyVec   (addressof(_e[i])); }
    FGSTD_FORCE_INLINE       size_type  size()                    const { return _e.size(); }
    //FGSTD_FORCE_INLINE operator const E& () const { return _e; }

private:
    const E& _e;
};

template<typename OP, typename E1, typename E2>
class BinOpExpr : public Expr<BinOpExpr<OP, E1, E2>, ExprTraits2<typename OP::value_type, typename OP::vec_type, typename E1::size_type> >
{
public:
    typedef typename E1::size_type size_type;
    typedef typename OP::value_type value_type;
    typedef typename OP::vec_type vec_type;

    FGSTD_FORCE_INLINE BinOpExpr(const E1& a, const E2& b)
        : _a(a), _b(b)
    {}

    FGSTD_FORCE_INLINE size_type  size()                  const { return _a.size(); }
    FGSTD_FORCE_INLINE value_type operator[](size_type i) const { return OP::applyScalar(_a[i], _b[i]); }
    FGSTD_FORCE_INLINE vec_type block(size_type i)        const { return OP::applyVec(_a.block(i), _b.block(i)); }

private:
    const E1 _a;
    const E2 _b;
};


template<typename T, int _>
struct makeET;

template<typename T>
struct makeET<T, 0>
{
    typedef ScalarExpr<T> type;
    FGSTD_FORCE_INLINE static type makeExpr(const T& e)
    {
        return ScalarExpr<T>(e);
    }
};

template<typename T>
struct makeET<T, 1>
{
    typedef T type;
    FGSTD_FORCE_INLINE static const type& makeExpr(const T& e)
    {
        return e;
    }
};

template<typename T>
struct makeET<T, 2>
{

    typedef typename T::Expr subtype;
    typedef typename LoadWrapExpr<subtype> type;
    FGSTD_FORCE_INLINE static type makeExpr(const T& e)
    {
        return type(subtype(e));
    }
};

template<typename E1, typename E2, bool _>
struct BinOpCheck2
{
    enum { enable = false };
};

template<typename E1, typename E2>
struct BinOpCheck2<E1, E2, true>
{
    enum {
        enable = is_same<
            typename get_value_type<E1>::type,
            typename get_value_type<E2>::type
        >::value
    };
};

template<typename E1, typename E2>
struct BinOpCheck
{
    enum
    {
        enable = BinOpCheck2<E1, E2, is_usable_expr<E1>::value || is_usable_expr<E2>::value>::enable
    };
};

template<typename T>
struct makeexpr
{
    enum 
    {
        _1 = (is_expr_type<T>::value ? 1 : 0) | (has_internal_expr_type<T>::value ? 2 : 0),
        dispatch = _1 == 3 ? 1 : _1
    };

    typedef makeET<T, dispatch> conv;
    typedef typename conv::type type;
};

template<typename T>
FGSTD_FORCE_INLINE typename et::makeexpr<T>::type expr(const T& x)
{
    return et::makeexpr<T>::conv::makeExpr(x);
}

template<typename T, u32 SZ>
FGSTD_FORCE_INLINE et::ArrayExpr<T, SZ> expr(const T (&arr)[SZ])
{
    return ArrayExpr<T, SZ>(arr);
}

template<typename T>
FGSTD_FORCE_INLINE et::PointerExpr<T> expr(const T *ptr, u32 sz)
{
    return PointerExpr<T>(ptr, sz);
}

template<typename OP, typename E1, typename E2>
struct BinOpOverload
{
    typedef typename makeexpr<E1>::type E1T;
    typedef typename OP::template Op<typename E1T::value_type> Opr;
    typedef BinOpExpr<
        Opr,
        E1T,
        typename makeexpr<E2>::type
    > expr_type;

    static FGSTD_FORCE_INLINE expr_type makeOp(const E1& a, const E2& b)
    {
        return expr_type(expr(a), expr(b));
    }
};

/* FIXME
template<typename E>
struct expr_store_<E, false>
{
    typedef typename E::size_type size_type;
    typedef typename E::value_type T;

    static void store(T* p, const E& e)
    {
        const size_type sz = e.size();
        for(typename E::size_type i = 0; i < sz; ++i)
            new (p + i) T(e[i]);
    }
};
*/

template<typename E>
struct expr_store_<E, true>
{
    typedef typename E::value_type T;
    typedef typename E::size_type size_type;
    typedef fgstd::op::Operators<T> Opr;

    static void store(T* dst, const E& src)
    {
        //StoreExpr<DST*, SRC> store(dst, src);
        const size_type sz = src.size();
        size_type i = 0;

        // actual computation happens here, just before the store
        if(Opr::BLOCK_SIZE > 1)
            for(; i < sz; i += Opr::BLOCK_SIZE)
                Opr::Vec::store(dst + i, src.block(i));
        for(; i < sz; ++i)
            Opr::Scalar::store(dst + i, src[i]);
    }
};


} // end namespace et

using et::expr;


}


#define _FGSTD_MAKE_OPR(cls, _, opr) \
template <typename E1, typename E2> \
typename fgstd::enable_if< \
   fgstd::et::BinOpCheck<E1, E2>::enable, \
   typename fgstd::et::BinOpOverload<fgstd::op::cls, E1, E2>::expr_type \
>::type \
FGSTD_FORCE_INLINE operator opr (const E1& a, const E2& b) \
{ \
    return fgstd::et::BinOpOverload<fgstd::op::cls, E1, E2>::makeOp(a, b); \
}
#define _FGSTD_MAKE_FUNC(cls, mth) /* TODO: fixme */
#include "_op_builder.h"
