#pragma once
#include "PCH.h"
#include "Event/Event.h"
#include "Graphics/GraphicsContext.h"


namespace SP {
using namespace RHI;
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

class SP_API Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    Window(const WindowProps& prop)
    {
        _Data.Width  = prop.Width;
        _Data.Height = prop.Height;
        _Data.Title  = prop.Title;
    }

    virtual ~Window() {}

    unsigned int       GetWidth() const { return _Data.Width; };
    unsigned int       GetHeight() const { return _Data.Height; };
    const std::string& GetTitle() const { return _Data.Title; };
    void               SetEventCallback(const EventCallbackFn& callback) { _Data.EventCallback = callback; }
    bool               IsVSync() const { return _Data.VSync; }

    virtual void  SetVSync(bool enabled) = 0;
    virtual int   ProcessMessage()       = 0;
    virtual void  SwapChains()           = 0;
    virtual void  DrawScene() {}
    virtual void* GetNativeWindowHandle() const = 0;
    virtual void* GetWnd() const                = 0;
    virtual void  InitRendererContext(){};
    virtual void  DisableRendererContext(){};

    static Window* Create(const WindowProps& props = WindowProps());

    // protected:
    //     GraphicsContext* _Context;

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
