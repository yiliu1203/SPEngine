﻿//***************************************************************************************
// BoxApp2.cpp by Frank Luna (C) 2015 All Rights Reserved.
//
// Shows how to draw a box in Direct3D 12.
//
// Controls:
//   Hold the left mouse button down and move the mouse to rotate.
//   Hold the right mouse button down and move the mouse to zoom in and out.
//***************************************************************************************

#include "Common/MathHelper.h"
#include "Common/UploadBuffer.h"
#include "Common/d3dApp.h"
#include <iostream>


using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct ObjectConstants
{
    XMFLOAT4X4 World = MathHelper::Identity4x4();
};

struct PassConstants
{
    XMFLOAT4X4 ViewProj = MathHelper::Identity4x4();
};


class ShapesApp : public D3DApp
{
public:
    ShapesApp(HINSTANCE hInstance);
    ShapesApp(const ShapesApp& rhs)            = delete;
    ShapesApp& operator=(const ShapesApp& rhs) = delete;
    ~ShapesApp();

    virtual bool Initialize() override;

private:
    virtual void OnResize() override;
    virtual void Update(const GameTimer& gt) override;
    virtual void Draw(const GameTimer& gt) override;

    virtual void OnMouseDown(WPARAM btnState, int x, int y) override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y) override;

    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildBoxGeometry();
    void BuildPSO();

private:
    ComPtr<ID3D12RootSignature>  mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap       = nullptr;

    std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
    std::unique_ptr<UploadBuffer<PassConstants>>   mPassCB   = nullptr;

    std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

    ComPtr<ID3DBlob> mvsByteCode = nullptr;
    ComPtr<ID3DBlob> mpsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    ComPtr<ID3D12PipelineState> mPSO = nullptr;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView  = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj  = MathHelper::Identity4x4();

    float mTheta  = 1.5f * XM_PI;
    float mPhi    = XM_PIDIV4;
    float mRadius = 5.0f;

    POINT mLastMousePos;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try {
        ShapesApp theApp(hInstance);
        if (!theApp.Initialize()) return 0;

        return theApp.Run();
    }
    catch (DxException& e) {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}

ShapesApp::ShapesApp(HINSTANCE hInstance)
    : D3DApp(hInstance)
{}

ShapesApp::~ShapesApp() {}

bool ShapesApp::Initialize()
{
    if (!D3DApp::Initialize()) return false;

    // Reset the command list to prep for initialization commands.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    BuildDescriptorHeaps();
    BuildConstantBuffers();
    BuildRootSignature();
    BuildShadersAndInputLayout();
    BuildBoxGeometry();
    BuildPSO();

    // Execute the initialization commands.
    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList* cmdsLists[] = {mCommandList.Get()};
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until initialization is complete.
    FlushCommandQueue();

    return true;
}

void ShapesApp::OnResize()
{
    D3DApp::OnResize();

    // The window resized, so update the aspect ratio and recompute the projection matrix.
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

void ShapesApp::Update(const GameTimer& gt)
{
    // Convert Spherical to Cartesian coordinates.
    float x = mRadius * sinf(mPhi) * cosf(mTheta);
    float z = mRadius * sinf(mPhi) * sinf(mTheta);
    float y = mRadius * cosf(mPhi);

    // Build the view matrix.
    XMVECTOR pos    = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&mView, view);

    auto trans0 = XMMatrixTranslation(0, 0, 0);

    XMMATRIX world         = XMLoadFloat4x4(&mWorld);
    XMMATRIX proj          = XMLoadFloat4x4(&mProj);
    XMMATRIX worldViewProj = trans0 * world * view * proj;

    // Update the constant buffer with the latest worldViewProj matrix.
    ObjectConstants objConstants;
    XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(trans0 * world));
    mObjectCB->CopyData(0, objConstants);

    PassConstants passConstants;
    XMStoreFloat4x4(&passConstants.ViewProj, XMMatrixTranspose(view * proj));
    mPassCB->CopyData(0, passConstants);
}

