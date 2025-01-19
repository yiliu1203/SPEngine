#pragma once

#include "PCH.h"
#include "Event.h"


namespace SP {

class SP_API MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : _MouseX(x)
        , _MouseY(y)
    {}

    inline float GetX() const { return _MouseX; }
    inline float GetY() const { return _MouseY; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MounseMoveEvent[" << _MouseX << "," << _MouseY << "]";
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryInput | EvnetCategoryMouse)

protected:
    float _MouseX;
    float _MouseY;
};


class SP_API MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : _XOffset(xOffset)
        , _YOffset(yOffset)
    {}
    inline float GetXOffset() const { return _XOffset; }
    inline float GetYOffset() const { return _YOffset; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseScrollEvent[" << _XOffset << "," << _YOffset << "]";
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScolled)
    EVENT_CLASS_CATEGORY(EventCategoryInput | EvnetCategoryMouse)

protected:
    float _XOffset;
    float _YOffset;
};

class SP_API MouseButtonEvent : public Event
{
public:
    inline int GetMouseButton() const { return _Button; }
    EVENT_CLASS_CATEGORY(EvnetCategoryMouse | EventCategoryInput)
protected:
    MouseButtonEvent(int mButton)
        : _Button(mButton)
    {}
    int _Button;

private:
};

class SP_API MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button)
        : MouseButtonEvent(button)
    {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPRessedEvent[" << _Button << "] ";
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};


class SP_API MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button)
        : MouseButtonEvent(button)
    {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent[" << _Button << "] ";
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};





}   // namespace SP
