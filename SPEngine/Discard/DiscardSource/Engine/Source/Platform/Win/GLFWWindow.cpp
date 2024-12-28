#include "GLFWWindow.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Graphics/RHI.h"
#include "Math/mathfwd.h"
#include "Platform/GraphicsGL/OpenGLContext.h"
#include "glad/glad.h"



namespace SP {

using namespace RHI;

static bool s_GLFWInitialized = false;

GLFWWindow::GLFWWindow(const WindowProps& props)
    : Window{props}
{
    Init(props);
    InitRendererContext();
}

GLFWWindow::~GLFWWindow()
{
    ShutDown();
}

void GLFWWindow::Init(const WindowProps& props)
{

    SP_INFO("Init Window {0}, {1}, {2}", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
        int success = glfwInit();
        ASSERT(success, "GLFW INIT FAILED");
        s_GLFWInitialized = true;
    }

    _Window = glfwCreateWindow(_Data.Width, _Data.Height, _Data.Title.c_str(), nullptr, nullptr);

    // Global::g_context = new OpenGLContext(_Window);
    // Global::g_context->Init();

    glfwSetWindowUserPointer(_Window, &_Data);
    SetVSync(true);

    glfwSetWindowCloseCallback(_Window, [](GLFWwindow* _glfwWindow) {
        WindowCloseEvent e;
        WindowData&      data = *(WindowData*)glfwGetWindowUserPointer(_glfwWindow);
        data.EventCallback(e);
    });

    glfwSetWindowSizeCallback(_Window, [](GLFWwindow* _glfwWindow, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_glfwWindow);
        data.Height      = height;
        data.Width       = width;
        WindowResizeEvent e(width, height);
        data.EventCallback(e);
    });

    glfwSetKeyCallback(_Window, [](GLFWwindow* _glfwWindow, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_glfwWindow);
        switch (action) {
        case GLFW_PRESS:
        {
            KeyPressedEvent e(key, 0);
            data.EventCallback(e);
            break;
        }

        case GLFW_RELEASE:
        {
            KeyReleaseedEvent e(key);
            data.EventCallback(e);
            if (key == 256 /*Esc*/) {
                WindowCloseEvent e;
                WindowData&      data = *(WindowData*)glfwGetWindowUserPointer(_glfwWindow);
                data.EventCallback(e);
            }
            break;
        }

        case GLFW_REPEAT:
        {
            KeyPressedEvent e(key, 1);
            data.EventCallback(e);
            break;
        }
        }
    });

    glfwSetMouseButtonCallback(_Window, [](GLFWwindow* _glfwWindow, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(_glfwWindow);

        switch (action) {
        case GLFW_PRESS:
        {
            MouseButtonPressedEvent e(button);
            data.EventCallback(e);
            break;
        }

        case GLFW_RELEASE:
        {
            MouseButtonReleasedEvent e(button);
            data.EventCallback(e);
            break;
        }
        }
    });

    glfwSetCursorPosCallback(_Window, [](GLFWwindow* _glfwWindow, double xpos, double ypos) {
        WindowData&     data = *(WindowData*)glfwGetWindowUserPointer(_glfwWindow);
        MouseMovedEvent e((float)xpos, (float)ypos);
        data.EventCallback(e);
    });

    glfwSetScrollCallback(_Window, [](GLFWwindow* _glfwWindow, double xoffset, double yoffset) {
        WindowData&        data = *(WindowData*)glfwGetWindowUserPointer(_glfwWindow);
        MouseScrolledEvent e(xoffset, yoffset);
        data.EventCallback(e);
    });

    // glad: load all OpenGL function pointers
    // ---------------------------------------
}

void GLFWWindow::ShutDown()
{
    glfwDestroyWindow(_Window);
}


int GLFWWindow::ProcessMessage()
{
    glfwPollEvents();
    return 0;
}

void GLFWWindow::SwapChains()
{
    Global::g_context->SwapBuffers();
}

void GLFWWindow::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    _Data.VSync = enabled;
}


void GLFWWindow::DrawScene()
{
#if 0
    // clang-format off

    float vertices[] = {
        // possition            colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,
    };
    // clang-format on
    unsigned int indices[] = {0, 1, 2};
    // version1
    // {
    //     glGenVertexArrays(1, &m_vertex_array);
    //     glBindVertexArray(m_vertex_array);
    //     m_vertexbuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float)));
    //     m_indexbuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
    //     // 需要在bindbuffer 的后面
    //     // position attribute
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //     glEnableVertexAttribArray(0);
    //     // color attribute
    //     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //     glEnableVertexAttribArray(1);
    // }

    // version 2
    {
        VertexBuffer* vertex_buffer = (VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float)));
        vertex_buffer->SetLayout(E_VertexFormateLayout::VFL_f3xyz_f3rgb);
        m_vertexarray.reset(VertexArray::Create());
        std::shared_ptr<VertexBuffer> vertex_buffer_ref{vertex_buffer};
        m_vertexarray->AddVertexBuffer(vertex_buffer_ref);
        IndexBuffer*                 index_buffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));
        std::shared_ptr<IndexBuffer> index_buffer_ref{index_buffer};
        m_vertexarray->AddIndexBuffer(index_buffer_ref);
    }

    // version3
    {}

    std::string vertexShader   = R"(
        #version 410 core
        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_color;
        out vec3 v_color;
        void main()
        {
            gl_Position = vec4(a_position, 1.0);
            v_color = a_color;
        }
    )";
    std::string fragmentShader = R"(
        #version 410 core
        layout(location=0) out vec4 FragColor;
        in vec3 v_color;
        void main()
        {
            FragColor = vec4(v_color, 1.0);
        }
        
    )";
    m_shader.reset(new Shader(vertexShader, fragmentShader));
    m_shader->Bind();
    m_vertexarray->Bind();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);
    // test
    {
        std::cout << VertexFormateLayoutTraits_VFL_fxyz::name << std::endl;
        math::vec2 v{1, 1};
        SP_INFO("{0}", v);
        SP_INFO(" is vec2 = {0}", math::IsVec<math::vec2>::value);
        math::quat q{1.0f, 1.0f, 1.0f, 1.0f};
        SP_INFO("{0}", math::IsQuat<math::quat>::value);
        SP_INFO("{0}", q);

        math::mat m;
        SP_INFO("{0}", math::IsMatix<math::mat>::value);
        SP_INFO("{0}", m);
    }
#endif
}










}   // namespace SP