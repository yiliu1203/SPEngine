
#pragma once

#include "PCH.h"

#include "Graphics/Device.h"



namespace SP::RHI {
class SP_API Dx11Device : public Device
{
public:
    virtual VertexBuffer*    CreateVertexBuffer(const float* vertices, uint32_t count) { return nullptr; };
    virtual IndexBuffer*     CreateIndexBuffer(const uint32_t* vertices, uint32_t count) { return nullptr; };
    virtual VertexArray*     CreateVertexArray() { return nullptr; };
    virtual void             DrawIndexed(const std::shared_ptr<VertexArray> vertex_array) { return; };
    virtual void             ClearColor() { return; };
    virtual Ref<Shader>      CreateShader(const std::string& name, const AssertURI& filename) { return nullptr; };
    virtual Ref<Texture2D>   CreateTexture2d(uint32 width, uint32 height) { return nullptr; };
    virtual Ref<Texture2D>   CreateTexture2d(const AssertURI& uri) { return nullptr; };
    virtual Ref<Framebuffer> CreateFramebuffer(const FramebufferSpecification& framebuffer_spe) { return nullptr; };
    virtual void             SetViewport(uint32 x, uint32 y, uint32 width, uint32 height){};
};
}   // namespace SP::RHI
