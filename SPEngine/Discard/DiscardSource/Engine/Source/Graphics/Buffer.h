#pragma once
#include "PCH.h"
#include "Graphics/APreRHI.h"
#include "Graphics/VertexFormateLayout.h"



namespace SP::RHI {

class VertexBuffer
{
    NoCopyable(VertexBuffer);

public:
    virtual ~VertexBuffer() = default;
    static VertexBuffer* Create(const float* vertices, uint32_t count);


    virtual void                  Bind()                                   = 0;
    virtual void                  UnBind()                                 = 0;
    virtual void                  SetData(const void* data, uint32_t size) = 0;
    void                          SetLayout(E_VertexFormateLayout layout) { m_vfl_layout = layout; };
    virtual E_VertexFormateLayout GetLayout() { return m_vfl_layout; }

protected:
    E_VertexFormateLayout m_vfl_layout;
    VertexBuffer() = default;
};

class IndexBuffer
{
    NoCopyable(IndexBuffer);

public:
    virtual ~IndexBuffer() = default;
    static IndexBuffer* Create(const uint32_t* vertices, uint32_t count);
    virtual void        Bind()                                   = 0;
    virtual void        UnBind()                                 = 0;
    virtual void        SetData(const void* data, uint32_t size) = 0;
    virtual uint32_t    GetCount()                               = 0;

protected:
    IndexBuffer() = default;
};



}   // namespace SP::RHI