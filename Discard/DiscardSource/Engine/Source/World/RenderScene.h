#pragma once

#include "PCH.h"
#include "Event/Event.h"
#include "Graphics/RHI.h"


namespace SP {
using namespace RHI;
class SP::Camera;
class RenderScene
{
public:
    RenderScene(Camera* camera);
    ~RenderScene();

    void Init();
    void DrawFrame();

private:
    std::shared_ptr<VertexArray> m_triangle_va;
    std::shared_ptr<VertexArray> m_square_va;
    std::shared_ptr<Shader>      m_triangle_shader;
    std::shared_ptr<Shader>      m_square_shader;

    Camera* m_camera;
};
}   // namespace SP