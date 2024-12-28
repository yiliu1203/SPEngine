#pragma once
#include "PCH.h"

namespace SP::RHI {

class VertexBuffer;
class IndexBuffer;

class VertexArray
{
    NoCopyable(VertexArray);

public:
    static VertexArray* Create();
    virtual ~VertexArray()                                          = default;
    virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vb) = 0;
    virtual void AddIndexBuffer(std::shared_ptr<IndexBuffer>& ib)   = 0;
    virtual void Bind() const                                       = 0;
    virtual void UnBind() const                                     = 0;

protected:
    VertexArray() = default;
};


}   // namespace SP::RHI
