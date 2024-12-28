#include "OpenGLFramebuffer.h"
#include "OpenGLHelpers.h"
#include "glad/glad.h"

namespace SP::RHI {
OpenGLFramebuffer::~OpenGLFramebuffer()
{
    SP_ASSERT(m_renderid > 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteBuffers(1, &m_renderid);
    m_renderid = 0;
}
void OpenGLFramebuffer::Bind()
{
    SP_ASSERT(m_renderid > 0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderid);
}
void OpenGLFramebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void   OpenGLFramebuffer::Resize(uint32 width, uint32 height) {}
uint32 OpenGLFramebuffer::GetColorAttachmentRendererID(uint32 index)
{
    return m_colorattachment_renderid;
}
OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& framebuffer_spe)
    : Framebuffer{framebuffer_spe}
{

    glGenFramebuffers(1, &m_renderid);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderid);
    // create a color attachment texture
    glGenTextures(1, &m_colorattachment_renderid);
    glBindTexture(GL_TEXTURE_2D, m_colorattachment_renderid);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 g_texture_data_formate_config[(size_t)E_TextureDataFormat::RGBA8].InternalFormat,
                 m_framebuff_spe.width,
                 m_framebuff_spe.height,
                 0,
                 g_texture_data_formate_config[(size_t)E_TextureDataFormat::RGBA8].DataFormate,
                 GL_UNSIGNED_BYTE,
                 NULL);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_framebuff_spe.width, m_framebuff_spe.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorattachment_renderid, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          GL_DEPTH24_STENCIL8,
                          m_framebuff_spe.width,
                          m_framebuff_spe.height);   // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);   // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    SP_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame Buffer Not Complete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
Ref<OpenGLFramebuffer> OpenGLFramebuffer::Create(const FramebufferSpecification& framebuffer_spe)
{
    return CreateRef<OpenGLFramebuffer>(framebuffer_spe);
}
}   // namespace SP::RHI