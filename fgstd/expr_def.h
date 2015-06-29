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

}}
