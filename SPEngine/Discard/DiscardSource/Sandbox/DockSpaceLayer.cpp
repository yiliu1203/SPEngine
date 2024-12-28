#include "DockSpaceLayer.h"

namespace SP {

void DockSpaceLayer::OnAttach() {}

void DockSpaceLayer::OnPrepared()
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
        ShaderLibary::instance().load("pos_color2", "shaders/base_vcolor.glsl");

        // texutre_shader
        ShaderLibary::instance().load("pos_texture2", "shaders/base_texture.glsl");
        m_texture_bg   = Texture2D::Create("textures/bg.png");
        m_texture_logo = Texture2D::Create("textures/logo.png");

        // use frame buffer
        m_framebuffer = Framebuffer::Create(FramebufferSpecification{Const::WindowWidthInitial, Const::WindowHeightInitial});
    }
}

void DockSpaceLayer::OnImGuiRenderer()
{
    // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    static bool               open            = true;
    static bool               opt_fullscreen  = true;
    static bool               opt_padding     = false;
    bool*                     p_open          = &open;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |=
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding) ImGui::PopStyleVar();

    if (opt_fullscreen) ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else {
        // ShowDockingDisabledMessage();
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Options")) {
            if (ImGui::MenuItem("Close", NULL, false, true)) {
                Application::Get()->Close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }



    color4f color;
    ImGui::Begin("Setting2");
    ImGui::ColorEdit4("square Color2", color.v);
    // ImGui::Image(reinterpret_cast<void*>(m_texture_logo->getRenderID()),
    //              ImVec2(m_texture_logo->GeWidth(), m_texture_logo->GetHeight()));
    ImGui::Image(reinterpret_cast<void*>(m_framebuffer->GetColorAttachmentRendererID()),
                 ImVec2(m_framebuffer->GetWidth(), m_framebuffer->GetHeight()));
    ImGui::End();
    ImGui::End();
}

void DockSpaceLayer::OnUpdate()
{
    m_framebuffer->Bind();
    RenderCommand::ClearColor();
    Renderer::BeginScene(*Global::g_engine->GetMainCamera());
    mat4 middle{1.0};
    m_texture_bg->Bind(0);
    Renderer::Submit(ShaderLibary::instance().get("pos_texture2"), m_square_va, middle, UniformParam{"u_texture", 0});
    m_texture_logo->Bind(0);
    Renderer::Submit(ShaderLibary::instance().get("pos_texture2"), m_square_va, middle, UniformParam{"u_texture", 0});
    Renderer::EndScene();
    m_framebuffer->Unbind();
}

}   // namespace SP