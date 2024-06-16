#pragma once
#include "PCH.h"

namespace SP {
class SP_API Application
{
public:
    Application();
    virtual void Init();
    virtual void Run();
};

// SP_API Application* CreateApplication();

}   // namespace SP