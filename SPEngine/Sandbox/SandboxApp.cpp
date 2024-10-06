#include "Application.h"
#include <iostream>


class Sandbox : public SP::Application
{};

int main()
{
    Sandbox sandbox;
    sandbox.Init();
    sandbox.Run();
}
