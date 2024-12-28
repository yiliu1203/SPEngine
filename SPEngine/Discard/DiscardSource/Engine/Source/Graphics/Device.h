#pragma once

#include "PCH.h"
#include "Misc/AssertURI.h"

namespace SP {

namespace RHI {

class VertexBuffer;
class IndexBuffer;
class VertexArray;
class Shader;
class Texture2D;
class Framebuffer;
class FramebufferSpecification;

class Device
{
public:
    NoCopyable(Device);
    Device() = default;
    virtual ~Device();

    virtual VertexBuffer*    CreateVertexBuffer(const float* vertices, uint32_t count)          = 0;
    virtual IndexBuffer*     CreateIndexBuffer(const uint32_t* vertices, uint32_t count)        = 0;
    virtual VertexArray*     CreateVertexArray()                                                = 0;
    virtual Ref<Shader>      CreateShader(const std::string& name, const AssertURI& filename)   = 0;
    virtual Ref<Texture2D>   CreateTexture2d(uint32 width, uint32 height)                       = 0;
    virtual Ref<Texture2D>   CreateTexture2d(const AssertURI& uri)                              = 0;
    virtual Ref<Framebuffer> CreateFramebuffer(const FramebufferSpecification& framebuffer_spe) = 0;
    virtual void             DrawIndexed(const std::shared_ptr<VertexArray> vertex_array)       = 0;
    virtual void             ClearColor()                                                       = 0;
    virtual void             SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)       = 0;

public:
    static Device* Get() { return s_device; }

public:
    static Device* s_device;
};

}   // namespace RHI
}   // namespace SP