#pragma once

#include "PCH.h"
#include "Event/Event.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Math/mathfwd.h"
#include "Misc/Global.h"

namespace SP {

extern Engine* g_engine;
using namespace math;
using namespace RHI;
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 0.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
private:
    // camera Attributes
    math::vec3 mPosition;
    math::vec3 mFront;
    math::vec3 mUp;
    math::vec3 mRight;
    math::vec3 mWorldUp;
    // euler Angles
    float mYaw;
    float mPitch;
    // camera options
    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;


    math::mat4 mProjectionOtho;
    math::mat4 mProjectionPerspective;
    math::mat4 mView_matrix;
    math::mat4 mViewprojection;


    E_CameraOrtho mOrthoMode;

private:
    float mLastMouseX = -1;
    float mLastMouseY = -1;

    // Orthogonal


public:
    // constructor with vectors
    Camera(math::vec3 position = math::vec3(0.0f, 0.0f, 0.0f), math::vec3 up = math::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
           float pitch = PITCH)
        : mFront(math::vec3(0.0f, 0.0f, -1.0f))
        , mMovementSpeed(SPEED)
        , mMouseSensitivity(SENSITIVITY)
        , mZoom(ZOOM)
        , mOrthoMode(E_CameraOrtho::None)
    {
        mPosition = position;
        mWorldUp  = up;
        mYaw      = yaw;
        mPitch    = pitch;
        // updateCameraVectors();
        // mProjectionPerspective = mat4::perspective(radians(mZoom), (float)Const::WindowWidth / Const::WindowHeight, 0.1f, 100.0f);
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : mFront(math::vec3(0.0f, 0.0f, -1.0f))
        , mMovementSpeed(SPEED)
        , mMouseSensitivity(SENSITIVITY)
        , mZoom(ZOOM)
        , mOrthoMode(E_CameraOrtho::None)
    {
        mPosition = math::vec3(posX, posY, posZ);
        mWorldUp  = math::vec3(upX, upY, upZ);
        mYaw      = yaw;
        mPitch    = pitch;
        // updateCameraVectors();
    }

    void SetOrtho(float left, float right, float bottom, float top)
    {
        mOrthoMode      = E_CameraOrtho::Orthogonal;
        mProjectionOtho = mat4::ortho(left, right, bottom, top, Const::Near, Const::Far);
        CalcuteViewProjectionMatrix();
    }

    void SetPerspective()
    {
        mOrthoMode = E_CameraOrtho::Perspective;
        mProjectionPerspective =
            mat4::perspective(mZoom, (float)Const::WindowWidthInitial / Const::WindowHeightInitial, 0.1f, 500.0f);
    }

    void SetPosition(math::vec3 position)
    {
        mPosition = position;
        CalcuteViewProjectionMatrix();
    }

    void OnUpdate(float interval)
    {
        // vec3 dir = {0, 0, 0};

        // if (Input::IsKeyPressed(KeyCode::KEY_A)) {
        //     dir[0] = -1;
        // }
        // if (Input::IsKeyPressed(KeyCode::KEY_D)) {
        //     dir[0] = 1;
        // }

        // if (this->mOrthoMode == E_CameraOrtho::Perspective) {
        //     if (Input::IsKeyPressed(KeyCode::KEY_W)) {
        //         dir[2] = 1;
        //     }

        //     if (Input::IsKeyPressed(KeyCode::KEY_S)) {
        //         dir[2] = -1;
        //     }
        // }
        // else if (this->mOrthoMode == E_CameraOrtho::Orthogonal) {
        //     if (Input::IsKeyPressed(KeyCode::KEY_W)) {
        //         dir[1] = 1;
        //     }

        //     if (Input::IsKeyPressed(KeyCode::KEY_S)) {
        //         dir[1] = -1;
        //     }
        // }

        // float velocity = mMovementSpeed * interval;
        // mPosition += velocity * dir * (mRight + mFront + mUp);

        // if (mLastMouseX > 0 && mLastMouseY > 0) {
        //     auto cur_mouse_pos = Input::GetMousePosition();
        //     ProcessMouseMovement(cur_mouse_pos.first - mLastMouseX, cur_mouse_pos.second - mLastMouseY);
        //     mLastMouseX = cur_mouse_pos.first;
        //     mLastMouseY = cur_mouse_pos.second;
        // }

        // CalcuteViewProjectionMatrix();
    }

    void OnEvent(Event& event)
    {
        // EventDispatcher eventDisPatcher(event);
        // eventDisPatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e) -> bool {
        //     if (e.GetMouseButton() == KeyCode::MOUSE_BUTTON_RIGHT) {
        //         auto mouse_pos = Input::GetMousePosition();
        //         mLastMouseX    = mouse_pos.first;
        //         mLastMouseY    = mouse_pos.second;
        //     }
        //     return true;
        // });

        // eventDisPatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e) -> bool {
        //     if (e.GetMouseButton() == KeyCode::MOUSE_BUTTON_RIGHT) {
        //         mLastMouseX = 0;
        //         mLastMouseY = 0;
        //     }
        //     return true;
        // });
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    math::mat4 GetViewMatrix() { return mat4::lookAt(mPosition, mPosition + mFront, mUp); }
    math::mat4 GetProjectionMatrix()
    {
        if (mOrthoMode == E_CameraOrtho::Orthogonal) {
            return mProjectionPerspective;
        }
        else if (mOrthoMode == E_CameraOrtho::Perspective) {
            return mProjectionPerspective;
        }
        return mat4{1.0f};
    }


    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = mMovementSpeed * deltaTime;
        if (direction == FORWARD) mPosition += mFront * velocity;
        if (direction == BACKWARD) mPosition -= mFront * velocity;
        if (direction == LEFT) mPosition -= mRight * velocity;
        if (direction == RIGHT) mPosition += mRight * velocity;
        if (direction == UP) mPosition += mUp * velocity;
        if (direction == DOWN) mPosition -= mUp * velocity;
        // CalcuteViewProjectionMatrix();
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= mMouseSensitivity;
        yoffset *= mMouseSensitivity;

        mYaw += xoffset;
        mPitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (mPitch > 89.0f) mPitch = 89.0f;
            if (mPitch < -89.0f) mPitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        // updateCameraVectors();
        // CalcuteViewProjectionMatrix();
    }

    mat4 CalcuteViewProjectionMatrix()
    {
        updateCameraVectors();
        mat4 view_matrix = GetViewMatrix();
        if (mOrthoMode == E_CameraOrtho::Orthogonal) {
            // 因为这里改的是相机姿态矩阵，作用到顶点上还需要取逆
            mViewprojection = mProjectionOtho * matrix::inverse(view_matrix);
        }
        else if (mOrthoMode == E_CameraOrtho::Perspective) {
            mViewprojection = mProjectionPerspective * matrix::inverse(view_matrix);
        }

        return mViewprojection;
    }

    const mat4& GetViewProjection() const { return mViewprojection; }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        mZoom -= (float)yoffset;
        if (mZoom < 1.0f) mZoom = 1.0f;
        if (mZoom > 45.0f) mZoom = 45.0f;
    }

    void ApplyTransformation(vec3 pos_offset, vec3 rot)
    {
        mPosition += pos_offset;
        mYaw += rot.x;
        mPitch += rot.y;
        CalcuteViewProjectionMatrix();
    }
    vec3 GetRightDir() { return mRight; }
    vec3 GetFrontDir() { return mFront; }
    vec3 GetUpDir() { return mUp; }




private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        math::vec3 front;
        front.x = cos(math::radians(mYaw)) * cos(math::radians(mPitch));
        front.y = sin(math::radians(mPitch));
        front.z = sin(math::radians(mYaw)) * cos(math::radians(mPitch));
        mFront  = normalize(front);
        mRight  = normalize(cross(
            mFront,
            mWorldUp));   // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        mUp     = normalize(cross(mRight, mFront));
    }
};

}   // namespace SP