#pragma once
#include "SPing.h"
#include <iostream>

namespace SP {
class DockSpaceLayer : public Layer
{
public:
    DockSpaceLayer(const std::string& name) {}
    virtual ~DockSpaceLayer(){};

    virtual void OnAttach();
    virtual void OnDetach() {}
    virtual void OnPrepared();
    virtual void OnImGuiRenderer();
    virtual void OnEvent(Event& event) {}
    virtual void OnUpdate();

private:
    Ref<VertexArray> m_square_va;
    color4f          m_color;
    Ref<Texture2D>   m_texture_bg;
    Ref<Texture2D>   m_texture_logo;
    Ref<Framebuffer> m_framebuffer;
};

}   // namespace SP