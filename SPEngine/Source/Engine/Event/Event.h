#pragma once

#include "PCH.h"
namespace SP {




enum EventType
{
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScolled,
    EventTypeEnd,
};

enum EventCategory
{
    None_                    = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput       = BIT(1),
    EventCategoryKeyboard    = BIT(2),
    EvnetCategoryMouse       = BIT(3),
    EventCategoryMouseButton = BIT(4),


};

constexpr int EventCategoryMap[/*EventTypeEnd*/] = {None_,
                                                    EventCategoryApplication,
                                                    EventCategoryApplication,
                                                    EventCategoryApplication,
                                                    EventCategoryApplication,
                                                    EventCategoryApplication,
                                                    EventCategoryApplication,
                                                    EventCategoryApplication,                      /* AppUpdate **/
                                                    EventCategoryApplication,
                                                    EventCategoryInput | EventCategoryKeyboard,    /** KeyPressed **/
                                                    EventCategoryInput | EventCategoryKeyboard,
                                                    EventCategoryInput | EventCategoryMouseButton, /**MouseButtonPressed**/
                                                    EventCategoryInput | EventCategoryMouseButton, /**MouseButtonPressed**/
                                                    EventCategoryInput | EvnetCategoryMouse,
                                                    EventCategoryInput | EvnetCategoryMouse};

static_assert(EventTypeEnd * sizeof(int) == sizeof EventCategoryMap);   // 确保上述定义 长度一致

#define EVENT_CLASS_TYPE(typeName)                  \
    static EventType GetStaticType()                \
    {                                               \
        return EventType::##typeName;               \
    }                                               \
    virtual EventType GetEventType() const override \
    {                                               \
        return GetStaticType();                     \
    }                                               \
    virtual const char* GetName() const override    \
    {                                               \
        return #typeName;                           \
    }                                               \
    virtual int GetCategoryFlags() const override   \
    {                                               \
        return EventCategoryMap[GetStaticType()];   \
    }
// 去掉 EVENT_CLASS_CATEGORY, 由 type 决定就够了
#define EVENT_CLASS_CATEGORY(category)

class SP_API Event
{
    friend class EventDispatcher;

public:
    virtual EventType   GetEventType() const     = 0;
    virtual const char* GetName() const          = 0;
    virtual int         GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory category) { return false; }


    bool Handled = false;
};


class SP_API EventDispatcher
{
    template <typename T>
    using EventFn = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event)
        : _Event(event)
    {}

    template <typename T>
    bool Dispatch(EventFn<T> func)
    {
        if (_Event.GetEventType() == T::GetStaticType()) {
            _Event.Handled = func(*(T*)(&_Event));

            // SP_CORE_INFO("{0} Be Handled", _Event.GetName());

            return true;
        }
        return false;
    }

private:
    Event& _Event;
};



// spdlog 库用来输出
template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<SP::Event, T>::value, char>> : fmt::formatter<std::string>
{
    template <typename FormatCtx>
    auto format(const SP::Event& a, FormatCtx& ctx)
    {
        return fmt::formatter<std::string>::format(a.ToString(), ctx);
    }
};



}   // namespace SP
