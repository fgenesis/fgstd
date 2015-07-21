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
    typedef typename E::size_type size_type;
};

template<typename VT, typename ST>
class ExprTraits2
{
public:
    typedef VT value_type;
    typedef ST size_type;
};

template<typename T>
class ScalarExpr : public Expr<ScalarExpr<T>, ExprTraits2<T, u32> >
{
public:
    FGSTD_FORCE_INLINE ScalarExpr(const T& val) : _val(val) {}
    typedef u32 size_type;
    typedef T value_type;

    FGSTD_FORCE_INLINE const value_type& operator[](size_type) const { return _val; }
    FGSTD_FORCE_INLINE       size_type  size()                 const { return 1; }
    FGSTD_FORCE_INLINE operator T const&() const { return _val; }

private:
    const T _val;
};

// TODO: ArrayExpr

template<typename OP, typename E1, typename E2>
class BinOpExpr : public Expr<BinOpExpr<OP, E1, E2>, ExprTraits2<typename E1::size_type, typename OP::value_type> >
{
public:
    typedef typename E1::size_type size_type;
    typedef typename OP::value_type value_type;

    FGSTD_FORCE_INLINE BinOpExpr(const E1& a, const E2& b)
        : _a(a), _b(b)
    {}

    FGSTD_FORCE_INLINE size_type  size()                  const { return _a.size(); }
    FGSTD_FORCE_INLINE value_type operator[](size_type i) const { return OP::apply(_a[i], _b[i]); }

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
    typedef typename T::Expr type;
    FGSTD_FORCE_INLINE static type makeExpr(const T& e)
    {
        return typename T::Expr(e);
    }
};


template<typename E1, typename E2>
struct BinOpCheck
{
    enum { enable = is_usable_expr<E1>::value || is_usable_expr<E2>::value };
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

template<typename OP, typename E1, typename E2>
struct BinOpOverload
{
    typedef BinOpExpr<
        typename OP::template Op<typename makeexpr<E1>::type::value_type>,
        typename makeexpr<E1>::type,
        typename makeexpr<E2>::type
    > expr_type;
    
    static FGSTD_FORCE_INLINE expr_type makeOp(const E1& a, const E2& b)
    {
        return expr_type(expr(a), expr(b));
    }
};


} // end namespace et

using et::expr;


#define _FGSTD_MAKE_BINOP(opr, cls) \
template <typename E1, typename E2> \
typename enable_if< \
   et::BinOpCheck<E1, E2>::enable, \
   typename et::BinOpOverload<op::cls, E1, E2>::expr_type \
>::type \
FGSTD_FORCE_INLINE operator opr (const E1& a, const E2& b) \
{ \
    return et::BinOpOverload<op::cls, E1, E2>::makeOp(a, b); \
}

_FGSTD_MAKE_BINOP(+, Add)
_FGSTD_MAKE_BINOP(-, Sub)
_FGSTD_MAKE_BINOP(*, Mul)
_FGSTD_MAKE_BINOP(/, Div)
_FGSTD_MAKE_BINOP(^, Xor)
_FGSTD_MAKE_BINOP(%, Mod)
_FGSTD_MAKE_BINOP(&, BitAnd)
_FGSTD_MAKE_BINOP(|, BitOr)
_FGSTD_MAKE_BINOP(<<, Lsh)
_FGSTD_MAKE_BINOP(>>, Rsh)

#undef _FGSTD_MAKE_BINOP


}
