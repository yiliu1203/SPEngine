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

    constexpr Color()
        : r(0)
        , g(0)
        , b(0)
        , a(0)
    {}
    constexpr Color(uint8_t inR, uint8_t inG, uint8_t inB)
        : r(inR)
        , g(inG)
        , b(inB)
        , a(255)
    {}
    constexpr Color(uint8_t inR, uint8_t inG, uint8_t inB, uint8_t inA)
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

using Color4B = Color<uint8_t>;
using Color4F = Color<float>;

constexpr Color4B white = Color4B(255, 255, 255, 255);
constexpr Color4B black = Color4B(0, 0, 0, 1);
constexpr Color4B red   = Color4B(1, 0, 0, 1);
constexpr Color4B green = Color4B(0, 1, 0, 1);
constexpr Color4B blue  = Color4B(0, 0, 1, 1);

}   // namespace SP::math::details
