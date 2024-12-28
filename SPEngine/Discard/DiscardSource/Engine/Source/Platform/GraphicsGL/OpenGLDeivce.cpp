
#include "OpenGLDeivce.h"
#include "OpenGLBuffer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"



namespace SP::RHI {

Device* Device::s_device = new OpenGLDevice();

VertexBuffer* OpenGLDevice::CreateVertexBuffer(const float* vertices, uint32_t count)
{
    OpenGLVertexBuffer* vertex_buffer = new OpenGLVertexBuffer(vertices, count);
    return vertex_buffer;
}
IndexBuffer* OpenGLDevice::CreateIndexBuffer(const uint32_t* vertices, uint32_t count)
{
    OpenGLIndexBuffer* index_buffer = new OpenGLIndexBuffer(vertices, count);
    return index_buffer;
}
VertexArray* OpenGLDevice::CreateVertexArray()
{
    OpenGLVertexArray* vertex_array = new OpenGLVertexArray();
    return vertex_array;
}
void OpenGLDevice::DrawIndexed(const std::shared_ptr<VertexArray> vertex_array)
{
    vertex_array->Bind();   // ? 再 Bind 一次？
    auto     opengl_vertex_array = std::dynamic_pointer_cast<OpenGLVertexArray>(vertex_array);
    uint32_t count               = opengl_vertex_array->GetIndexBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)0);
}
void OpenGLDevice::ClearColor()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
std::shared_ptr<Shader> OpenGLDevice::CreateShader(const std::string& name, const AssertURI& filename)
{
    return std::make_shared<OpenGLShader>(name, filename);
}
Ref<Texture2D> OpenGLDevice::CreateTexture2d(uint32 width, uint32 height)
{
    return std::static_pointer_cast<Texture2D>(OpenGLTexture2D::Create(width, height));
}
Ref<Texture2D> OpenGLDevice::CreateTexture2d(const AssertURI& uri)
{
    return std::static_pointer_cast<Texture2D>(OpenGLTexture2D::Create(uri));
}

Ref<Framebuffer> OpenGLDevice::CreateFramebuffer(const FramebufferSpecification& framebuffer_spe)
{
    return std::static_pointer_cast<Framebuffer>(OpenGLFramebuffer::Create(framebuffer_spe));
}

void OpenGLDevice::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
    // glViewport(x, y, width, height);
}

}   // namespace SP::RHI