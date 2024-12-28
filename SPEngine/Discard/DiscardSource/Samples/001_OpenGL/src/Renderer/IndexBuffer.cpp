#include "IndexBuffer.h"
#include "Core/APre.h"
#include "Core/GLCheck.h"

NS_BEGIN

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, unsigned int offset)
    : m_BufferId{0}
    , m_Count{count}
    , m_Offset{offset}
{
    GLCHECK(glGenBuffers(1, &m_BufferId));
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId));
    GLCHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW));
    // GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));   // 因为有VAO, 这通用不能在 VAO unbind 之前做unbind
}

IndexBuffer::~IndexBuffer()
{
    if (m_BufferId > 0) {
        GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        m_BufferId = 0;
    }
}

void IndexBuffer::Bind() const
{
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId));
}

NS_END