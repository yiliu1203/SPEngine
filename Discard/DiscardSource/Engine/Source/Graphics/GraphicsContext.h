#pragma once

namespace SP::RHI {
class GraphicsContext
{
public:
    virtual void Init()        = 0;
    virtual void SwapBuffers() = 0;
    virtual void Clear()       = 0;
};
}   // namespace SP::RHI