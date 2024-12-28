#include "Global.h"
#include "Window/Window.h"
namespace SP {
Engine*               Global::g_engine  = nullptr;
RHI::GraphicsContext* Global::g_context = nullptr;
Input*                Global::g_input   = nullptr;
Window*               Global::g_window  = nullptr;

// RHI::Device*          Global::g_device  = nullptr;

}   // namespace SP