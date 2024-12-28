#include "SPing.h"
#include <iostream>

#include "DockSpaceLayer.h"
#include "EntryPoint.h"
#include "SceneLayer.h"



class ExampleLayer : public SP::Layer
{
public:
    ExampleLayer(const std::string& name) {}
};


class Sandbox : public SP::Application
{
public:
    Sandbox()
    {
        PushLayer(new SP::SceneLayer("scene"));
        // PushLayer(new SP::DockSpaceLayer("dockspace"));
    }
};

SP::Application* SP::CreateApplication()
{
    return new Sandbox();
}
