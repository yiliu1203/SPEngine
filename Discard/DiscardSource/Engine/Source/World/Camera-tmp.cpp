#include "Camera-tmp.h"
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "math/mathfwd.h"

namespace SP {
Camera::Camera(float left, float right, float bottom, float top)
    : mOrthoMode{E_CameraOrtho::Orthogonal}
    , m_projection{mat4::ortho(left, right, bottom, top, Const::Near, Const::Far)}
    , m_view_matrix{1.0f}
    , m_position{0.0f, 0.0f, 0.0f}
    , m_rotation(0.0f)
{
    CalcuteViewProjectionMatrix();
}
void Camera::SetRotation(float angle)
{
    m_rotation = angle / 180 * math::F_PI;
    CalcuteViewProjectionMatrix();
}

void Camera::SetPosition(vec3 position)
{
    m_position = position;
    CalcuteViewProjectionMatrix();
}

void Camera::OnEvent(Event& event)
{
    EventDispatcher eventDisPatcher(event);
    eventDisPatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) -> bool { return true; });
}

mat4 Camera::CalcuteViewProjectionMatrix()
{
    mat4 rot       = mat4::rotation(m_rotation, math::vec3{0.0f, 0.0f, 1.0f});
    mat4 transform = mat4::translation(m_position) * rot;
    // 因为这里改的是相机姿态矩阵，作用到顶点上还需要取逆
    m_view_matrix = matrix::inverse(transform);
    // m_view_matrix  = mat4::lookAt(m_position,
    //                              vec3{
    //                                  0.0f,
    //                                  0.0f,
    //                                  0.0f,
    //                              },
    //                              vec3{1.0f, 1.0f, 0.0f});
    // m_view_matrix    = matrix::inverse(m_view_matrix);
    m_viewprojection = m_projection * m_view_matrix;
    return m_viewprojection;
}
}   // namespace SP