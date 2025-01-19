#pragma once

#include "Event/ApplicationEvent.h"
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

namespace SP {

class Layer;
class Engine;
class IWindow;

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

    const IWindow* GetWindow() const { return m_window.get(); }

    static Application* Get();

private:
    bool OnWindowResize(const WindowResizeEvent& e);

private:
    std::unique_ptr<IWindow> m_window;
    bool                     m_running;
    bool                     m_minimized;
};


Application* CreateApplication();

}   // namespace SP
