#include "EntryPoint.h"
#include <iostream>


class Sandbox : public SP::Application
{};



SP::Application* SP::CreateApplication()
{
    return new Sandbox();
}