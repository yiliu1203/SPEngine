﻿
#include "PCH.h"
#include "color.h"
#include <cmath>




namespace SP::math {



// Color::Color(uint8_t inR, uint8_t inG, uint8_t inB)
//     : r(inR)
//     , g(inG)
//     , b(inB)
//     , a(255)
// {}

// Color::Color(uint8_t inR, uint8_t inG, uint8_t inB, uint8_t inA)
//     : r(inR)
//     , g(inG)
//     , b(inB)
//     , a(inA)
// {}

// Color::Color(const LinearColor& inColor)
// {
//     *this = inColor.ToColor();
// }

// Color::Color(const Color& inOther) = default;

// Color::Color(Color&& inOther) noexcept
//     : r(inOther.r)
//     , g(inOther.g)
//     , b(inOther.b)
//     , a(inOther.a)
// {}

// Color& Color::operator=(const Color& inOther) = default;

// LinearColor Color::ToLinearColor() const
// {
//     return {static_cast<float>(r) / 255.0f,
//             static_cast<float>(g) / 255.0f,
//             static_cast<float>(b) / 255.0f,
//             static_cast<float>(a) / 255.0f};
// }

// std::string Color::ToHexString() const
// {
//     std::stringstream result;
//     result << "0x" << std::hex << r << std::hex << g << std::hex << b << std::hex << a;
//     return result.str();
// }


const Color ColorConsts::white = Color(255, 255, 255, 255);
const Color ColorConsts::black = Color(0, 0, 0, 1);
const Color ColorConsts::red   = Color(1, 0, 0, 1);
const Color ColorConsts::green = Color(0, 1, 0, 1);
const Color ColorConsts::blue  = Color(0, 0, 1, 1);
}   // namespace SP::math
