#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "Platform/Win/GLFWWindow.h"
#include "Platform/Win/Win32Window.h"
#include "glad/glad.h"
#include <Windows.h>


namespace SP::RHI {


#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0X2092
#define WGL_CONTEXT_FLAGS_ARB 0X2094
#define WGL_CONTEXT_COREPROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);

typedef const char*(WINAPI* PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);
typedef int(WINAPI* PFNWGLGETSWAPINTERVALEXTPROC)(void);

void InitRendererAPI()
{
    std::cout << "InitRendererAPI";
}


void EnableRendererAPI(Win32Window& _this)
{
    // get the device context (DC)
    _this.m_hdc = GetDC(_this.m_hWnd);
    //https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdc
    PIXELFORMATDESCRIPTOR pfd;
    // https://www.cplusplus.com/reference/cstring/memset/
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;

    //https://en.wikipedia.org/wiki/Stencil_buffer#:~:text=A%20stencil%20buffer%20is%20an,RAM%20of%20the%20graphics%20hardware.
    pfd.cDepthBits   = 32;
    pfd.cStencilBits = 8;

    int pixelFormat = ChoosePixelFormat(_this.m_hdc, &pfd);
    SetPixelFormat(_this.m_hdc, pixelFormat, &pfd);

    HGLRC tempRC = wglCreateContext(_this.m_hdc);
    wglMakeCurrent(_this.m_hdc, tempRC);
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    const int attribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,
        4,
        WGL_CONTEXT_MINOR_VERSION_ARB,
        6,
        WGL_CONTEXT_FLAGS_ARB,
        0,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_COREPROFILE_BIT_ARB,
        0,
    };

    _this.m_hrc = wglCreateContextAttribsARB(_this.m_hdc, 0, attribList);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempRC);
    wglMakeCurrent(_this.m_hdc, _this.m_hrc);

    if (!gladLoadGL()) {
        std::cout << "Could not initialize GLAD \n";
    }
    else {
        std::cout << "OpenGL Version " << GLVersion.major << std::endl;
    }

    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT =
        (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    bool swapControlSupported = strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") != 0;
    //https://www.khronos.org/opengl/wiki/Swap_Interval
    int vsynch = 0;

    if (swapControlSupported) {
        PFNWGLSWAPINTERVALEXTPROC    wglSwapInternalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT =
            (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
        if (wglSwapInternalEXT(1)) {
            std::cout << "VSynch enabled \n";
        }
        else {
            std::cout << "Could not enable VSynch";
        }
    }
    else {
        std::cout << "WGL_EXT_swap_control not supported \n";
    }
    Global::g_context = new OpenGLWin32Context(&_this);
    Global::g_context->Init();
}
void DisableRendererAPI(Win32Window& _this)
{

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(_this.m_hrc);
    ReleaseDC(_this.m_hWnd, _this.m_hdc);
}

void EnableRendererAPI(GLFWWindow& _this)
{
    Global::g_context = new OpenGLGLFWContext(_this._Window);
    Global::g_context->Init();
}

void DisableRendererAPI(GLFWWindow& _this) {}

OpenGLGLFWContext::OpenGLGLFWContext(GLFWwindow* window_handle)
    : _WindowHandle{window_handle}
{}
void OpenGLGLFWContext::Init()
{
    // useglfw
    glfwMakeContextCurrent(_WindowHandle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SP_TRACE("Failed to initialize GLAD");
    }
    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void OpenGLGLFWContext::SwapBuffers()
{
    glfwSwapBuffers(_WindowHandle);
}
void OpenGLWin32Context::Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void OpenGLWin32Context::SwapBuffers()
{
    ::SwapBuffers(m_pWin32Window->m_hdc);
    if (m_pWin32Window->IsVSync()) glFinish();
}
}   // namespace SP::RHI
