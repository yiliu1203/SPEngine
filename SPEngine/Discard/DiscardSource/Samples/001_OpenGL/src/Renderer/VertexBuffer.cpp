#include "VertexBuffer.h"
#include "Core/GLCheck.h"

NS_BEGIN

VertexBuffer::VertexBuffer(const void* data, unsigned int byteSize)
    : m_BufferId{0}
{
    glGenBuffers(1, &m_BufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);        // 因为后面还有 attribute ， 这里就不能unbind了， 当前上下文是在一个OpenGL的大的状态机下
}

VertexBuffer::~VertexBuffer()
{
    if (m_BufferId > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        m_BufferId = 0;
    }
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
}

NS_END
