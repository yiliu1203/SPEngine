
#pragma once

#include "PCH.h"

#include "Window/Layer.h"

namespace SP {
class SP_API MainLayer : public Layer
{
public:
    MainLayer(const std::string& name);
    ~MainLayer();
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRenderer() override;
    virtual void OnEvent(Event& event);
    void         Begin();
    void         End();

private:
    void* _window_handle;
};
}   // namespace SP
