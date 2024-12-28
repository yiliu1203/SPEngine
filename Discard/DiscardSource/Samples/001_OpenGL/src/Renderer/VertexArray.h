#pragma once
#include "Core/APre.h"
#include <GL/glew.h>

NS_BEGIN

class VertexBufferLayout;
class VertexBuffer;

class VertexArray
{
private:
    unsigned int m_RenderID;

public:
    VertexArray();
    ~VertexArray();
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void Bind() const;
    void UnBind() const;
};

NS_END