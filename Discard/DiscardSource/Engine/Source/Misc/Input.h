#pragma once

#include "PCH.h"
#include "Core/Delegate.h"


namespace SP {


template <typename T>
class Delegate2
{};

class Event;
class Application;
class Win32Window;


class SP_API Input
{
    friend class Application;
    friend class Win32Window;

public:
    using EventDelegateType = Delegate<bool, Event&>;
    using EventHandleType   = typename EventDelegateType::Handler;
    inline static bool IsKeyPressed(uint32 keycode) { return s_instance->IsKeyPressedImpl(keycode); }
    inline static bool IsMouseButtonPressed(uint32 keycode) { return s_instance->IsMouseButtonPressedImpl(keycode); }
    inline static std::pair<int, int> GetMousePosition() { return s_instance->GetMousePositionImpl(); }
    inline static int                 GetMousePositionX() { return s_instance->GetMousePositionXImpl(); }
    inline static int                 GetMousePositionY() { return s_instance->GetMousePositionYImpl(); }
    inline static Input* Instance() { return Input::s_instance; }


    void AddEventListener(EventHandleType handle);
    void RemoveEventListener(EventHandleType handle);

protected:
    virtual bool                IsKeyPressedImpl(uint32 keycode) const { return m_keystate[keycode]; };
    virtual bool                IsMouseButtonPressedImpl(uint32 keycode) const { return m_keystate[KeyCode::MOUSE_BUTTON_1]; };
    virtual bool                IsMouseInWindow() { return true; };
    virtual std::pair<int, int> GetMousePositionImpl() const { return {m_mouseX, m_mouseY}; };
    virtual int                 GetMousePositionXImpl() const { return m_mouseX; };
    virtual int                 GetMousePositionYImpl() const { return m_mouseY; };


private:
    // Mouse Event
    void OnMouseLeftButtonPressed(int x, int y);
    void OnMouseLeftButtonLReleased(int x, int y);
    void OnMouseRightButtonPressed(int x, int y);
    void OnMouseRightButtonReleased(int x, int y);
    void OnMouseMoved(int x, int y);
    void OnMouseScrolled(int x, int y, int delta);
    void OnLoseFocus();

    // KeyBoard Event
    void OnKeyPressed(int key, bool isRepeat);
    void OnKeyReleased(int key);
    // for text input
    void OnChar(int key);

    // Special Input
    void OnCloseMsg();
    void OnWindowCreated(int width, int height);

    void DispatchEvent(const Event& event);

    template <typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;

private:
    static Input*    s_instance;
    std::bitset<512> m_keystate;
    std::queue<char> m_charbuffer;
    int              m_mouseX;
    int              m_mouseY;

    int m_windowWidth;
    int m_windowHeight;

    static constexpr unsigned int bufferSize = 16u;

    EventDelegateType m_DelegateDispacher;
};
}   // namespace SP