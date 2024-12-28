#pragma once

#include "PCH.h"
#include "Graphics/Shader.h"
#include "Graphics/UniformParam.h"
#include "Graphics/VertexArray.h"
#include "World/Camera.h"
#include "math/mathfwd.h"



namespace SP::RHI {

class SP_API Renderer
{

public:
    Renderer() = default;
    NoCopyable(Renderer);
    E_API GetRenderAPI() { return Renderer::m_api; }

    static void Init();
    static void BeginScene(const SP::Camera& camera);
    static void EndScene();
    static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array,
                       const math::mat4 model_matrix = math::mat4{1.0f}, const UniformParam& param = {});

    static void OnWindowResize(int x, int y);

    // ------------Test-------
    static void DrawTest();

private:
    static E_API      m_api;
    static math::mat4 s_viewprojection;
};
}   // namespace SP::RHI