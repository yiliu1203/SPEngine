#pragma once
#include "PCH.h"

namespace SP {
class Application
{
public:
    Application();
    void Run();
};

Application* CreateApplication();

}   // namespace SP