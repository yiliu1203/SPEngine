// enum_info.hpp
#ifndef ENUM_INFO_HPP
#define ENUM_INFO_HPP

#include <cstddef>
#include <string_view>
#include <type_traits>

namespace SP::refl {

namespace details {

using namespace std;

template <typename Enum_type>
constexpr auto enum_type_name() noexcept
{
    // 静态断言, Enum_type 必须是枚举类型
    static_assert(std::is_enum_v<Enum_type>, " requires Enum_type == enum");
#ifdef _MSC_VER
    // __FUNCSIG__:
    // auto __cdecl enum_info::details::enum_type_name<enum main::B>(void) noexcept
    constexpr std::size_t prefix = sizeof("auto __cdecl enum_info::details::enum_type_name<enum ") - 1;
    constexpr std::size_t suffix = sizeof(">(void) noexcept") - 1;
    constexpr string_view name{__FUNCSIG__ + prefix, sizeof(__FUNCSIG__) - prefix - suffix - 1};

#else
    static_assert(false, "not support!");

#endif

    return name;
}

template <typename E_Type, E_Type v>
constexpr auto enum_value_name() noexcept
{
    // 静态断言, E_Type 必须是枚举类型
    static_assert(std::is_enum_v<E_Type>, " requires E_Type == enum");
#ifdef _MSC_VER
    // __FUNCSIG__:
    // auto __cdecl enum_info::details::enum_type_name<enum main::B>(void) noexcept
    constexpr std::size_t prefix = sizeof("auto __cdecl enum_info::details::enum_type_name<enum ") - 1;
    constexpr std::size_t suffix = sizeof(">(void) noexcept") - 1;
    constexpr string_view full_name{__FUNCSIG__ + prefix + 1, sizeof(__FUNCSIG__) - prefix - suffix - 1 - 1};
    constexpr std::size_t _split_index = full_name.find_first_of(',');
    constexpr string_view enum_name{full_name.begin(), full_name.begin() + _split_index};
    constexpr string_view name{full_name.begin() + _split_index + 1, full_name.end()};

#else
    static_assert(false, "not support!");

#endif

    return name;
}
}   // namespace details


template <typename E_Type, E_Type v>
struct EnumItemInfo
{
    static constexpr size_t           value         = (size_t)(v);
    static constexpr std::string_view _full_name    = details::enum_value_name<E_Type, v>();   // ->  eunum::xyz
    static constexpr size_t           _split_index0 = _full_name.find_last_of(':');
    static constexpr std::string_view item_name     = {_full_name.begin() + _split_index0 + 1, _full_name.end()};   // -> xyz
    static constexpr size_t           _split_index1 =
        _full_name.find_last_of(':', _split_index0 - 2) == _full_name.npos ? -1 : _full_name.find_last_of(':', _split_index0 - 2);
    static constexpr std::string_view enum_name = {_full_name.begin() + _split_index1 + 1,
                                                   _full_name.begin() + _split_index0 - 1};   // -> enum
};

template <typename E_Type>
struct EnumTypeInfo
{
    static constexpr std::string_view _full_name    = details::enum_type_name<E_Type>();   // ->  eunum::xyz
    static constexpr size_t           _split_index0 = _full_name.find_last_of(':');
    static constexpr std::string_view enum_name     = {_full_name.begin() + _split_index0 + 1, _full_name.end()};   // -> xyz
};
}   // namespace SP::refl

#endif   // !ENUM_INFO_HPP
