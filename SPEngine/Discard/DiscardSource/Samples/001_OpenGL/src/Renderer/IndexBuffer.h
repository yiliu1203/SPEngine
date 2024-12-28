#pragma once
#include "Core/APre.h"
#include <GL/glew.h>



NS_BEGIN

class IndexBuffer
{
private:
    unsigned int m_BufferId;
    unsigned int m_Count;
    unsigned int m_Offset;

public:
    IndexBuffer() = delete;
    IndexBuffer(const unsigned int* data, unsigned int count, unsigned int offset = 0);
    ~IndexBuffer();
    unsigned int GetCount() const { return m_Count; }
    unsigned int GetElementType() const { return GL_UNSIGNED_INT; }
    void         Bind() const;
};

NS_END