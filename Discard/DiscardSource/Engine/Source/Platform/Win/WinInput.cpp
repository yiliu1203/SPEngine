// #include "WinInput.h"
// #include "Application.h"
// #include "GLFW/glfw3.h"


// namespace SP {

// Input* Input::s_instance = new WinInput();

// bool WinInput::IsKeyPressedImpl(int keycode) const
// {
//     auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindowHandle());
//     auto state  = glfwGetKey(window, keycode);
//     return state == GLFW_PRESS || state == GLFW_REPEAT;
// }

// bool WinInput::IsMouseButtonPressedImpl(int keycode) const
// {
//     auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindowHandle());
//     auto state  = glfwGetMouseButton(window, keycode);
//     return state == GLFW_PRESS || state == GLFW_REPEAT;
// }

// std::pair<float, float> WinInput::GetMousePositionImpl() const
// {
//     auto   window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindowHandle());
//     double xpos, ypos;
//     glfwGetCursorPos(window, &xpos, &ypos);
//     return {(float)xpos, (float)ypos};
// }

// float WinInput::GetMousePositionXImpl() const
// {
//     auto [x, y] = GetMousePositionImpl();
//     return x;
// }

// float WinInput::GetMousePositionYImpl() const
// {
//     auto [x, y] = GetMousePositionImpl();
//     return y;
// }



// }   // namespace SP