#pragma once

#include "Event/Event.h"

namespace SP {

class Application;

class SP_API Layer
{
    friend class Application;

public:
    Layer(const std::string& name = "Layer");

    virtual ~Layer();

    virtual void        OnAttach() {}
    virtual void        OnDetach() {}
    virtual void        OnUpdate() {}
    virtual void        OnPrepared() {}
    virtual void        OnImGuiRenderer(){};
    virtual void        OnEvent(Event& event) {}
    inline Application* GetApp() { return _app; }

    inline const std::string& GetName() const { return _DebugName; }

protected:
    std::string  _DebugName;
    Application* _app;
};


}   // namespace SP
