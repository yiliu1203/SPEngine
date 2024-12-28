#include "PCH.h"
#include "Misc/AssertURI.h"

namespace SP::RHI {

struct FramebufferSpecification
{
    uint32 width  = 0;
    uint32 height = 0;
};
class Framebuffer
{
public:
    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

    virtual ~Framebuffer()                                                        = default;
    virtual void                   Bind()                                         = 0;
    virtual void                   Unbind()                                       = 0;
    virtual void                   Resize(uint32 width, uint32 height)            = 0;
    virtual uint32                 GetRenderID() const                            = 0;
    virtual uint32                 GetColorAttachmentRendererID(uint32 index = 0) = 0;
    const FramebufferSpecification GetFramebufferSpecification() { return m_framebuff_spe; }
    uint32                         GetWidth() { return m_framebuff_spe.width; }
    uint32                         GetHeight() { return m_framebuff_spe.height; }

protected:
    Framebuffer(const FramebufferSpecification& spec)
        : m_framebuff_spe{spec}
    {}
    FramebufferSpecification m_framebuff_spe;
};
}   // namespace SP::RHI