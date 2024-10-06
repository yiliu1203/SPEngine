#pragma once
#include "PCH.h"
#include "Event/Event.h"


namespace SP {
class SP_API WindowProps
{
public:
    WindowProps(const std::string& title = "Hello SPing", unsigned int width = Const::WindowWidthInitial,
                unsigned int height = Const::WindowHeightInitial)
        : Title(title)
        , Width(width)
        , Height(height)
    {}

    std::string  Title;
    unsigned int Width;
    unsigned int Height;
};

class SP_API IWindow
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    IWindow(const WindowProps& prop)
    {
        _Data.Width  = prop.Width;
        _Data.Height = prop.Height;
        _Data.Title  = prop.Title;
    }

    virtual ~IWindow() {}

    unsigned int       GetWidth() const { return _Data.Width; };
    unsigned int       GetHeight() const { return _Data.Height; };
    const std::string& GetTitle() const { return _Data.Title; };
    void               SetEventCallback(const EventCallbackFn& callback) { _Data.EventCallback = callback; }
    bool               IsVSync() const { return _Data.VSync; }

    virtual void  SetVSync(bool enabled) = 0;
    virtual int   ProcessMessage() { return 0; };
    virtual void  SwapChains() = 0;
    virtual void  DrawScene() {}
    virtual void* GetNativeWindowHandle() const = 0;
    virtual void* GetWnd() const                = 0;
    virtual void  InitRendererContext() {};
    virtual void  DisableRendererContext() {};

    static IWindow* Create(const WindowProps& props = WindowProps());

protected:
    virtual void OnCloseMsg() {}
    virtual void OnLoseFocus() {}
    virtual void OnKeyPressed(uint8 c, bool is_reapeat) {}
    virtual void OnKeyReleased(uint8 c) {}
    virtual void OnChar(int c) {}
    virtual void OnMouseMoved(int x, int y) {}
    virtual void OnMouseLeftButtonPressed(int x, int y) {}
    virtual void OnMouseLeftButtonReleased(int x, int y) {}
    virtual void OnMouseRightButtonPressed(int x, int y) {}
    virtual void OnMouseRightButtonReleased(int x, int y) {}
    virtual void OnMouseScrolled(int x, int y, int delta) {}


    struct WindowData
    {
        std::string  Title;
        unsigned int Width  = 0;
        unsigned int Height = 0;
        bool         VSync  = false;

        EventCallbackFn EventCallback;
    };

    WindowData _Data;
};



}   // namespace SP
