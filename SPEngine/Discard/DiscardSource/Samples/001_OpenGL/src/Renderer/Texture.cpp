#include "Texture.h"
#include "Core/Exception.h"
#include "Core/GLCheck.h"
#include "GL/glew.h"
#include "Vendor/stb_image/stb_image.h"



NS_BEGIN



Texture::Texture(const std::string& path)
    : m_Path{path}
    , m_Width{0}
    , m_Height{0}
    , m_BPP{0}
    , m_Buffer(nullptr)
{
    stbi_set_flip_vertically_on_load(1);   // 是否翻转， opengl 纹理坐标用 左下角 作 0 0

    // gen texture
    GLCHECK(glGenTextures(1, &m_RenderID));
    GLCHECK(glBindTexture(GL_TEXTURE_2D, m_RenderID));


    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    m_Buffer = stbi_load(m_Path.path().c_str(), &m_Width, &m_Height, &m_BPP, 4);
    GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));

    GLCHECK(glBindTexture(GL_TEXTURE_2D, 0));


    stbi_image_free((void*)m_Buffer);
    m_Buffer = nullptr;
}


Texture::~Texture()
{
    GLCHECK(glDeleteTextures(1, &m_RenderID));
}

void Texture::Bind(unsigned int slot)
{
    GLCHECK(glActiveTexture(GL_TEXTURE0 + slot));
    GLCHECK(glBindTexture(GL_TEXTURE_2D, m_RenderID));
}

void Texture::UnBind()
{
    GLCHECK(glBindTexture(GL_TEXTURE_2D, 0));
}


NS_END