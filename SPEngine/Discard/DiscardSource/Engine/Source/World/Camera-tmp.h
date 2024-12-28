#pragma once
#include "PCH.h"
#include "Math/mathfwd.h"

namespace SP {
using namespace RHI;
using namespace math;
class Event;
class Camera
{
public:
    Camera(float left, float right, float bottom, float top);
    void SetRotation(float angle);

    void SetPosition(vec3 position);

    void OnEvent(Event& event);

    const mat4& GetViewProjection() const { return m_viewprojection; }
    mat4&       GetViewProjection() { return m_viewprojection; }

private:
    mat4          CalcuteViewProjectionMatrix();
    E_CameraOrtho mOrthoMode;
    math::mat4    m_projection;
    math::mat4    m_view_matrix;
    math::mat4    m_viewprojection;
    math::vec3    m_position;
    float         m_rotation;
};
}   // namespace SP
