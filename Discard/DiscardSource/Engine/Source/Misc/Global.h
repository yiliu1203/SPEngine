#pragma once
#include "Engine.h"
#include "Graphics/Device.h"
#include "Graphics/GraphicsContext.h"
#include "Misc/Input.h"




namespace SP {
class Window;
struct Global
{
    static Engine*               g_engine;
    static RHI::GraphicsContext* g_context;
    static Input*                g_input;
    static Window*               g_window;
    // static RHI::Device*          g_device;
};



// extern Engine* g_engine;

}   // namespace SP