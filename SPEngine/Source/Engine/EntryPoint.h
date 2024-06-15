#pragma once

#include "PCH.h"
#include "Application.h"

extern SP::Application* SP::CreateApplication();

int main()
{

    SP::Application* app = SP::CreateApplication();
    app->Run();
    delete app;
    return 0;
}