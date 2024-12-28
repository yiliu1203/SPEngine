#include "Buffer.h"
#include "APreRHI.h"
#include "Device.h"



namespace SP::RHI {

VertexBuffer* VertexBuffer::Create(const float* vertices, uint32_t count)
{
    return Device::s_device->CreateVertexBuffer(vertices, count);
}

IndexBuffer* IndexBuffer::Create(const uint32_t* vertices, uint32_t count)
{
    return Device::s_device->CreateIndexBuffer(vertices, count);
}

}   // namespace SP::RHI
