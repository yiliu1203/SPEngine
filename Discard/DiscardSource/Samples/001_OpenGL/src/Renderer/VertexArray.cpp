#include "VertexArray.h"
#include "Core/GLCheck.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


NS_BEGIN

VertexArray::VertexArray()
{
    GLCHECK(glGenVertexArrays(1, &m_RenderID));
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RenderID);
}

/**
 * 所以这里注意 layout 和 vb 需要准确对应， 可能应该把 layout 放到 vb 里面更好些
*/
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    auto         layoutElements = layout.GetElements();
    unsigned int offset         = 0;
    for (size_t i = 0; i < layoutElements.size(); i++) {
        auto& layoutElement = layoutElements[i];

        std::cout << " GetStride : " << layout.GetStride() << "offset : " << offset << std::endl;

        GLCHECK(glVertexAttribPointer(i,
                                      layoutElement.count,
                                      layoutElement.type,
                                      layoutElement.normalized ? GL_TRUE : GL_FALSE,
                                      layout.GetStride(),
                                      (void*)offset));
        glEnableVertexAttribArray(i);
        offset += layoutElement.GetElementsSize();
    }
}

void VertexArray::Bind() const
{
    GLCHECK(glBindVertexArray(m_RenderID));
}

void VertexArray::UnBind() const
{
    GLCHECK(glBindVertexArray(0));
}

NS_END