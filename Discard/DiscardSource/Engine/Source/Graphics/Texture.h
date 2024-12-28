#pragma once
#include "PCH.h"
#include "Misc/AssertURI.h"


namespace SP::RHI {

class Texture
{
public:
    virtual ~Texture()                                           = default;
    virtual uint32              GeWidth() const                  = 0;
    virtual uint32              GetHeight() const                = 0;
    virtual uint32              getRenderID() const              = 0;
    virtual const AssertURI&    GetRUI() const                   = 0;
    virtual void                SetData(void* data, uint32 size) = 0;
    virtual void                Bind(uint32 slot) const          = 0;
    virtual bool                IsLoaded() const                 = 0;
    virtual E_TextureDataFormat GetDataFormate() const           = 0;
};

class Texture2D : public Texture
{
public:
    static Ref<Texture2D> Create(uint32 width, uint32 height);
    static Ref<Texture2D> Create(const AssertURI& uri);
};
}   // namespace SP::RHI