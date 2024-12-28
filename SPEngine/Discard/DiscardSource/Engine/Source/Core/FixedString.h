#pragma once
#include "PCH.h"

namespace SP {

// 不能导出 dllexport, 模板本来就不应该导出
template <size_t N>
struct FixedString
{
    constexpr static size_t size = N;
    char                    m_Data[N];
    constexpr FixedString(const char (&array)[N]) { std::copy(array, array + N, m_Data); }

    template <size_t M>
    const bool operator==(const char (&array)[M]) const
    {
        return (N == M) && std::string_view(m_Data) == std::string_view(array);
    }



    // 定义成成员函数似乎不行
    // template <size_t M>
    // constexpr bool operator==(const FixedString<M>& s) const
    // {
    //     return (N == M) && std::string_view<m_Data> == std::string_view(s.m_Data);
    // }
};

template <size_t M, size_t N>
bool operator==(const FixedString<M>& s1, const FixedString<N>& s2)
{
    return (M == N) && std::string_view(s1.m_Data) == std::string_view(s2.m_Data);
}

}   // namespace SP