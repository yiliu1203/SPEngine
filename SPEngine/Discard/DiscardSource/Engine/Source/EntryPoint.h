#pragma once

#include "PCH.h"
#include "Application.h"

class SP::Application;

extern SP::Application* SP::CreateApplication();

int main()
{
    SP::Application* app = SP::CreateApplication();
    if (!app->Init()) {
        return 0;
    }
    app->Run();
    delete app;
    return 0;
}