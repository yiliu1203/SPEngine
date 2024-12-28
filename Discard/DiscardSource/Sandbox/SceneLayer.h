#pragma once
#include "SPing.h"
#include <iostream>

namespace SP {
class SceneLayer : public Layer
{
public:
    SceneLayer(const std::string& name) {}
    virtual ~SceneLayer(){};

    virtual void OnAttach();
    virtual void OnDetach() {}
    virtual void OnPrepared();
    virtual void OnImGuiRenderer();
    virtual void OnEvent(Event& event) {}
    virtual void OnUpdate();

private:
    Ref<VertexArray> m_square_va;
    // Ref<Shader>      m_square_shader;
    // Ref<Shader>      m_texture_shader;
    color4f        m_color;
    Ref<Texture2D> m_texture_bg;
    Ref<Texture2D> m_texture_logo;
};

}   // namespace SP