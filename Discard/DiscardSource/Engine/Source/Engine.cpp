#include "Engine.h"
#include "Misc/Global.h"
#include "World/Camera.h"
#include "World/OrthoCameraControl.h"
#include "World/RenderScene.h"


namespace SP {


Engine::Engine()
{
    auto now   = std::chrono::high_resolution_clock::now();
    auto msecs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    _current_stamp = msecs.count() / 1000.0;
    _last_stamp    = _current_stamp;
    _delta_time    = 0;

    Global::g_engine       = this;
    _maincamera            = new Camera();
    m_ortho_camera_control = new OrthoCameraControl(_maincamera);
    _scene                 = new RenderScene(_maincamera);
}

Engine::~Engine()
{
    delete _scene;
}

bool Engine::Initialize()
{
    return true;
    _scene->Init();
    return true;
}


void Engine::RunFrame()
{
    auto  now             = std::chrono::high_resolution_clock::now();
    auto  msecs           = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    float cursecond_stamp = msecs.count() / 1000.0;
    _delta_time           = cursecond_stamp - _last_stamp;
    _last_stamp           = _current_stamp;
    _current_stamp        = cursecond_stamp;
    // _maincamera->OnUpdate(_delta_time);
    m_ortho_camera_control->OnUpdate(_delta_time);
    _scene->DrawFrame();
}

void Engine::OnEvent(Event& event)
{
    // _maincamera->OnEvent(event);
    if (m_ortho_camera_control) {
        m_ortho_camera_control->OnEvent(event);
    }
}



}   // namespace SP