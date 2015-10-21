#pragma once

namespace fgstd {
namespace op {

template<typename T>
struct Traits
{
    typedef T value_type;
    typedef T vec_type;
    enum { BLOCK_SIZE = 1 };
};


}}
