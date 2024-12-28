
#include "enum_info.hpp"
#include <iostream>
#include <string>
#include <string_view>



using namespace std;

enum class my_enum
{
    none = 0,
};

template <typename E_Type, E_Type v>
struct EnumValueInfo2
{
    static constexpr string_view full_name = {"aaa"};
};

int main(void)
{
    std::cout << enum_info::enum_class_name<my_enum> << std::endl;

    std::cout << enum_info::enum_value_name<my_enum, my_enum::none> << std::endl;
    std::cout << enum_info::EnumValueInfo<my_enum, my_enum::none>::full_name << std::endl;
    std::cout << enum_info::EnumValueInfo<my_enum, my_enum::none>::enum_name << std::endl;
    std::cout << enum_info::EnumValueInfo<my_enum, my_enum::none>::value_name << std::endl;
    std::cout << enum_info::EnumValueInfo<my_enum, my_enum::none>::value << std::endl;



    return 0;
}