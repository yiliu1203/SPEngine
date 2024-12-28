#include "Texture.h"
#include "Device.h"

namespace SP::RHI {

Ref<Texture2D> Texture2D::Create(uint32 width, uint32 height)
{
    return Device::s_device->CreateTexture2d(width, height);
}

Ref<Texture2D> Texture2D::Create(const AssertURI& uri)
{
    return Device::s_device->CreateTexture2d(uri);
}
}   // namespace SP::RHI