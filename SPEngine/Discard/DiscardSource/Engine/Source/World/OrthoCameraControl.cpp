#include "OrthoCameraControl.h"
#include "Camera.h"
namespace SP {

void OrthoCameraControl::OnEvent(Event& event)
{
    EventDispatcher eventDisPatcher(event);
    eventDisPatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e) -> bool {
        if (e.GetMouseButton() == KeyCode::MOUSE_BUTTON_RIGHT) {
            auto mouse_pos = Input::GetMousePosition();
            mLastMouseX    = mouse_pos.first;
            mLastMouseY    = mouse_pos.second;
        }
        return true;
    });

    eventDisPatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e) -> bool {
        if (e.GetMouseButton() == KeyCode::MOUSE_BUTTON_RIGHT) {
            mLastMouseX = 0;
            mLastMouseY = 0;
        }
        return true;
    });

    eventDisPatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) -> bool {
        m_last_scroll_offset = e.GetYOffset();
        return true;
    });
}

void OrthoCameraControl::OnUpdate(float interval)
{
    vec3 dir = {0, 0, 0};

    if (Input::IsKeyPressed(KeyCode::KEY_A)) {
        dir[0] = -1;
    }
    if (Input::IsKeyPressed(KeyCode::KEY_D)) {
        dir[0] = 1;
    }
    if (Input::IsKeyPressed(KeyCode::KEY_W)) {
        dir[1] = 1;
    }
    if (Input::IsKeyPressed(KeyCode::KEY_S)) {
        dir[1] = -1;
    }

    dir[2] = m_last_scroll_offset * 20;

    float velocity = CameraControlConfig::SPEED * interval;
    vec3  pos_offset =
        velocity * (dir[0] * m_camera->GetRightDir() + dir[1] * m_camera->GetUpDir() + dir[2] * m_camera->GetFrontDir());
    float offset_x = 0;
    float offset_y = 0;

    if (mLastMouseX > 0 && mLastMouseY > 0) {
        auto cur_mouse_pos = Input::GetMousePosition();
        offset_x           = cur_mouse_pos.first - mLastMouseX;
        offset_y           = cur_mouse_pos.second - mLastMouseY;
        mLastMouseX        = cur_mouse_pos.first;
        mLastMouseY        = cur_mouse_pos.second;
    }
    // update camera
    UpdateCamera(pos_offset, offset_x * CameraControlConfig::SENSITIVITY, offset_y * CameraControlConfig::SENSITIVITY);
    m_last_scroll_offset = 0;
}

void OrthoCameraControl::UpdateCamera(vec3 pos_offset, float yew_offset, float pich_offset)
{
    m_camera->ApplyTransformation(pos_offset, vec3{yew_offset, pich_offset, 0});
}

}   // namespace SP