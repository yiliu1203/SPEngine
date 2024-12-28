#include "PCH.h"
#include "Device.h"
#include "VertexArray.h"



namespace SP::RHI {
VertexArray* VertexArray::Create()
{
    return Device::s_device->CreateVertexArray();
}
}   // namespace SP::RHI