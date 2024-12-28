#include "SPing.h"
#include <iostream>

#include "DockSpaceLayer.h"
#include "EntryPoint.h"
#include "SceneLayer.h"


class Sandbox_DX11 : public SP::Application
{
public:
    Sandbox() {}
};

SP::Application* SP::CreateApplication()
{
    return new Sandbox_DX11();
}
