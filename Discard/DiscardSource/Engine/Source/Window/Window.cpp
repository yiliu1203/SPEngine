#include "Window/Window.h"
#include "Platform/Win/GLFWWindow.h"
#include "Platform/Win/Win32Window.h"



namespace SP {
Window* Window::Create(const WindowProps& props /* = WindowProps() */)
{
#ifdef USE_GLFW
    return new GLFWWindow(props);   // ????
#else
    return new Win32Window(props);
#endif
}
}   // namespace SP