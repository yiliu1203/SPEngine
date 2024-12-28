#include "OpenGLBuffer.h"
#include "PCH.h"
#include "Graphics/APreRHI.h"
#include "glad/glad.h"

namespace SP::RHI {
OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t count)
{

    glGenBuffers(1, &m_renderid);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, vertices, GL_STATIC_DRAW);
}
OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    if (m_renderid > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_renderid);
        m_renderid = 0;
    }
}
void OpenGLVertexBuffer::Bind()
{
    SP_ASSERT(m_renderid > 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderid);
}
void OpenGLVertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {}




//-----------------------------------IndexBuffer ------------------------------------


OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* vertices, uint32_t count)
    : m_count{count}
{
    glGenBuffers(1, &m_renderid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, vertices, GL_STATIC_DRAW);
}

/// @brief IndexBuffer
OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    if (m_renderid > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_renderid);
        m_renderid = 0;
    }
}

/// @brief Bind
void OpenGLIndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderid);
}
void OpenGLIndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void     OpenGLIndexBuffer::SetData(const void* data, uint32_t size) {}
uint32_t OpenGLIndexBuffer::GetCount()
{
    return m_count;
}
}   // namespace SP::RHI