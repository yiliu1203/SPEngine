#pragma once
#include "PCH.h"
#include "Graphics/Device.h"
#include "Graphics/VertexArray.h"

namespace SP::RHI {

class RenderCommand
{
public:
    inline static void DrawIndexed(const std::shared_ptr<VertexArray> vertex_array)
    {
        return Device::s_device->DrawIndexed(vertex_array);
    }

    inline static void ClearColor() { return Device::s_device->ClearColor(); }

    inline static void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
    {
        return Device::s_device->SetViewport(x, y, width, height);
    }
};
}   // namespace SP::RHI