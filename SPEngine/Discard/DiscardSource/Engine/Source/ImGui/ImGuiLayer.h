
#pragma once

#include "PCH.h"
#include "Event/MouseEvent.h"

#include "Window/Layer.h"

struct GLFWwindow;
namespace SP {
class SP_API ImGuiLayer : public Layer
{
public:
    ImGuiLayer(const std::string& name);
    ~ImGuiLayer();
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
