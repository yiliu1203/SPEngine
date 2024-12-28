#pragma once
#include "Core/Exception.h"
#include <GL/glew.h>
#include <vector>


NS_BEGIN


// 这里是对 glVertexAttribPointer 的封装， 希望从这个对象就能拿到 GlVertexAttributePointer 的参数

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    bool         normalized;

    unsigned int GetElementsSize()
    {
        switch (type) {
        case GL_FLOAT: return sizeof(float) * count; break;
        case GL_BYTE: return sizeof(char) * count; break;
        case GL_UNSIGNED_INT: return sizeof(unsigned int) * count; break;

        default: ASSERT(0); break;
        }
        return 0;
    }
};


class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int                     m_Stride;

public:
    VertexBufferLayout();

    template <typename T>
    void Push(unsigned count)
    {
        ASSERT(false);
    }

    template <>
    void Push<float>(unsigned int count)
    {
        VertexBufferElement vbe{GL_FLOAT, count, false};
        m_Stride += vbe.GetElementsSize();
        m_Elements.push_back(vbe);
    }

    template <>
    void Push<unsigned int>(unsigned int count)
    {
        VertexBufferElement vbe{GL_UNSIGNED_INT, count, false};
        m_Stride += vbe.GetElementsSize();
        m_Elements.push_back(vbe);
    }

    template <>
    void Push<unsigned char>(unsigned int count)
    {
        VertexBufferElement vbe{GL_BYTE, count, false};
        m_Stride += vbe.GetElementsSize();
        m_Elements.push_back(vbe);
    }

    // push byte push...

    const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    unsigned int                            GetStride() const { return m_Stride; }
};

NS_END