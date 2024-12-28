#include "Dx11Context.h"
#include "DxgiInfoManager.h"
#include "Misc/Const.h"
#include "Platform/Win/Win32Window.h"
#include <Windows.h>



namespace wrl = Microsoft::WRL;
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
namespace SP::RHI {

void InitRendererAPI()
{
    std::cout << "InitRendererAPI";
}


void EnableRendererAPI(Win32Window& _this)
{

    Global::g_context = new Dx11Context(&_this);
    Global::g_context->Init();
}
void DisableRendererAPI(Win32Window& _this)
{

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(_this.mHrc);
    ReleaseDC(_this.mHwnd, _this.mHdc);
}

void Dx11Context::Init()
{
    DXGI_SWAP_CHAIN_DESC sd               = {};
    sd.BufferDesc.Width                   = 0;
    sd.BufferDesc.Height                  = 0;
    sd.BufferDesc.Format                  = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator   = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count                   = 1;
    sd.SampleDesc.Quality                 = 0;
    sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount                        = 1;
    sd.OutputWindow                       = *(HWND*)m_pWin32Window->GetWnd();
    sd.Windowed                           = TRUE;
    sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags                              = 0;

    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    // for checking results of d3d functions
    HRESULT hr;

    DX_ASSERT(D3D11CreateDeviceAndSwapChain(nullptr,
                                            D3D_DRIVER_TYPE_HARDWARE,
                                            nullptr,
                                            swapCreateFlags,
                                            nullptr,
                                            0,
                                            D3D11_SDK_VERSION,
                                            &sd,
                                            &mSwapChain,
                                            &mDevice,
                                            nullptr,
                                            &mDeviceContext));


    // gain access to texture subresource in swap chain (back buffer)
    wrl::ComPtr<ID3D11Resource> pBackBuffer;
    DX_ASSERT(mSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
    DX_ASSERT(mDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &mRenderTargetView));

    // create depth stensil state
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable              = TRUE;
    dsDesc.DepthWriteMask           = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc                = D3D11_COMPARISON_LESS;
    wrl::ComPtr<ID3D11DepthStencilState> pDSState;
    DX_ASSERT(mDevice->CreateDepthStencilState(&dsDesc, &pDSState));

    // bind depth state
    mDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);

    // create depth stensil texture
    wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC         descDepth = {};
    descDepth.Width                        = Const::WindowWidthInitial;
    descDepth.Height                       = Const::WindowHeightInitial;
    descDepth.MipLevels                    = 1u;
    descDepth.ArraySize                    = 1u;
    descDepth.Format                       = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count             = 1u;
    descDepth.SampleDesc.Quality           = 0u;
    descDepth.Usage                        = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags                    = D3D11_BIND_DEPTH_STENCIL;
    DX_ASSERT(mDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

    // create view of depth stensil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format                        = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension                 = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice            = 0u;
    DX_ASSERT(mDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &mDSV));

    // bind depth stensil view to OM
    mDeviceContext->OMSetRenderTargets(1u, mRenderTargetView.GetAddressOf(), mDSV.Get());

    // configure viewport
    D3D11_VIEWPORT vp;
    vp.Width    = Const::WindowWidthInitial;
    vp.Height   = Const::WindowHeightInitial;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    mDeviceContext->RSSetViewports(1u, &vp);
}
void Dx11Context::SwapBuffers()
{
    mSwapChain->Present(0, 0);
}
void Dx11Context::Clear()
{
    static float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};   // RGBA = (0,0,0,255)
    mDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), black);
}
}   // namespace SP::RHI
