#pragma once

#include "vec4.h"
#include <cstdint>
#include <sstream>


namespace SP::math::details {

template <typename T>
struct Color
{
    union
    {
        T        v[4];
        TVec3<T> rgb;
        TVec4<T> rgba;
        struct
        {
            T r, g, b, a;
        };
    };

    Color()
        : r(0)
        , g(0)
        , b(0)
        , a(0)
    {}
    Color(uint8_t inR, uint8_t inG, uint8_t inB)
        : r(inR)
        , g(inG)
        , b(inB)
        , a(255)
    {}
    Color(uint8_t inR, uint8_t inG, uint8_t inB, uint8_t inA)
        : r(inR)
        , g(inG)
        , b(inB)
        , a(inA)
    {}
    // explicit Color(const LinearColor& inColor);
    Color(const Color& inOther) = default;
    Color(Color&& inOther)
        : r(inOther.r)
        , g(inOther.g)
        , b(inOther.b)
        , a(inOther.a)
    {}
    Color& operator=(const Color& inOther) = default;
};

using Color4B = Color<uint8>;
using Color4F = Color<float>;

struct ColorConsts
{
    static const Color4B white;
    static const Color4B black;
    static const Color4B red;
    static const Color4B blue;
    static const Color4B green;
    // TODO more color here
};
}   // namespace SP::math::details
