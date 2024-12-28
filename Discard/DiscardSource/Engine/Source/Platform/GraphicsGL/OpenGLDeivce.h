
#pragma once

#include "PCH.h"

#include "Graphics/Device.h"



namespace SP::RHI {
class SP_API OpenGLDevice : public Device
{
public:
    virtual VertexBuffer*    CreateVertexBuffer(const float* vertices, uint32_t count) override;
    virtual IndexBuffer*     CreateIndexBuffer(const uint32_t* vertices, uint32_t count) override;
    virtual VertexArray*     CreateVertexArray() override;
    virtual void             DrawIndexed(const std::shared_ptr<VertexArray> vertex_array) override;
    virtual void             ClearColor() override;
    virtual Ref<Shader>      CreateShader(const std::string& name, const AssertURI& filename) override;
    virtual Ref<Texture2D>   CreateTexture2d(uint32 width, uint32 height) override;
    virtual Ref<Texture2D>   CreateTexture2d(const AssertURI& uri) override;
    virtual Ref<Framebuffer> CreateFramebuffer(const FramebufferSpecification& framebuffer_spe) override;
    virtual void             SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
};
}   // namespace SP::RHI
