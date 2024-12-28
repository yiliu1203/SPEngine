#pragma once
#include <iostream>
#include <rttr/registration_friend>
#include <rttr/type>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>




struct MyStruct1
{
    MyStruct1(){};
    void func(double){};
    int  data;
};



class node
{
public:
    node(std::string name, node* parent = nullptr);
    virtual ~node();
    void               set_name(const std::string& name);
    const std::string& get_name() const;
    std::vector<node*> get_children() const;
    void               set_visible(bool visible, bool cascade = true);
    virtual void       render();

private:
    node*              m_parent;
    std::string        m_name;
    std::vector<node*> m_children;
    bool               m_isvisible;
    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};