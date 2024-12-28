#pragma once
#include "PCH.h"
#include "Graphics/Framebuffer.h"
namespace SP::RHI {

class OpenGLFramebuffer : public Framebuffer
{
public:
    friend class OpenGLDevice;
    virtual ~OpenGLFramebuffer() override;
    virtual void   Bind() override;
    virtual void   Unbind() override;
    virtual void   Resize(uint32 width, uint32 height) override;
    virtual uint32 GetColorAttachmentRendererID(uint32 index = 0) override;
    virtual uint32 GetRenderID() const override { return m_renderid; };

    OpenGLFramebuffer(const FramebufferSpecification& framebuffer_spe);

private:
    uint32                        m_renderid                 = 0;
    uint32                        m_colorattachment_renderid = 0;
    static Ref<OpenGLFramebuffer> Create(const FramebufferSpecification& framebuffer_spe);
};
}   // namespace SP::RHI