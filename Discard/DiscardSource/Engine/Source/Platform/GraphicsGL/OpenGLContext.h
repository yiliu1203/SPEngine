#pragma once

#include "PCH.h"
#include "Graphics/GraphicsContext.h"


struct GLFWwindow;
namespace SP {

class Win32Window;
class GLFWWindow;

namespace RHI {

void InitRendererAPI();
void EnableRendererAPI(Win32Window& _this);
void DisableRendererAPI(Win32Window& _this);
void EnableRendererAPI(GLFWWindow& _this);
void DisableRendererAPI(GLFWWindow& _this);


class OpenGLWin32Context : public GraphicsContext
{
public:
    OpenGLWin32Context(Win32Window* _pWin32Window)
        : m_pWin32Window(_pWin32Window)
    {}
    virtual void Init() override;
    virtual void SwapBuffers() override;

private:
    Win32Window* m_pWin32Window;
};

class OpenGLGLFWContext : public GraphicsContext
{
public:
    OpenGLGLFWContext(GLFWwindow* window_handle);
    virtual void Init() override;
    virtual void SwapBuffers() override;

private:
    GLFWwindow* _WindowHandle;
};
}   // namespace RHI
}   // namespace SP