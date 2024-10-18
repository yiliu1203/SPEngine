

#include "ConfigSP.h"
#include "d3dx12.h"
#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_3.h>
#include <stdexcept>
#include <tchar.h>
#include <wrl/client.h>
#include <xstring>




#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using Microsoft::WRL::ComPtr;

namespace {
const int WINDOW_WIDTH       = 400;
const int WINDOW_HEIGHT      = 240;
const int BUFFER_COUNT       = 2;
HWND      g_mainWindowHandle = 0;
};   // namespace

namespace ws {

// Helper function for resolving the full path of assets.
std::wstring GetAssetFullPath(const std::string& assetName)
{

    auto engine_path   = std::string(ENGINE_ROOT);
    auto resource_path = engine_path + std::string("/Resource/") + std::string(assetName);

    wchar_t ws[200];
    swprintf(ws, 200, L"%hs", resource_path.c_str());
    return std::wstring(ws);
}

}   // namespace ws

void CHK(HRESULT hr)
{
    if (FAILED(hr)) throw runtime_error("HRESULT is failed value.");
}

struct _pos
{
    float x, y, z, w;
};
struct _color
{
    float r, g, b, a;
};

struct GRS_VERTEX
{
    _pos   m_pos;
    _color m_color;
};


const uint8_t nFrameBackBuffCount = BUFFER_COUNT;
class D3D_Demo
{
    int     iWidth      = 1024;
    int     iHeight     = 768;
    uint8_t nFrameIndex = 0;
    uint8_t nFrame      = 0;

    unsigned nDxgiFactoryFlags  = 0u;
    unsigned nRTVDescriptorSize = 0u;

    float fAspectRadio = 3.0f;

    D3D12_VERTEX_BUFFER_VIEW stVertexBuffView = {};
    uint64_t                 n64FenceValue    = 0u;
    HANDLE                   hFenceEvent      = nullptr;

    ComPtr<IDXGIFactory3>             pIDxgiFactory3;
    ComPtr<IDXGIAdapter1>             pIAdapter;
    ComPtr<ID3D12Device4>             pID3DDevice;
    ComPtr<ID3D12CommandQueue>        pID3DComandQueue;
    ComPtr<IDXGISwapChain1>           pISwapChain1;
    ComPtr<IDXGISwapChain2>           pISwapChain2;
    ComPtr<ID3D12DescriptorHeap>      pIRTVHeap;
    ComPtr<ID3D12Resource>            pIARenderTargets[nFrameBackBuffCount];
    ComPtr<ID3D12CommandAllocator>    pICommandAllocator;
    ComPtr<ID3D12RootSignature>       pIRootSignature;
    ComPtr<ID3D12PipelineState>       pIPipelineState;
    ComPtr<ID3D12GraphicsCommandList> pICommandList;
    ComPtr<ID3D12Resource>            pIVertexBuffer;
    ComPtr<ID3D12Fence>               pIFence;

    HANDLE mFenceEveneHandle = 0;

    UINT64 mFrameCount = 0;

public:
    ID3D12Device* GetDevice() const { return pID3DDevice.Get(); }
    D3D_Demo(int width, int height, HWND hwnd)
        : iWidth(width)
        , iHeight(height)
    {
        // 7. 创建dxgi
        {
            ComPtr<ID3D12Debug> debugController;
            CHK(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
            {
                debugController->EnableDebugLayer();
                // Enable additional debug layers.
                nDxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
        }
        CHK(CreateDXGIFactory2(nDxgiFactoryFlags, IID_PPV_ARGS(pIDxgiFactory3.ReleaseAndGetAddressOf())));
        CHK(pIDxgiFactory3->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));   // 关闭 alt+enter 全屏

        // 8. 创建d3d12的设备对象接口
        for (size_t adapterIndex = 0;
             DXGI_ERROR_NOT_FOUND != pIDxgiFactory3->EnumAdapters1(adapterIndex, pIAdapter.ReleaseAndGetAddressOf());
             adapterIndex++) {
            DXGI_ADAPTER_DESC1 desc1 = {};
            pIAdapter->GetDesc1(&desc1);
            if (desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                continue;
            }
            // 特别注意接口最后一个参数设置为null, 则不会实际创建一个设备，也不用在调用release来释放接口
            // D3D12CreateDevice(pIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device), nullptr);
            // 下面是直接创建好 device
            if (!FAILED(D3D12CreateDevice(
                    pIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(pID3DDevice.ReleaseAndGetAddressOf())))) {
                break;
            }
        }

        // 9. 创建D3D12 的命令队列
        /**
         * 现代显卡上 已经包含多个可以同时并行执行命令的“引擎”了 【3D命令的引擎、执行复制命令的引擎（从CPU内存复制内容到GPU显存或者反之或者GPU内部已经引擎之间）、执行通用计算命令的引擎、视频编码解码的引擎】
         * 在D3D12中，针对这些不同的引擎，需要创建不同的命令队列接口来带代表不同的引擎对象
         * D3D12_COMMAND_LIST_TYPE_DIRECT: 真正含义是说 我们创建的不只是能够执行3D图形命令的队列那么简单，而是说它是图形设备的直接代表物，本质上还可以执行几乎所有的命令，包括图形命令、复制、视频编码解码、捆绑包，
         * 它是3D图形命令队列引擎的超集，基本代表了整个GPU的执行能力，顾名直接
         */
        CHK(pID3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                IID_PPV_ARGS(pICommandAllocator.ReleaseAndGetAddressOf())));
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Type                     = D3D12_COMMAND_LIST_TYPE_DIRECT;
        CHK(pID3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(pID3DComandQueue.ReleaseAndGetAddressOf())));

