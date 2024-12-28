#pragma once
#include "Core/APre.h"
#include <GL/glew.h>

NS_BEGIN

class VertexBuffer
{
private:
    unsigned int m_BufferId;

public:
    VertexBuffer() = delete;
    VertexBuffer(const void* data, unsigned int byteSize);
    ~VertexBuffer();
    void Bind() const;
};

NS_END