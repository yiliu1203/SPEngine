#include "SceneLayer.h"

namespace SP {



void SceneLayer::OnAttach() {}

void SceneLayer::OnPrepared()
{
    // square
    {
        VertexBuffer* vertex_buffer =
            (VertexBuffer::Create(SquareMesh::Vertex_XYZ_RGB_UV, sizeof(SquareMesh::Vertex_XYZ_RGB_UV) / sizeof(float)));
        vertex_buffer->SetLayout(E_VertexFormateLayout::VFL_f3xyz_f3rgb_f2uv);
        m_square_va.reset(VertexArray::Create());
        Ref<VertexBuffer> vertex_buffer_ref{vertex_buffer};
        m_square_va->AddVertexBuffer(vertex_buffer_ref);
        IndexBuffer* index_buffer = IndexBuffer::Create(SquareMesh::indices, sizeof(SquareMesh::indices) / sizeof(unsigned int));
        Ref<IndexBuffer> index_buffer_ref{index_buffer};
        m_square_va->AddIndexBuffer(index_buffer_ref);
        ShaderLibary::instance().load("pos_color", "shaders/base_vcolor.glsl");

        // texutre_shader
        ShaderLibary::instance().load("pos_texture", "shaders/base_texture.glsl");
        m_texture_bg   = Texture2D::Create("textures/bg.png");
        m_texture_logo = Texture2D::Create("textures/logo.png");
    }
}

void SceneLayer::OnImGuiRenderer()
{
    ImGui::Begin("Setting");
    ImGui::ColorEdit4("square Color", m_color.v);
    ImGui::End();
}

void SceneLayer::OnUpdate()
{
    Renderer::BeginScene(*Global::g_engine->GetMainCamera());
    mat4 transform{1.0};
    // auto trans0 = mat4::rotation(math::F_PI / 4, vec3{0, 0, 1});
    for (int i = 0; i < 100; i++) {
        int  row   = i / 10;
        int  col   = i % 10;
        mat4 trans = mat4::translation(vec3{col * 0.2f, row * 0.2f, 0.0f});
        trans      = matrix::scale(trans, 0.19f);
        Renderer::Submit(ShaderLibary::instance().get("pos_color"),
                         m_square_va,
                         trans,
                         UniformParam{"u_emission", vec3{m_color.r, m_color.g, m_color.b}});
    }

    mat4 middle{1.0};
    m_texture_bg->Bind(0);
    Renderer::Submit(ShaderLibary::instance().get("pos_texture"), m_square_va, middle, UniformParam{"u_texture", 0});
    m_texture_logo->Bind(0);
    Renderer::Submit(ShaderLibary::instance().get("pos_texture"), m_square_va, middle, UniformParam{"u_texture", 0});
    Renderer::EndScene();
}

}   // namespace SP