#pragma once
#include "SPing.h"
#include <iostream>

namespace SP {
class EditorLayer : public Layer
{
public:
    EditorLayer(const std::string& name) {}
    virtual ~EditorLayer(){};

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
    vec2             m_viewport_size;
};

}   // namespace SP