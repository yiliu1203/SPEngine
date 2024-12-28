#include "test1.h"
#include <rttr/registration>


using namespace rttr;

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<MyStruct1>("MyStruct")
        .constructor<>()
        .property("data", &MyStruct1::data)
        .method("func", &MyStruct1::func);


    // registration::class_<node>("node")
    //     .constructor<std::string, node*>()(
    //         policy::ctor::as_std_shared_ptr,   // should create an instance of the class as shared_ptr<node>
    //         default_arguments(nullptr)         // second argument is optional, so we provide the default value for it
    //         )
    //     .property("name", &node::get_name, &node::set_name)(
    //         metadata("TOOL_TIP", "Set the name of node.")   // stores metadata associated with this property
    //         )
    //     .property("m_isvisible", &node::m_isvisible);
    // register directly a member object pointer; mark it as 'private'
    // .property("parent", &node::m_parent, registration::private_access)
    // .property_readonly("children", &node::get_children)   // a read-only property; so not set possible
    // .method("set_visible", &node::set_visible)(
    //     default_arguments(true),                          // the default value for 'cascade'
    //     parameter_names("visible",
    //                     "cascade")   // provide the names of the parameter; optional, but might be useful for clients
    //     )
    // .method("render", &node::render);
    // ;
};
