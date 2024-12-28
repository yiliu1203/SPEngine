#pragma once

#include "PCH.h"
#include "Event/Event.h"



namespace SP {

class Application;
class RenderScene;
class Camera;
class OrthoCameraControl;
namespace RHI {
class GraphicsContext;
}

class SP_API Engine
{
    friend class Application;

public:
    ~Engine();
    bool                 Initialize();
    void                 RunFrame();
    void                 OnEvent(Event& event);
    float                GetCurrentAbsStamp() { return _current_stamp; }
    float                GetLastAbsStamp() { return _last_stamp; }
    float                GetDeltaTime() { return _delta_time; }
    inline const Camera* GetMainCamera() { return _maincamera; }

private:
    Engine();
    RenderScene*        _scene;
    Camera*             _maincamera;
    OrthoCameraControl* m_ortho_camera_control;


    float _current_stamp;
    float _last_stamp;
    float _delta_time;
};

}   // namespace SP