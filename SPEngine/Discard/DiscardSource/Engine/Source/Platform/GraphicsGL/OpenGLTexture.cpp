#include "OpenGLTexture.h"
#include "OpenGLHelpers.h"
#include "glad/glad.h"



namespace SP::RHI {
OpenGLTexture2D::~OpenGLTexture2D()
{
    // SP_ASSERT(m_isloaded && m_rendererid);
    glDeleteTextures(1, &m_rendererid);
    m_rendererid = 0;
}
void OpenGLTexture2D::SetData(void* data, uint32 size)
{
    uint32_t bpp = g_texture_data_formate_config[static_cast<int>(m_data_formate)].bpp;
    SP_ASSERT(size == m_width * m_height * bpp, "Data must be entire texture!");
    glTextureSubImage2D(m_rendererid,
                        0,
                        0,
                        0,
                        m_width,
                        m_height,
                        g_texture_data_formate_config[static_cast<int>(m_data_formate)].DataFormate,
                        GL_UNSIGNED_BYTE,
                        data);
}
void OpenGLTexture2D::Bind(uint32 slot) const
{
    glBindTextureUnit(slot, m_rendererid);
}
OpenGLTexture2D::OpenGLTexture2D(uint32 width, uint32 height)
    : m_width(width)
    , m_height(m_height)
    , m_isloaded(false)
{}

OpenGLTexture2D::OpenGLTexture2D(const AssertURI& uri)
    : m_uri(uri)
    , m_isloaded(false)
{
    int      width, height, channels;
    stbi_uc* data = nullptr;
    data          = stbi_load(m_uri.get_abs_path().c_str(), &width, &height, &channels, 0);
    if (data) {
        m_isloaded = true;
        m_width    = width;
        m_height   = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4) {
            m_data_formate = E_TextureDataFormat::RGBA8;
        }
        else if (channels == 3) {
            m_data_formate = E_TextureDataFormat::RGB8;
        }
        else {
            SP_ASSERT(false, "Texture channels error");
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererid);
        glTextureStorage2D(
            m_rendererid, 1, g_texture_data_formate_config[static_cast<int>(m_data_formate)].InternalFormat, m_width, m_height);

        glTextureParameteri(m_rendererid, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // 放大时的采样方案：GL_LINEAR 会变模糊。GL_NEAREST 正常
        glTextureParameteri(m_rendererid, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererid, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_rendererid, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_rendererid,
                            0,
                            0,
                            0,
                            m_width,
                            m_height,
                            g_texture_data_formate_config[static_cast<int>(m_data_formate)].DataFormate,
                            GL_UNSIGNED_BYTE,
                            data);

        stbi_image_free(data);
    }
    else {
        m_isloaded = false;
        SP_ASSERT(false, "Texture Load Failed! {0}", m_uri.get_abs_path());
    }
}
}   // namespace SP::RHI