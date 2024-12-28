#pragma once

#include "PCH.h"
#include "Graphics/GraphicsContext.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>


namespace SP {

class Win32Window;

namespace RHI {

void InitRendererAPI();
void EnableRendererAPI(Win32Window& _this);
void DisableRendererAPI(Win32Window& _this);


class Dx11Context : public GraphicsContext
{
public:
    Dx11Context(Win32Window* _pWin32Window)
        : m_pWin32Window(_pWin32Window)
    {}
    virtual void Init() override;
    virtual void SwapBuffers() override;
    virtual void Clear() override;

public:
    Win32Window* m_pWin32Window;

    Microsoft::WRL::ComPtr<ID3D11Device>           mDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain>         mSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    mDeviceContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDSV;
};

}   // namespace RHI
}   // namespace SP