#include "Win32Window.h"
#include <Windows.h>
// #include <wchar.h>

namespace SP {


typedef struct POINTS
{
    SHORT x;
    SHORT y;
} POINTS;
#define MAKEPOINTS_FROM_PARAM(x) *(POINTS*)&(x)

Win32Window::WindowClass Win32Window::WindowClass::wndClass;

Win32Window::WindowClass::WindowClass()
    : hInstance(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc    = {sizeof(WNDCLASSEX)};   // cbsize 必须被设置为 sizeof(WNDCLASSEX)
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hbrBackground = nullptr;
    wc.hCursor       = nullptr;
    wc.hIcon         = nullptr;
    wc.hInstance     = GetInstance();    // 应用程序实例的句柄
    wc.lpfnWndProc   = HandleMsgSetup;   // 消息处理函数
    wc.lpszClassName = GetName();        // 窗口类的名字， 上面三个属性最重要了
    wc.hCursor       = nullptr;
    wc.style         = CS_OWNDC;
    //wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    RegisterClassEx(&wc);
}

Win32Window::WindowClass::~WindowClass()
{
    UnregisterClass(GetName(), GetInstance());
}

Win32Window::Win32Window(const WindowProps& prop)
    : IWindow{prop}
{
    RECT wr;
    wr.left   = 100;
    wr.right  = GetWidth() + wr.left;
    wr.top    = 100;
    wr.bottom = GetHeight() + wr.top;

    // window 大小不包括标题、menu等
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    mHwnd = CreateWindowEx(0,
                           WindowClass::GetName(),
                           GetTitle().c_str(),
                           WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
                           200,
                           200,
                           wr.right - wr.left,
                           wr.bottom - wr.top,
                           nullptr,
                           nullptr,
                           WindowClass::GetInstance(),
                           this);

    ShowWindow(mHwnd, SW_SHOWDEFAULT);
    SetWindowText(mHwnd, GetTitle().c_str());
    // EnableOpenGL();
    InitRendererContext();
}

Win32Window::~Win32Window() {}

void Win32Window::SetTitle(const char* title) const
{
    SetWindowTextA(mHwnd, title);
}

int Win32Window::ProcessMessage()
{
    MSG msg = {0};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))   // PeekMessage 不会阻塞, 这个有bug ？？ 没有收到 WM_QUIT
    {
        int res = GetMessage(&msg, NULL, 0, 0);
        if (res) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            break;
        }
    }
    if (msg.message == WM_QUIT) {
        DisableRendererContext();
        return -1;
    }
    return 1;
}

void Win32Window::SwapChains()
{
    // SwapBuffers(m_hdc);
    // if (IsVSync()) glFinish();
    // Global::g_context->SwapBuffers();
}


LRESULT CALLBACK Win32Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)   // 第一个Message, 在这里建立 hwnd 和 this 的联系。 else 里面拿到this指针
    {
        const CREATESTRUCT* const pCreate  = reinterpret_cast<CREATESTRUCT*>(lParam);
        Win32Window* const        pThisWnd = static_cast<Win32Window*>(pCreate->lpCreateParams);
        // pThisWnd pointer was store inside window userdata。 and then we can got it on message process
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThisWnd));
        // Global::g_input->OnWindowCreated(pThisWnd->GetWidth(), pThisWnd->GetHeight());
        return pThisWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    else {
        Win32Window* const pWnd = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Win32Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg) {
    case WM_CLOSE:
    case WM_DESTROY:
    {
        this->OnCloseMsg();
        PostQuitMessage(0);
        break;
    }
    case WM_KILLFOCUS:
    {
        this->OnLoseFocus();
        break;
    }

    case WM_KEYDOWN:
    {
        // The previous key state. The value is 1 if the key is down before the message is sent, or it is zero if the key is up.
        bool isReapeat = lParam & 0x40000000;
        this->OnKeyPressed(static_cast<int>(wParam), isReapeat);
        break;
    }
    case WM_KEYUP: this->OnKeyReleased(static_cast<int>(wParam)); break;
    case WM_CHAR: this->OnChar(static_cast<int>(wParam)); break;
    case WM_MOUSEMOVE:
    {
        // todo when mouse drag outof window
        const POINTS pt = MAKEPOINTS_FROM_PARAM(lParam);
        this->OnMouseMoved(pt.x, pt.y);
        break;
    }

    case WM_LBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS_FROM_PARAM(lParam);
        this->OnMouseLeftButtonPressed(pt.x, pt.y);
        break;
    }

    case WM_RBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS_FROM_PARAM(lParam);
        this->OnMouseRightButtonPressed(pt.x, pt.y);
        break;
    }

    case WM_LBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS_FROM_PARAM(lParam);
        this->OnMouseLeftButtonPressed(pt.x, pt.y);
        break;
    }

    case WM_RBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS_FROM_PARAM(lParam);
        this->OnMouseRightButtonPressed(pt.x, pt.y);
        break;
    }

    case WM_MOUSEWHEEL:
    {
        const POINTS pt    = MAKEPOINTS_FROM_PARAM(lParam);
        const int    delta = GET_WHEEL_DELTA_WPARAM(wParam);
        this->OnMouseScrolled(pt.x, pt.y, delta);
        break;
    }

    default: break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}



}   // namespace SP
