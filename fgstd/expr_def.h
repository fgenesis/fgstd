#pragma once

#include "traits.h"
#include "op_def.h"

namespace fgstd {
namespace et {

template<typename T, bool ty>
struct vec_type_or_void
{
    typedef void type;
};

template<typename T>
struct vec_type_or_void<T, true>
{
    typedef typename T::vec_type type;
};

template <typename T>
struct has_vec_type
{
    typedef char yes[1];
    typedef char no[2];

    template <typename C> static yes& test(typename C::vec_type*);
    template <typename> static no& test(...);

    enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};

template<typename T>
struct default_vec_type
{
    typedef typename vec_type_or_void<T, has_vec_type<T>::value>::type type;
};

template<typename E, typename TRAITS = E>
class Expr
{
public:
    typedef bool is_expr_tag;
    typedef TRAITS traits;
    typedef typename TRAITS::value_type value_type;
    typedef typename default_vec_type<TRAITS>::type vec_type;
    typedef typename TRAITS::size_type size_type;

    FGSTD_FORCE_INLINE const value_type operator[](size_type i) const { return static_cast<E const&>(*this)[i]; }
    FGSTD_FORCE_INLINE const vec_type block(size_type i)        const { return (static_cast<E const&>(*this)).block(i) }
    //FGSTD_FORCE_INLINE       value_type& operator[](size_type i)       { return static_cast<E      &>(*this)[i]; }
    FGSTD_FORCE_INLINE       size_type  size()                   const { return static_cast<E const&>(*this).size(); }

    //FGSTD_FORCE_INLINE operator E&()             { return static_cast<      E&>(*this); }
    FGSTD_FORCE_INLINE operator E const&() const { return static_cast<const E&>(*this); }
};

template <typename T>
struct has_internal_expr_type
{
    typedef char yes[1];
    typedef char no[2];

    template <typename C> static yes& test(typename C::Expr*);
    template <typename> static no& test(...);

    enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};

template <typename T>
struct has_is_expr_tag
{
    typedef char yes[1];
    typedef char no[2];

    template <typename C> static yes& test(typename C::is_expr_tag*);
    template <typename> static no& test(...);

    enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};

template <typename T>
struct has_expr_source_tag
{
    typedef char yes[1];
    typedef char no[2];

    template <typename C> static yes& test(typename C::is_expr_source_tag*);
    template <typename> static no& test(...);

    enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};

/*
template <typename T>
struct has_expr_sink_tag
{
    typedef char yes[1];
    typedef char no[2];

    template <typename C> static yes& test(typename C::is_expr_sink_tag*);
    template <typename> static no& test(...);

    enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};
*/

template <typename T>
struct is_expr_type
{
    typedef char yes[1];
    typedef char no[2];

    template <typename C> static yes& test(typename C::is_expr_tag*);
    template <typename> static no& test(...);

    enum { value = sizeof(test<T>(0)) == sizeof(yes) };
};

template <typename T>
struct is_usable_expr
{
    enum { value = has_is_expr_tag<T>::value || has_internal_expr_type<T>::value };
};

template <typename T>
struct is_source_expr
{
    enum { value = is_usable_expr<T>::value && has_expr_source_tag<T>::value};
};

/*
template <typename T>
struct is_sink_expr
{
    enum { value = is_usable_expr<T>::value && has_expr_sink_tag<T>::value};
};
*/


template<typename E, bool FAST>
struct expr_store_;
// rest is defined in expr.h

template<typename E>
struct expr_store2
{
    FGSTD_FORCE_INLINE static void store(typename E::value_type *ptr, const E& src)
    {
         // TODO: also check that there is really an acceleration for the type
         expr_store_<E, fgstd::is_pod<typename E::value_type>::value>::store(ptr, src);
    }
};

template<typename T, typename E>
typename enable_if<is_usable_expr<E>::value && is_same<typename E::value_type, T>::value, void>::type
expr_store(T *ptr, const E& e)
{
    expr_store2<E>::store(ptr, e);
}


/*template<typename E>
typename fgstd::enable_if<is_usable_expr<E>::value, fgstd::et::ExprSink<E>>::type
sink(const E& e)
{
    return e; //fgstd::et::ExprSink<E>(e);
}*/

}}
