#pragma once

#include "PCH.h"
#include "Event.h"



namespace SP {

class SP_API WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : _Width(width)
        , _Height(height)
    {}

    inline unsigned int GetHeight() const { return _Height; }
    inline unsigned int GetWidth() const { return _Width; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent[" << _Width << "," << _Height << "]";
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)

protected:
    unsigned int _Width;
    unsigned int _Height;
};


class SP_API WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() {}
    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class SP_API AppTickEvent : public Event
{
public:
    AppTickEvent() {}
    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class SP_API AppUpdateEvent : public Event
{
public:
    AppUpdateEvent() {}
    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class SP_API AppRenderEvent : public Event
{
public:
    AppRenderEvent() {}
    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}   // namespace SP
