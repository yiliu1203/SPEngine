/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SP_MATH_MATHFWD_H
#define SP_MATH_MATHFWD_H

// MSVC cannot compute the size of math types correctly when this file is included before the
// actual implementations.
// See github.com/google/filament/issues/2190.
#include "PCH.h"
#include <math/color.h>
#include <math/mat2.h>
#include <math/mat3.h>
#include <math/mat4.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>




namespace SP::math {
namespace details {

template <typename T>
class TVec2;
template <typename T>
class TVec3;
template <typename T>
class TVec4;

template <typename T>
class TMat22;
template <typename T>
class TMat33;
template <typename T>
class TMat44;

template <typename T>
class TQuaternion;

}   // namespace details

using mat2d = details::TMat22<double>;
using mat2f = details::TMat22<float>;
using mat2  = details::TMat22<float>;

using mat3d = details::TMat33<double>;
using mat3f = details::TMat33<float>;
using mat3  = details::TMat33<float>;

using mat4d = details::TMat44<double>;
using mat4f = details::TMat44<float>;
using mat4  = details::TMat44<float>;
using mat   = details::TMat44<float>;

using quatd = details::TQuaternion<double>;
using quatf = details::TQuaternion<float>;
using quat  = details::TQuaternion<float>;
using quath = details::TQuaternion<half>;


using vec2d = details::TVec2<double>;
using vec2  = details::TVec2<float>;
using vec2i = details::TVec2<int32>;
using vec2u = details::TVec2<uint32>;
using vec2b = details::TVec2<uint8>;

using vec3d = details::TVec3<double>;
using vec3  = details::TVec3<float>;
using vec3i = details::TVec3<int32>;
using vec3u = details::TVec3<uint32>;
using vec3b = details::TVec3<uint8>;

using vec4d = details::TVec4<double>;
using vec4  = details::TVec4<float>;
using vec4i = details::TVec4<int32>;
using vec4u = details::TVec4<uint32>;
using vec4b = details::TVec4<uint8>;

using color4b = details::Color4B;
using color4f = details::Color4F;
// using color4f = LinearColor;

// using namespace math::details;

namespace matrix = details::matrix;


// template <typename T, typename VectorType = void>
// struct IsVec : std::false_type
// {};

// template <typename T>
// struct IsVec<T, SP::math::details::TVec2<typename T::value_type>> : std::true_type
// {};
// template <typename T>
// struct IsVec<T, SP::math::details::TVec3<typename T::value_type>> : std::true_type
// {};
// template <typename T>
// struct IsVec<T, SP::math::details::TVec4<typename T::value_type>> : std::true_type
// {};

template <typename T, typename VecType = void>
struct IsVec : public std::false_type
{};

template <typename T>
/** void_t 相当于对任何类型都满足推导，会优先用特化版本 */
class IsVec<T, std::void_t<typename T::VecType>> : public std::true_type
{};


template <typename T, typename MatixType = void>
struct IsMatix : public std::false_type
{};

template <typename T>
/** void_t 相当于对任何类型都满足推导，会优先用特化版本 */
class IsMatix<T, std::void_t<typename T::MatixType>> : public std::true_type
{};

template <typename T, typename VecType = void>
struct IsQuat : public std::false_type
{};

template <typename T>
class IsQuat<T, std::enable_if_t<std::is_base_of<quat, T>::value>> : public std::true_type
{};
template <typename T>
class IsQuat<T, std::enable_if_t<std::is_base_of<quatd, T>::value>> : public std::true_type
{};
template <typename T>
class IsQuat<T, std::enable_if_t<std::is_base_of<quath, T>::value>> : public std::true_type
{};

// spdlog 库用来输出
template <typename T>
struct fmt::formatter<T, std::enable_if_t<IsVec<T>::value, char>> : fmt::formatter<std::string>
{
    template <typename FormatCtx>
    auto format(const T& vec, FormatCtx& ctx)
    {
        std::stringstream ss;
        ss << "vec[";
        for (int i = 0; i < vec.size(); i++) {
            ss << vec[i];
            if (i != vec.size() - 1) ss << ",";
        }
        ss << "]";
        return fmt::formatter<std::string>::format(ss.str(), ctx);
    }
};


// spdlog 库用来输出
template <typename T>
struct fmt::formatter<T, std::enable_if_t<IsMatix<T>::value, char>> : fmt::formatter<std::string>
{
    template <typename FormatCtx>
    auto format(const T& _mat, FormatCtx& ctx)
    {
        std::stringstream ss;
        ss << "matix[";
        for (int i = 0; i < _mat.COL_SIZE; i++) {
            for (int j = 0; j < _mat.ROW_SIZE; j++) {
                ss << _mat[i][j];
                if (i * j != (_mat.COL_SIZE * _mat.ROW_SIZE) - 1) ss << ",";
            }
        }
        ss << "]";
        return fmt::formatter<std::string>::format(ss.str(), ctx);
    }
};

// spdlog 库用来输出
template <typename T>
struct fmt::formatter<T, std::enable_if_t<IsQuat<T>::value, char>> : fmt::formatter<std::string>
{
    template <typename FormatCtx>
    auto format(const T& vec, FormatCtx& ctx)
    {
        std::stringstream ss;
        ss << "Quat[";
        for (int i = 0; i < vec.size(); i++) {
            ss << vec[i];
            if (i != vec.size() - 1) ss << ",";
        }
        ss << "]";
        return fmt::formatter<std::string>::format(ss.str(), ctx);
    }
};

}   // namespace SP::math



#endif   // SP_MATH_MATHFWD_H
