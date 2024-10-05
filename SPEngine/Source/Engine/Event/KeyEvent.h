#pragma once


#include "PCH.h"
#include "Event.h"

namespace SP {

class SP_API KeyEvent : public Event
{
public:
    inline int GetKeyCode() const { return _KeyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
    KeyEvent(int keyCode)
        : _KeyCode(keyCode)
    {}

    int _KeyCode;
};

class SP_API KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int keyBoard, int repeatCount)
        : KeyEvent(keyBoard)
        , _RepeatCount(repeatCount)
    {}

    inline int GetRepeatCount() const { return _RepeatCount; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent[" << _KeyCode << " (" << _RepeatCount << " repeats)] ";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)

protected:
    int _RepeatCount;
};

class SP_API KeyReleaseedEvent : public KeyEvent
{
public:
    KeyReleaseedEvent(int keyBoard)
        : KeyEvent(keyBoard)
    {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent[" << _KeyCode << "] ";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

}   // namespace SP
