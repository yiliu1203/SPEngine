#pragma once

#include "PCH.h"
#include "Graphics/Buffer.h"

namespace SP::RHI {

class OpenGLVertexBuffer : public VertexBuffer
{
    friend class OpenGLDevice;

public:
    ~OpenGLVertexBuffer();
    virtual void Bind() override;
    virtual void UnBind() override;
    virtual void SetData(const void* data, uint32_t size) override;

private:
    uint32_t m_renderid;
    OpenGLVertexBuffer(const float* vertices, uint32_t count);
};
class OpenGLIndexBuffer : public IndexBuffer
{
    friend class OpenGLDevice;

public:
    ~OpenGLIndexBuffer();
    virtual void     Bind() override;
    virtual void     UnBind() override;
    virtual void     SetData(const void* data, uint32_t size) override;
    virtual uint32_t GetCount() override;

private:
    uint32_t m_renderid;
    uint32_t m_count;
    OpenGLIndexBuffer(const uint32_t* vertices, uint32_t count);
};
}   // namespace SP::RHI