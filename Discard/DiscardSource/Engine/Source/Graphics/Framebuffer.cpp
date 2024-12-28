#include "Framebuffer.h"
#include "Device.h"

namespace SP::RHI {
Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
    return Device::s_device->CreateFramebuffer(spec);
}
}   // namespace SP::RHI