#include "Graphics/Renderer.h"
#include "Graphics/RenderCommand.h"
namespace SP::RHI {

E_API Renderer::m_api            = E_API::OpenGL;
mat4  Renderer::s_viewprojection = mat4{1.0f};

extern void InitRendererAPI();
void        Renderer::Init()
{
    InitRendererAPI();
}

void Renderer::BeginScene(const SP::Camera& camera)
{
    s_viewprojection = camera.GetViewProjection();
}
void Renderer::EndScene() {}
void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array,
                      const math::mat4 model_matrix, const UniformParam& param)
{
    shader->Bind();   // Bind 之后会把 之前绑定的shader param 清理掉了
    vertex_array->Bind();
    shader->SetMat4("u_viewprojection", s_viewprojection);
    shader->SetMat4("u_model", model_matrix);

    // 绑定其他param

    for (auto item : param) {
        shader->SetShaderParam(item.first, item.second);
    }

    RenderCommand::DrawIndexed(vertex_array);   // 在这里屏蔽上层，开始关注渲染API
}
void Renderer::OnWindowResize(int x, int y)
{
    RenderCommand::SetViewport(0, 0, x, y);
}

void Renderer::DrawTest() {}

}   // namespace SP::RHI