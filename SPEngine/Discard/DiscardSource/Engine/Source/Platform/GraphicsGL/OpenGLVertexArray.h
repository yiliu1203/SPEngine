#pragma once

#include "PCH.h"
#include "Graphics/APreRHI.h"
#include "Graphics/VertexArray.h"

namespace SP {
namespace RHI {

class OpenGLVertexArray : public VertexArray
{
    friend class OpenGLDevice;

public:
    virtual ~OpenGLVertexArray();
    virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vb);
    virtual void AddIndexBuffer(std::shared_ptr<IndexBuffer>& ib);

    virtual void                       Bind() const;
    virtual void                       UnBind() const;
    const std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_indexbuffer; }


protected:
    OpenGLVertexArray();


private:
    uint32_t                                   m_renderid;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexbuffers;
    std::shared_ptr<IndexBuffer>               m_indexbuffer;
};

}   // namespace RHI
}   // namespace SP