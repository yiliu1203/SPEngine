
#include "enum_info.hpp"
#include <iostream>
#include <string>
#include <string_view>



using namespace std;

enum class my_enum
{
    none = 0,
};


int main(void)
{
    std::cout << enum_info::enum_class_name<my_enum> << std::endl;   // --> my_enum

    std::cout << enum_info::enum_value_name<my_enum, my_enum::none> << std::endl;        // --> my_enum::none
    std::cout << enum_info::EnumInfo<my_enum, my_enum::none>::full_name << std::endl;    // --> my_enum::none
    std::cout << enum_info::EnumInfo<my_enum, my_enum::none>::enum_name << std::endl;    // --> my_enum
    std::cout << enum_info::EnumInfo<my_enum, my_enum::none>::value_name << std::endl;   // --> none
    std::cout << enum_info::EnumInfo<my_enum, my_enum::none>::value << std::endl;        // --> 0

    return 0;
}