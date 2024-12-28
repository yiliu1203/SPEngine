#pragma once
#include "Graphics/Texture.h"
#include <stb_image.h>

namespace SP::RHI {

class OpenGLTexture2D : public Texture2D
{
public:
    friend class OpenGLDevice;

    virtual ~OpenGLTexture2D();
    virtual uint32              GeWidth() const override { return m_width; };
    virtual uint32              GetHeight() const override { return m_height; };
    virtual uint32              getRenderID() const override { return m_rendererid; };
    virtual const AssertURI&    GetRUI() const override { return m_uri; };
    virtual void                SetData(void* data, uint32 size) override;
    virtual void                Bind(uint32 slot) const override;
    virtual bool                IsLoaded() const override { return m_isloaded; };
    virtual E_TextureDataFormat GetDataFormate() const override { return m_data_formate; }

private:
    // 私有构造函数，只能通过 Create
    OpenGLTexture2D(uint32 width, uint32 height);
    OpenGLTexture2D(const AssertURI& uri);

private:
    // 私有函数，只能通过友元类 OpenGLDevice
    template <typename... Args>
    static auto Create(Args&&... args)
    {
        // 为了让 私有构造函数可以访问
        struct EnabledMakeShared : public OpenGLTexture2D
        {
            EnabledMakeShared(Args&&... args)
                : OpenGLTexture2D(std::forward<Args>(args)...)
            {}
        };
        // return std::static_pointer_cast<OpenGLTexture2D>(CreateRef<EnabledMakeShared>(std::forward<Args>(args)...));
        return RefCastUp<OpenGLTexture2D>(CreateRef<EnabledMakeShared>(std::forward<Args>(args)...));
    };
    uint32 m_width;
    uint32 m_height;
    uint32 m_rendererid;
    bool   m_isloaded;

    AssertURI           m_uri;
    E_TextureDataFormat m_data_formate;
};



}   // namespace SP::RHI