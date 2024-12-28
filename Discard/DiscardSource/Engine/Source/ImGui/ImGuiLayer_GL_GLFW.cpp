#include "ImGuiLayer.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Application.h"
#include "Misc/Path.h"
#include "imgui.h"
#include <glad/glad.h>
#include <stdio.h>


#include "GLFW/glfw3.h"   // Will drag system OpenGL headers

namespace SP {

ImGuiLayer::ImGuiLayer(const std::string& name)
    : Layer{name}
{}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach()
{
    SP_INFO("ImGuiLayer::OnAttach");
    ASSERT(glfwInit(), "ImGuiLayer::OnAttach Error!");

    // 1.init
    const char* glsl_version = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);             // 3.0+ only
                                                                     // 2. Setup Dear ImGui context
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    _window_handle = (GLFWwindow*)_app->GetWindow()->GetNativeWindowHandle();
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)_window_handle, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Load Fonts
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF((Path::res_root / "fonts" / "Cousine-Regular.ttf").generic_string().c_str(), 16.0f);
    io.Fonts->AddFontFromFileTTF((Path::res_root / "fonts" / "Roboto-Medium.ttf").generic_string().c_str(), 16.0f);
    io.Fonts->AddFontFromFileTTF((Path::res_root / "fonts" / "Cousine-Regular.ttf").generic_string().c_str(), 15.0f);
}

void ImGuiLayer::OnDetach()
{
    SP_INFO("ImGuiLayer::OnDetach");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate()
{
    // SP_INFO("ImGuiLayer::OnUpdate");
    // Start the Dear ImGui frame
    // bool show_demo_window = true;
    // ImGui::ShowDemoWindow(&show_demo_window);
    // glfwSwapBuffers(_glfw_window);
}

void ImGuiLayer::OnImGuiRenderer()
{
    return;
    static bool show_demo_window    = true;
    static bool show_another_window = true;
    ImVec4      clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGuiIO&    io                  = ImGui::GetIO();
    (void)io;

    // ImGui::ShowDemoWindow(&show_demo_window);

    {
        static float f       = 0.0f;
        static int   counter = 0;

        ImGui::Begin("Hello, world!");                       // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");            // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);   // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);              // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color);   // Edit 3 floats representing a color

        if (ImGui::Button("Button"))   // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}

void ImGuiLayer::OnEvent(Event& event)
{
    if (event.GetEventType() == EventType::KeyPressed) {
        KeyPressedEvent* keypressEvent = dynamic_cast<KeyPressedEvent*>(&event);
        SP_INFO("{0}", (char)(keypressEvent->GetKeyCode()));
    }
}

void ImGuiLayer::Begin()
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    // glfwPollEvents();  to WinWindow.OnUpdate

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End()
{
    ImGuiIO& io = ImGui::GetIO();
    // ImVec4   clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize((GLFWwindow*)_window_handle, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    // glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

}   // namespace SP
