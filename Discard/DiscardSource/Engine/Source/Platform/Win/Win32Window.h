#pragma once
#include "PCH.h"
#include "Graphics/RHI.h"
#include "Window/Window.h"

namespace SP {
class Win32Window;

namespace RHI {
extern void EnableRendererAPI(Win32Window& _this);
extern void DisableRendererAPI(Win32Window& _this);
class OpenGLWin32Context;
}   // namespace RHI


class SP_API Win32Window : public Window
{
public:
    friend void RHI::EnableRendererAPI(Win32Window& _this);
    friend void RHI::DisableRendererAPI(Win32Window& _this);
    friend class RHI::OpenGLWin32Context;
    Win32Window(const WindowProps&);
    ~Win32Window();
    void SetTitle(const char* title) const;

    virtual void  SetVSync(bool enabled) override {}
    virtual int   ProcessMessage();
    virtual void  SwapChains();
    virtual void  DrawScene() {}
    virtual void* GetNativeWindowHandle() const { return (void*)this; };
    virtual void* GetWnd() const { return (void*)(&this->mHwnd); }
    virtual void  InitRendererContext() override { RHI::EnableRendererAPI(*this); }
    virtual void  DisableRendererContext() override { RHI::DisableRendererAPI(*this); }


private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT                 HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


    HWND  mHwnd;
    HDC   mHdc;
    HGLRC mHrc;

private:
    class WindowClass
    {
    public:
        static const char*     GetName() noexcept { return wndClassName; }
        static const HINSTANCE GetInstance() noexcept { return wndClass.hInstance; };

    private:
        WindowClass();
        ~WindowClass();
        static constexpr const char* wndClassName = "LD3D";
        static WindowClass           wndClass;
        HINSTANCE                    hInstance;
    };
};
}   // namespace SP


// tips: noexcept, delete