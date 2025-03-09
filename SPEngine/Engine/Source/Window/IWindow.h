#pragma once
#include "PCH.h"
#include "Event/Event.h"

namespace SP {


struct WindowProps
{
    using EventCallbackFn = std::function<void(void*)>;
    WindowProps(const std::string& title = "Hello SPing", uint32 width = Const::WindowWidthInitial, uint32 height = Const::WindowHeightInitial, EventCallbackFn callback = nullptr)
        : Title(title)
        , Width(width)
        , Height(height)
        , EventCallback(callback)
    {}

    std::string     Title;
    uint32          Width;
    uint32          Height;
    EventCallbackFn EventCallback;
};


struct WindowEventInfo
{
    void*  handle = nullptr;
    uint32 msg    = 0;
    uint64 wparam = 0;
    int64  lparam = 0;
    float  width  = 0.0f;
    float  height = 0.0f;
};


class SP_API IWindow
{
public:
    IWindow(const WindowProps& prop)
        : m_init_desc{prop}
    {}
    // using EventCallbackFn = WindowProps::EventCallbackFn;

    virtual ~IWindow() {}

    uint32             GetWidth() const { return m_init_desc.Width; };
    uint32             GetHeight() const { return m_init_desc.Height; };
    const std::string& GetTitle() const { return m_init_desc.Title; };
    virtual int        ProcessMessage() { return 0; };
    virtual void*      GetWnd() const = 0;
    // virtual void  InitRendererContext() {};
    // virtual void  DisableRendererContext() {};

    static IWindow* Create(const WindowProps& props = WindowProps());
    void            Quit(int32 exit_code = 0) const {}

protected:
    virtual void OnCloseMsg() {}
    // virtual void OnLoseFocus() {}
    // virtual void OnKeyPressed(uint8 c, bool is_reapeat) {}
    // virtual void OnKeyReleased(uint8 c) {}
    // virtual void OnChar(int c) {}
    // virtual void OnMouseMoved(int x, int y) {}
    // virtual void OnMouseLeftButtonPressed(int x, int y) {}
    // virtual void OnMouseLeftButtonReleased(int x, int y) {}
    // virtual void OnMouseRightButtonPressed(int x, int y) {}
    // virtual void OnMouseRightButtonReleased(int x, int y) {}
    // virtual void OnMouseScrolled(int x, int y, int delta) {}
    void BroadcastEvent(void* event_data)
    {

        if (m_init_desc.EventCallback) {
            m_init_desc.EventCallback(event_data);
        }
    }

    WindowProps m_init_desc;
};



}   // namespace SP