void ShapesApp::Draw(const GameTimer& gt)
{
    // Reuse the memory associated with command recording.
    // We can only reset when the associated command lists have finished execution on the GPU.
    std::cout << "Draw" << std::endl;
    ThrowIfFailed(mDirectCmdListAlloc->Reset());

    // A command list can be reset after it has been added to the command queue via ExecuteCommandList.
    // Reusing the command list reuses memory.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get()));

    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // Indicate a state transition on the resource usage.
    auto barrier0 = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    mCommandList->ResourceBarrier(1, &barrier0);

    // Clear the back buffer and depth buffer.
    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // Specify the buffers we are going to render to.
    auto depthStencilView      = DepthStencilView();
    auto currentBackBufferView = CurrentBackBufferView();
    mCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);

    ID3D12DescriptorHeap* descriptorHeaps[] = {mCbvHeap.Get()};
    mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

    auto vertexBuffView = mBoxGeo->VertexBufferView();
    auto indexBuffView  = mBoxGeo->IndexBufferView();
    mCommandList->IASetVertexBuffers(0, 1, &vertexBuffView);
    mCommandList->IASetIndexBuffer(&indexBuffView);
    mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /**
     * 在Draw函数中，我们会使用ID3D12GraphicsCommandList::SetGraphicsRootDescriptorTable和着色器寄存槽绑定，
     * 它告诉寄存槽我们在设置根签名时传入的CBV具体是什么，而现在我们因为有2个描述符表，所以要绑定两次。
        注意：根参数的起始索引要和根描述符表的地址（即CBV堆中的元素地址）对应。
     */
    CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
    mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);
    cbvHandle.Offset(1, mCbvSrvUavDescriptorSize);
    mCommandList->SetGraphicsRootDescriptorTable(1, cbvHandle);

    mCommandList->DrawIndexedInstanced(mBoxGeo->DrawArgs["box"].IndexCount, 1, mBoxGeo->DrawArgs["box"].StartIndexLocation, mBoxGeo->DrawArgs["box"].BaseVertexLocation, 0);

    // CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandlePyramid(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
    // cbvHandlePyramid.Offset(1, mCbvSrvUavDescriptorSize);
    // mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandlePyramid);
    // mCommandList->DrawIndexedInstanced(
    //     mBoxGeo->DrawArgs["pyramid"].IndexCount, 1, mBoxGeo->DrawArgs["pyramid"].StartIndexLocation, mBoxGeo->DrawArgs["pyramid"].BaseVertexLocation, 0);

    // Indicate a state transition on the resource usage.
    auto barrier1 = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    mCommandList->ResourceBarrier(1, &barrier1);

    // Done recording commands.
    ThrowIfFailed(mCommandList->Close());

    // Add the command list to the queue for execution.
    ID3D12CommandList* cmdsLists[] = {mCommandList.Get()};
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // swap the back and front buffers
    ThrowIfFailed(mSwapChain->Present(0, 0));
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

    // Wait until frame commands are complete.  This waiting is inefficient and is
    // done for simplicity.  Later we will show how to organize our rendering code
    // so we do not have to wait per frame.
    FlushCommandQueue();
}

void ShapesApp::OnMouseDown(WPARAM btnState, int x, int y)
{
    mLastMousePos.x = x;
    mLastMousePos.y = y;

    SetCapture(mhMainWnd);
}

void ShapesApp::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void ShapesApp::OnMouseMove(WPARAM btnState, int x, int y)
{
    if ((btnState & MK_LBUTTON) != 0) {
        // Make each pixel correspond to a quarter of a degree.
        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

        // Update angles based on input to orbit camera around box.
        mTheta += dx;
        mPhi += dy;

        // Restrict the angle mPhi.
        mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
    }
    else if ((btnState & MK_RBUTTON) != 0) {
        // Make each pixel correspond to 0.005 unit in the scene.
        float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
        float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

        // Update the camera radius based on input.
        mRadius += dx - dy;

        // Restrict the radius.
        mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}

void ShapesApp::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
    cbvHeapDesc.NumDescriptors = 2;
    cbvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvHeapDesc.NodeMask       = 0;
    ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));
}

void ShapesApp::BuildConstantBuffers()
{
    mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), 1, true);

    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

    D3D12_GPU_VIRTUAL_ADDRESS       cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
    cbvDesc.BufferLocation = cbAddress;
    cbvDesc.SizeInBytes    = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
    auto cbvHandle         = mCbvHeap->GetCPUDescriptorHandleForHeapStart();
    md3dDevice->CreateConstantBufferView(&cbvDesc, cbvHandle);

    // create pass const buff

    mPassCB = std::make_unique<UploadBuffer<PassConstants>>(md3dDevice.Get(), 1, true);

    D3D12_GPU_VIRTUAL_ADDRESS       passCBAddress = mPassCB->Resource()->GetGPUVirtualAddress();
    D3D12_CONSTANT_BUFFER_VIEW_DESC passCbvDesc;
    passCbvDesc.BufferLocation = passCBAddress;
    passCbvDesc.SizeInBytes    = d3dUtil::CalcConstantBufferByteSize(sizeof(PassConstants));
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mCbvHeap->GetCPUDescriptorHandleForHeapStart());
    handle.Offset(1, mCbvSrvUavDescriptorSize);
    md3dDevice->CreateConstantBufferView(&passCbvDesc, handle);
}

