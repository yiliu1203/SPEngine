#include "Input.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"


namespace SP {

Input* Input::s_instance = new Input();

void Input::AddEventListener(EventHandleType handle)
{
    m_DelegateDispacher += handle;
}

void Input::RemoveEventListener(EventHandleType handle)
{
    m_DelegateDispacher -= handle;
}

void Input::OnMouseLeftButtonPressed(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
    MouseButtonPressedEvent e{KeyCode::MOUSE_BUTTON_1};
    m_keystate[KeyCode::MOUSE_BUTTON_1] = true;
    DispatchEvent(e);
}

void Input::OnMouseLeftButtonLReleased(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
    MouseButtonReleasedEvent e{KeyCode::MOUSE_BUTTON_1};
    m_keystate[KeyCode::MOUSE_BUTTON_1] = false;
    DispatchEvent(e);
}

void Input::OnMouseRightButtonPressed(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
    MouseButtonPressedEvent e{KeyCode::MOUSE_BUTTON_2};
    m_keystate[KeyCode::MOUSE_BUTTON_2] = true;
    DispatchEvent(e);
}

void Input::OnMouseRightButtonReleased(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
    MouseButtonReleasedEvent e{KeyCode::MOUSE_BUTTON_2};
    m_keystate[KeyCode::MOUSE_BUTTON_2] = false;
    DispatchEvent(e);
}

void Input::OnMouseMoved(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
    MouseMovedEvent e(x, y);
    DispatchEvent(e);
}

void Input::OnMouseScrolled(int x, int y, int delta)
{

    MouseScrolledEvent e(delta, delta);
    DispatchEvent(e);
}

void Input::OnLoseFocus()
{
    m_keystate.reset();
    while (!m_charbuffer.empty()) {
        m_charbuffer.pop();
    }
}

void Input::OnKeyPressed(int key, bool isRepeat)
{
    m_keystate[size_t(key)] = true;
    KeyPressedEvent e(key, isRepeat ? 1 : 0);
    DispatchEvent(e);
}

void Input::OnKeyReleased(int key)
{
    m_keystate[size_t(key)] = false;
    KeyReleaseedEvent e(key);
    DispatchEvent(e);
    if (e.GetKeyCode() == KeyCode::KEY_ESCAPE) {
        OnCloseMsg();
    }
}

void Input::OnChar(int key)
{
    m_charbuffer.push(char(key));
    TrimBuffer(m_charbuffer);
}

template <typename T>
void Input::TrimBuffer(std::queue<T>& buffer) noexcept
{
    while (buffer.size() > bufferSize) {
        buffer.pop();
    }
}

void Input::OnCloseMsg()
{
    Application::Get()->Close();
}

void Input::OnWindowCreated(int width, int height)
{
    m_windowWidth  = width;
    m_windowHeight = height;
}

void Input::DispatchEvent(const Event& event)
{
    SP_INFO(event);
    SP_INFO("Input: MouseX={0}, MouseY={1}", m_mouseX, m_mouseY);
    m_DelegateDispacher.Execute(const_cast<Event&>(event));
}
}   // namespace SP
