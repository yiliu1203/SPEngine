#pragma once

#include "PCH.h"
#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Window/LayerStack.h"
#include "Window/Window.h"


namespace SP {

class Layer;
class Engine;
class SP_API Application
{
public:
    Application();
    virtual ~Application();

    virtual bool Init() { return InitMainWindow() && InitRenderer(); };

    bool InitMainWindow();
    bool InitRenderer();

    void Run();

    void Close();

    bool OnEvent(Event& event);

    void PushLayer(Layer* layer);
    void PushOverLayer(Layer* layer);

    Window* GetWindow() const { return _Window.get(); }

    const Layer* GetLayer(const std::string& layername);

    static Application* Get();

private:
    bool OnWindowResize(const WindowResizeEvent& e);

private:
    ImGuiLayer*             _ImGuiLayer;
    std::unique_ptr<Window> _Window;
    bool                    _IsRunning;
    LayerStack              _LayerStack;
    Engine*                 _engine;
    bool                    _Minimized;
};


Application* CreateApplication();

}   // namespace SP
