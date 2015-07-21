#pragma once

namespace fgstd {
namespace et {

template<typename E, typename TRAITS = E>
class Expr
{
public:
    typedef bool is_expr_tag;
    typedef typename TRAITS::value_type value_type;
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
    enum { value = is_expr_type<T>::value || has_internal_expr_type<T>::value };
};



}}
