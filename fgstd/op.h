#pragma once

#include "platform.h"

namespace fgstd {
namespace op {

struct Add
{
    template<typename T>
    struct Op
    {
        typedef T value_type;

        template<typename A, typename B>
        FGSTD_FORCE_INLINE T operator()(const A& a, const B& b) const
        {
            return a + b;
        }
    };
};

struct Sub
{
    template<typename T>
    struct Op
    {
        typedef T value_type;

        template<typename A, typename B>
        FGSTD_FORCE_INLINE T operator()(const A& a, const B& b) const
        {
            return a - b;
        }
    };
};

struct Mul
{
    template<typename T>
    struct Op
    {
        typedef T value_type;

        template<typename A, typename B>
        FGSTD_FORCE_INLINE T operator()(const A& a, const B& b) const
        {
            return a * b;
        }
    };
};

struct Div
{
    template<typename T>
    struct Op
    {
        typedef T value_type;

        template<typename A, typename B>
        FGSTD_FORCE_INLINE T operator()(const A& a, const B& b) const
        {
            return a / b;
        }
    };
};




}}

