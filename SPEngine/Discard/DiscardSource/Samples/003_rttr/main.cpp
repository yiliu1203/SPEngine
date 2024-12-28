
#include <iostream>
#include <rttr/registration>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>


#include "from_json.h"
#include "test1.h"
#include "to_json.h"


using namespace rttr;

int main()
{
    MyStruct1 ss;
    ss.data     = 10;
    type    t   = type::get<MyStruct1>();
    variant var = t.create();
    t.get_property("data").set_value(var, 3);
    for (auto& prop : t.get_properties())
        std::cout << "name: " << prop.get_name() << "  value: " << t.get_property("data").get_value(var).to_int32() << std::endl;

    for (auto& meth : t.get_methods()) std::cout << "name: " << meth.get_name() << std::endl;
    std::cout << "Hello " << std::endl << std::endl;


    MyStruct1 obj;

    property prop = type::get(obj).get_property("data");
    prop.set_value(obj, 23);

    variant var_prop = prop.get_value(obj);
    std::cout << var_prop.to_int();   // prints '23'


    std::cout << "\n############################################\n" << std::endl;

    std::cout << "Circle c_2:\n" << io::to_json(obj) << std::endl;
}