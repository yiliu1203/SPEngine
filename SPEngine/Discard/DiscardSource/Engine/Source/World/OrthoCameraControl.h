#pragma once

#include "PCH.h"
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Math/mathfwd.h"
#include "Misc/Global.h"
#include "World/Camera.h"

namespace SP {

extern Engine* g_engine;
using namespace math;
using namespace RHI;

class Camera;


struct CameraControlConfig
{
    static constexpr float YAW         = -90.0f;
    static constexpr float PITCH       = 0.0f;
    static constexpr float SPEED       = 0.5f;
    static constexpr float SENSITIVITY = 0.1f;
    static constexpr float ZOOM        = 45.0f;
};

class OrthoCameraControl
{

public:
    // constructor with vectors
    OrthoCameraControl(Camera* camera)
        : m_camera(camera)
    {
        vec4 ortho_value = vec4{-0.5 * Const::WindowWidthInitial,
                                0.5 * Const::WindowWidthInitial,
                                -0.5 * Const::WindowHeightInitial,
                                0.5 * Const::WindowHeightInitial} *
                           0.004;
        m_camera->SetOrtho(ortho_value[0], ortho_value[1], ortho_value[2], ortho_value[3]);
    }
    void OnResize(float width, float height)
    {
        m_aspectRatio = width / height;
        // m_camera->SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
    }

    void OnUpdate(float interval);

    void OnEvent(Event& event);

private:
    void UpdateCamera(vec3 pos_offset, float yew_offset = 0, float pich_offset = 0);

private:
    Camera* m_camera;
    vec3    m_pos_offset;
    float   m_yew_offset;
    float   m_pich_offset;
    float   m_last_scroll_offset;
    float   mLastMouseX   = -1;
    float   mLastMouseY   = -1;
    float   m_aspectRatio = 1;
    float   m_zoomLevel   = 1;
};

}   // namespace SP