        CHK(pID3DDevice->CreateCommandList(0,
                                           D3D12_COMMAND_LIST_TYPE_DIRECT,
                                           pICommandAllocator.Get(),
                                           nullptr,
                                           IID_PPV_ARGS(pICommandList.ReleaseAndGetAddressOf())));

        // 10. 创建交换链
        /**
         * D3D12 中具体创建交换链时需要指定一个命令队列，这也是最终呈现画面前，交换链需要确定规则操作是否完全完成了
         */
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount           = nFrameBackBuffCount;
        swapChainDesc.Width                 = width;
        swapChainDesc.Height                = height;
        swapChainDesc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count      = 1;
        swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        CHK(pIDxgiFactory3->CreateSwapChainForHwnd(
            pID3DComandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, pISwapChain1.ReleaseAndGetAddressOf()));

        // create Fence

        CHK(pID3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(pIFence.ReleaseAndGetAddressOf())));
        mFenceEveneHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);

        // CHK(pISwapChain1.As(&pISwapChain2));

        // 11. 创建RTV描述符堆和RT描述符
        /**
         * GPU在使用某段存储之前，需要通过类似描述符之类的概念等价物说清楚这段存储的用途，大小、读写属性、GPU/CPU访问权限等信息，因为交换链的主要目的是用它的缓冲区作为3D图形渲染的目标，所以问哦们就需要
         * 用渲染目标试图描述符告诉GPU这些缓冲区是渲染目标
         */
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors             = nFrameBackBuffCount;
        rtvHeapDesc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        CHK(pID3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(pIRTVHeap.ReleaseAndGetAddressOf())));
        nRTVDescriptorSize = pID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        // 有了RTV描述符堆， 我们就可以创建RTV描述符了
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(pIRTVHeap->GetCPUDescriptorHandleForHeapStart());
        for (size_t i = 0; i < nFrameBackBuffCount; i++) {
            CHK(pISwapChain1->GetBuffer(i, IID_PPV_ARGS(pIARenderTargets[i].ReleaseAndGetAddressOf())));
            pID3DDevice->CreateRenderTargetView(
                pIARenderTargets[i].Get(),
                nullptr,
                rtvHandle);   // 这个描述服还需要cpu handle, 描述符存储在Gpu显存上，由cpuhandle 拿到gpu handle, 最终创建在堆上？
            rtvHandle.Offset(nRTVDescriptorSize);
        }

        // 12. 创建根签名对象接口
        /**
         * 从总体上理解D3D12的话，就是在D3D12中接入了存储管理，所有的调用都是异步并行的方式并且为管理异步调用而加入了同步对象。根签名则是为了整体上统一集中管理之前D3D11中分散在各个资源创建函数参数中存储的Slot和对应的
         * 寄存器序号的对象
         * 
         * 熟悉Shader的话，知道我们在写Shader 的时候就需要指定每种数据比如常量缓冲、顶点寄存器、纹理等资源是对应那个存储槽，及序号。 对于存储槽 我们可以理解为一条从内存向现存传输数据的通道（想象成一个流水槽，也可以及为PCIE 一条通道）
         * 而对于这里的寄存器， 就不是指Cpu上的寄存器，而是指GPu上的寄存器。根据之前的描述，现代GPu在概念上可以理解为一个巨大的SIMD架构的处理器，由于高效并行指令的需要，它在存储管理上非常细分，它的寄存器也是细化分类的，
         * 由常量寄存器，纹理寄存器，采样寄存器等，并且每类寄存器都有若干个，以序号索引来使用。所以我们从Cpu加载资源到GPU上是就需要详细指定那些数据从哪个槽灌入到哪个序号的寄存器上。
         * 
         * 根签名就是描述了整个用于渲染的资源的存储布局：根签名是一个绑定约定，由应用程序定义，着色器使用它来定位他们需要方位的资源
         * 
         */

        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        CHK(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
        CHK(pID3DDevice->CreateRootSignature(0,
                                             signature->GetBufferPointer(),
                                             signature->GetBufferSize(),
                                             IID_PPV_ARGS(pIRootSignature.ReleaseAndGetAddressOf())));

        // 13. 编译shader及创建渲染管线状态对象接口
        /**
         * 从概念上来讲，渲染管线状态就是把原来的光栅化状态，DepthStenicl State、Blend State、各阶段Shader程序、以及输入数据模型（InputLayout)等组合在一个对象中，从而形成一个完整的可重用的Pipeline State Object。
         */

        ID3D10Blob *vs, *ps;
        {
            ID3D10Blob* info;
            UINT        flag = 0;
            flag             = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
            CHK(D3DCompileFromFile(
                ws::GetAssetFullPath("shader/shaders.hlsl").c_str(), nullptr, nullptr, "VSMain", "vs_5_0", flag, 0, &vs, &info));
            CHK(D3DCompileFromFile(
                ws::GetAssetFullPath("shader/shaders.hlsl").c_str(), nullptr, nullptr, "PSMain", "ps_5_0", flag, 0, &ps, &info));

            //CHK(D3DCompileFromFile(L"Default.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", flag, 0, &vs, &info));
            //CHK(D3DCompileFromFile(L"Default.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", flag, 0, &ps, &info));
        }
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        };


        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature                     = pIRootSignature.Get();
        psoDesc.PrimitiveTopologyType              = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.InputLayout                        = {inputElementDescs, _countof(inputElementDescs)};
        // psoDesc.InputLayout.pInputElementDescs     = inputLayout;
        // psoDesc.IBStripCutValue                 = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
        psoDesc.VS.pShaderBytecode              = vs->GetBufferPointer();
        psoDesc.VS.BytecodeLength               = vs->GetBufferSize();
        psoDesc.PS.pShaderBytecode              = ps->GetBufferPointer();
        psoDesc.PS.BytecodeLength               = ps->GetBufferSize();
        psoDesc.RasterizerState                 = CD3DX12_RASTERIZER_DESC(CCCD3DX12_DEFAULT());
        psoDesc.BlendState                      = CD3DX12_BLEND_DESC(CCCD3DX12_DEFAULT());
        psoDesc.DepthStencilState.DepthEnable   = false;
        psoDesc.DepthStencilState.StencilEnable = false;
        psoDesc.SampleMask                      = UINT_MAX;
        psoDesc.NumRenderTargets                = 1;
        psoDesc.RTVFormats[0]                   = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count                = 1;
        CHK(pID3DDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(pIPipelineState.ReleaseAndGetAddressOf())));
        vs->Release();
        ps->Release();

        //14. 加载待渲染数据
        GRS_VERTEX triggleVertices[] = {
            {{0.0f, 0.25f * fAspectRadio, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
            {{0.25f * fAspectRadio, -0.25f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
            {{-0.25f * fAspectRadio, -0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}},
        };

        const size_t vertex_buff_size = sizeof(triggleVertices);
        auto         HEAP_PROPERTIES  = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto         buff             = CD3DX12_RESOURCE_DESC::Buffer(vertex_buff_size);
        CHK(pID3DDevice->CreateCommittedResource(&HEAP_PROPERTIES,
                                                 D3D12_HEAP_FLAG_NONE,
                                                 &buff,
                                                 D3D12_RESOURCE_STATE_GENERIC_READ,
                                                 nullptr,
                                                 IID_PPV_ARGS(pIVertexBuffer.ReleaseAndGetAddressOf())));

        pIVertexBuffer->SetName(L"VertexBuffer");
        char* vbUploadPtr = nullptr;
        CHK(pIVertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&vbUploadPtr)));
        memcpy(vbUploadPtr, triggleVertices, sizeof(triggleVertices));
        pIVertexBuffer->Unmap(0, nullptr);

        stVertexBuffView.BufferLocation = pIVertexBuffer->GetGPUVirtualAddress();
        stVertexBuffView.StrideInBytes  = sizeof(GRS_VERTEX);
        stVertexBuffView.SizeInBytes    = vertex_buff_size;
    };

    void Draw()
    {
        mFrameCount++;

        // Get current RTV descriptor
        auto descHandleRtvStep                    = pID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE descHandleRtv = pIRTVHeap->GetCPUDescriptorHandleForHeapStart();
        descHandleRtv.ptr += ((mFrameCount - 1) % BUFFER_COUNT) * descHandleRtvStep;
        // Get current swap chain
        ID3D12Resource* d3dBuffer = pIARenderTargets[(mFrameCount - 1) % BUFFER_COUNT].Get();

        // Barrier Present -> RenderTarget
        setResourceBarrier(pICommandList.Get(), d3dBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

        // Viewport & Scissor
        D3D12_VIEWPORT viewport = {};
        viewport.Width          = (float)iWidth;
        viewport.Height         = (float)iHeight;
        pICommandList->RSSetViewports(1, &viewport);
        D3D12_RECT scissor = {};
        scissor.right      = (LONG)iWidth;
        scissor.bottom     = (LONG)iHeight;
        pICommandList->RSSetScissorRects(1, &scissor);

        // Clear
        {
            auto         saturate = [](float a) { return a < 0 ? 0 : a > 1 ? 1 : a; };
            float        clearColor[4];
            static float h = 0.0f;
            h += 0.02f;
            if (h >= 1) h = 0.0f;
            clearColor[0] = saturate(std::abs(h * 6.0f - 3.0f) - 1.0f);
            clearColor[1] = saturate(2.0f - std::abs(h * 6.0f - 2.0f));
            clearColor[2] = saturate(2.0f - std::abs(h * 6.0f - 4.0f));
            pICommandList->ClearRenderTargetView(descHandleRtv, clearColor, 0, nullptr);
        }

        pICommandList->OMSetRenderTargets(1, &descHandleRtv, true, nullptr);

        // Draw
        {
            pICommandList->SetGraphicsRootSignature(pIRootSignature.Get());
            //pICommandList->SetDescriptorHeaps()
            pICommandList->SetPipelineState(pIPipelineState.Get());
            pICommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            pICommandList->IASetVertexBuffers(0, 1, &stVertexBuffView);
            // pICommandList->IASetIndexBuffer(&mIBView);
            // pICommandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
            pICommandList->DrawInstanced(3, 1, 0, 0);
        }

        // Barrier RenderTarget -> Present
        setResourceBarrier(pICommandList.Get(), d3dBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

        // Exec
        CHK(pICommandList->Close());
        ID3D12CommandList* const ppCommandLists[] = {pICommandList.Get()};
        pID3DComandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        // Present
        CHK(pISwapChain1->Present(1, 0));

        // Set queue flushed event
        CHK(pIFence->SetEventOnCompletion(mFrameCount, mFenceEveneHandle));

        // Wait for queue flushed
        // This code would occur CPU stall!
        CHK(pID3DComandQueue->Signal(pIFence.Get(), mFrameCount));
        DWORD wait = WaitForSingleObject(mFenceEveneHandle, 10000);
        if (wait != WAIT_OBJECT_0) throw runtime_error("Failed WaitForSingleObject().");

        CHK(pICommandAllocator->Reset());
        CHK(pICommandList->Reset(pICommandAllocator.Get(), nullptr));
    }

    void setResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* res, D3D12_RESOURCE_STATES before,
                            D3D12_RESOURCE_STATES after)
    {
        D3D12_RESOURCE_BARRIER desc = {};
        desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        desc.Transition.pResource   = res;
        desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        desc.Transition.StateBefore = before;
        desc.Transition.StateAfter  = after;
        desc.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        commandList->ResourceBarrier(1, &desc);
    }
};


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC         hdc;

    switch (message) {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostMessage(hWnd, WM_DESTROY, 0, 0);
            return 0;
        }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY: PostQuitMessage(0); break;

    default: return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

static HWND setupWindow(int width, int height)
{
    WNDCLASSEX wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = (HMODULE)GetModuleHandle(0);
    wcex.hIcon         = nullptr;
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = _T("WindowClass");
    wcex.hIconSm       = nullptr;
    if (!RegisterClassEx(&wcex)) {
        throw runtime_error("RegisterClassEx()");
    }

    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    const int windowWidth  = (rect.right - rect.left);
    const int windowHeight = (rect.bottom - rect.top);

    HWND hWnd = CreateWindow(_T("WindowClass"),
                             _T("Window"),
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             0,
                             windowWidth,
                             windowHeight,
                             nullptr,
                             nullptr,
                             nullptr,
                             nullptr);
    if (!hWnd) {
        throw runtime_error("CreateWindow()");
    }

    return hWnd;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    MSG msg;
    ZeroMemory(&msg, sizeof msg);

    ID3D12Device* dev = nullptr;

#ifdef NDEBUG
    try
#endif
    {
        g_mainWindowHandle = setupWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
        ShowWindow(g_mainWindowHandle, SW_SHOW);
        UpdateWindow(g_mainWindowHandle);

        D3D_Demo d3d(WINDOW_WIDTH, WINDOW_HEIGHT, g_mainWindowHandle);
        dev = d3d.GetDevice();

        while (msg.message != WM_QUIT) {
            BOOL r = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
            if (r == 0) {
                d3d.Draw();
            }
            else {
                DispatchMessage(&msg);
            }
        }
    }
#ifdef NDEBUG
    catch (std::exception& e) {
        MessageBoxA(g_mainWindowHandle, e.what(), "Exception occuured.", MB_ICONSTOP);
    }
#endif

    // if (dev) dev->Release();

    return static_cast<int>(msg.wParam);
}
