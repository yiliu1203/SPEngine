#include "SPing.h"
#include <iostream>

#include "EditorLayer.h"
#include "EntryPoint.h"

using namespace SP;


class EditorApp : public SP::Application
{
public:
    EditorApp() { PushLayer(new EditorLayer("dockspace")); }
};

SP::Application* SP::CreateApplication()
{
    return new EditorApp();
}