void ShapesApp::BuildRootSignature()
{
    // Shader programs typically require resources as input (constant buffers,
    // textures, samplers).  The root signature defines the resources the shader
    // programs expect.  If we think of the shader programs as a function, and
    // the input resources as function parameters, then the root signature can be
    // thought of as defining the function signature.

    // Root parameter can be a table, root descriptor or root constants.
    CD3DX12_ROOT_PARAMETER slotRootParameter[2];

    // Create a single descriptor table of CBVs for Object
    CD3DX12_DESCRIPTOR_RANGE objCbvTable;
    objCbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    slotRootParameter[0].InitAsDescriptorTable(1, &objCbvTable);

    // Create a single descriptor table of CBVs for Pass
    CD3DX12_DESCRIPTOR_RANGE passCbvTable;
    passCbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);   // 特别注意这里的参数1
    slotRootParameter[1].InitAsDescriptorTable(1, &passCbvTable);


    // A root signature is an array of root parameters.
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
    ComPtr<ID3DBlob> serializedRootSig = nullptr;
    ComPtr<ID3DBlob> errorBlob         = nullptr;
    HRESULT          hr                = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

    if (errorBlob != nullptr) {
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }
    ThrowIfFailed(hr);
    ThrowIfFailed(md3dDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
}

void ShapesApp::BuildShadersAndInputLayout()
{
    HRESULT hr = S_OK;

    mvsByteCode = d3dUtil::CompileShader(L"shader/008_box3.hlsl", nullptr, "VS", "vs_5_0");
    mpsByteCode = d3dUtil::CompileShader(L"shader/008_box3.hlsl", nullptr, "PS", "ps_5_0");

    mInputLayout = {{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}};
}

void ShapesApp::BuildBoxGeometry()
{
    std::array<Vertex, 8> vertices = {Vertex({XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White)}),
                                      Vertex({XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black)}),
                                      Vertex({XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red)}),
                                      Vertex({XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green)}),
                                      Vertex({XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue)}),
                                      Vertex({XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow)}),
                                      Vertex({XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan)}),
                                      Vertex({XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta)})

    };
    // clang-format off
    std::array<std::uint16_t, 36> indices = {
        // front face
        0,1, 2, 0,2,3,

        // back face
        4,6,5,4,7,6,

        // left face
        4,5,1,4,1,0,

        // right face
        3,2,6,3,6,7,

        // top face
        1,5,6,1,6,2,

        // bottom face
        4,0,3,4,3,7,
    };

    std::array<Vertex, 5> verticesPyramid = {
        // 三棱锥
        Vertex({XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(Colors::Red)}),
        Vertex({XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(Colors::Green)}),
        Vertex({XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(Colors::Green)}),
        Vertex({XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(Colors::Green)}),
        Vertex({XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(Colors::Green)}),
    };


    std::array<std::uint16_t, 18> indicesPyramid = {
        // 三棱锥
        // 前
        0,2,1,

        // 后
        0,4,3,

        // 左
        0,1,4,

        // 右
        0,3,2,

        // 下
        2,4,1,2,3,4};
    // clang-format on

    // merge vertice and indices
    std::vector<Vertex> mergedVertices{vertices.begin(), vertices.end()};
    mergedVertices.insert(mergedVertices.end(), verticesPyramid.begin(), verticesPyramid.end());
    std::vector<std::uint16_t> mergedIndices{indices.begin(), indices.end()};
    mergedIndices.insert(mergedIndices.end(), indicesPyramid.begin(), indicesPyramid.end());

    const UINT vbByteSize = (UINT)mergedVertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)mergedIndices.size() * sizeof(std::uint16_t);

    mBoxGeo       = std::make_unique<MeshGeometry>();
    mBoxGeo->Name = "boxGeo";

    ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
    CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), mergedVertices.data(), vbByteSize);

    ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
    CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), mergedIndices.data(), ibByteSize);

    mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), mergedVertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

    mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), mergedIndices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

    mBoxGeo->VertexByteStride     = sizeof(Vertex);
    mBoxGeo->VertexBufferByteSize = vbByteSize;
    mBoxGeo->IndexFormat          = DXGI_FORMAT_R16_UINT;
    mBoxGeo->IndexBufferByteSize  = ibByteSize;

    SubmeshGeometry submesh;
    submesh.IndexCount         = (UINT)indices.size();
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    SubmeshGeometry submeshPyramid;
    submeshPyramid.IndexCount         = (UINT)indicesPyramid.size();
    submeshPyramid.StartIndexLocation = (UINT)indices.size();
    submeshPyramid.BaseVertexLocation = (UINT)vertices.size();

    mBoxGeo->DrawArgs["box"]     = submesh;
    mBoxGeo->DrawArgs["pyramid"] = submeshPyramid;
}

void ShapesApp::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout           = {mInputLayout.data(), (UINT)mInputLayout.size()};
    psoDesc.pRootSignature        = mRootSignature.Get();
    psoDesc.VS                    = {reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()), mvsByteCode->GetBufferSize()};
    psoDesc.PS                    = {reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()), mpsByteCode->GetBufferSize()};
    psoDesc.RasterizerState       = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState            = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState     = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask            = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets      = 1;
    psoDesc.RTVFormats[0]         = mBackBufferFormat;
    psoDesc.SampleDesc.Count      = m4xMsaaState ? 4 : 1;
    psoDesc.SampleDesc.Quality    = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    psoDesc.DSVFormat             = mDepthStencilFormat;
    ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}