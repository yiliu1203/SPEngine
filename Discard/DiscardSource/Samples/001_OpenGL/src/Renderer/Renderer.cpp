#include "Renderer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"



NS_BEGIN

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCHECK(glDrawElements(GL_TRIANGLES, ib.GetCount(), ib.GetElementType(), (void*)0));
}

NS_END