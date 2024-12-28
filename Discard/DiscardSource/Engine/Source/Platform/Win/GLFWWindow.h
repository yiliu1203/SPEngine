#pragma once

#include "GLFW/glfw3.h"
#include "Graphics/RHI.h"
#include "Graphics/VertexFormateLayout.h"
#include "Window/Window.h"




namespace SP {
using namespace RHI;
class GLFWWindow;
namespace RHI {
extern void EnableRendererAPI(GLFWWindow& _this);
extern void DisableRendererAPI(GLFWWindow& _this);
}   // namespace RHI

class SP_API GLFWWindow : public Window
{
public:
    friend void RHI::EnableRendererAPI(GLFWWindow& _this);
    friend void RHI::DisableRendererAPI(GLFWWindow& _this);
    GLFWWindow(const WindowProps&);
    virtual ~GLFWWindow();

    virtual void  SetVSync(bool enabled) override;
    virtual void* GetNativeWindowHandle() const override { return _Window; }
    virtual void* GetWnd() const { return nullptr; }
    virtual int   ProcessMessage();
    virtual void  SwapChains();

    virtual void InitRendererContext() { RHI::EnableRendererAPI(*this); };
    virtual void DisableRendererContext() { RHI::DisableRendererAPI(*this); };

    // GLFWwindow* GetGlfwWindow() { return _Window; }
    // test
    void DrawScene();


private:
    virtual void Init(const WindowProps& props);
    virtual void ShutDown();

private:
    GLFWwindow* _Window;


    // test
private:
    uint32_t m_vertex_array;
    // uint32_t                      m_vertex_buffer;
    // uint32_t                      m_index_buffer;
    std::unique_ptr<Shader>       m_shader;
    std::shared_ptr<VertexBuffer> m_vertexbuffer;
    std::shared_ptr<IndexBuffer>  m_indexbuffer;
    std::shared_ptr<VertexArray>  m_vertexarray;
};




}   // namespace SP
