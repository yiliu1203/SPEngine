#include "RenderScene.h"
#include "glad/glad.h"

namespace SP {
using namespace RHI;
RenderScene::RenderScene(Camera* camera)
    : m_camera(camera)
{}


RenderScene::~RenderScene()
{
    delete m_camera;
}
void RenderScene::Init()
{
    // clang-format off
    float triangle_vertices[] = {
        // possition            colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,
    };
    // clang-format on
    unsigned int triangle_indices[] = {0, 1, 2};

    // clang-format off
    float square_vertices[] = {
        // possition            colors
        -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,
    };
    // clang-format on
    unsigned int square_indices[] = {0, 1, 2, 0, 2, 3};

    // triangle
    {
        VertexBuffer* vertex_buffer = (VertexBuffer::Create(triangle_vertices, sizeof(triangle_vertices) / sizeof(float)));
        vertex_buffer->SetLayout(E_VertexFormateLayout::VFL_f3xyz_f3rgb);
        m_triangle_va.reset(VertexArray::Create());
        std::shared_ptr<VertexBuffer> vertex_buffer_ref{vertex_buffer};
        m_triangle_va->AddVertexBuffer(vertex_buffer_ref);
        IndexBuffer* index_buffer = IndexBuffer::Create(triangle_indices, sizeof(triangle_indices) / sizeof(unsigned int));
        std::shared_ptr<IndexBuffer> index_buffer_ref{index_buffer};
        m_triangle_va->AddIndexBuffer(index_buffer_ref);
        m_triangle_shader = Shader::Create("simple.glsl");
    }

    // square
    {
        VertexBuffer* vertex_buffer = (VertexBuffer::Create(square_vertices, sizeof(square_vertices) / sizeof(float)));
        vertex_buffer->SetLayout(E_VertexFormateLayout::VFL_f3xyz_f3rgb);
        m_square_va.reset(VertexArray::Create());
        std::shared_ptr<VertexBuffer> vertex_buffer_ref{vertex_buffer};
        m_square_va->AddVertexBuffer(vertex_buffer_ref);
        IndexBuffer* index_buffer = IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(unsigned int));
        std::shared_ptr<IndexBuffer> index_buffer_ref{index_buffer};
        m_square_va->AddIndexBuffer(index_buffer_ref);
        m_square_shader = Shader::Create("simple.glsl");
    }

    // camera
    m_camera->SetPosition({0, 0, 2.0f});
}
void RenderScene::DrawFrame()
{
    RenderCommand::ClearColor();
    // Renderer::BeginScene(*m_camera);
    // Renderer::Submit(m_squareShader, m_squareVA);

    // mat4 transform{1.0};
    // transform = transform.translation(vec3{1, 1, 1});
    // auto a    = transform* vec3{1, 1, 1};
    // Renderer::Submit(m_squareShader, m_squareVA, transform);
    // Renderer::Submit(m_triangleShader, m_triangleVA);

    // Renderer::EndScene();
}
}   // namespace SP