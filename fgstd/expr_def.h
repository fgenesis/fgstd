#pragma once

#include "traits.h"

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
   // typedef typename default_vec_type<TRAITS>::type vec_type;
    typedef typename TRAITS::size_type size_type;

    FGSTD_FORCE_INLINE const value_type& operator[](size_type i) const { return static_cast<E const&>(*this)[i]; }
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


template<typename T, typename E, bool FAST>
struct expr_store_;

template<typename T, typename E>
struct expr_store_<T, E, false>
{
    typedef typename E::size_type size_type;

    static void store(T* p, const E& e)
    {
        const size_type sz = e.size();
        for(typename E::size_type i = 0; i < sz; ++i)
            new (p + i) T(e[i]);
    }
};

template<typename T, typename E>
struct expr_store_<T, E, true>
{
    static void store(T* p, const E& e)
    {
        et::store_array_pod(p, e);
    }
};

template<typename T, typename E>
//typename fgstd::enable_if<is_sink_expr<E>::value, void>::type
void expr_store(T *ptr, const E& e)
{
    // todo: add check for fast conversion expression, eg. int<->float
    expr_store_<T, E, fgstd::is_pod<T>::value && is_same<T, typename E::value_type>::value>::store(ptr, e);
}


/*template<typename E>
typename fgstd::enable_if<is_usable_expr<E>::value, fgstd::et::ExprSink<E>>::type
sink(const E& e)
{
    return e; //fgstd::et::ExprSink<E>(e);
}*/

}}
