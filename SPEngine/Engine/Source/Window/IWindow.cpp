#include "Window/IWindow.h"
#include "Window/Win32Window.h"



namespace SP {
IWindow* IWindow::Create(const WindowProps& props /* = WindowProps() */)
{
    return new Win32Window(props);
}
}   // namespace SP