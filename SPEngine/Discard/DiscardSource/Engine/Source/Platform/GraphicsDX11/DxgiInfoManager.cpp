#include "DxgiInfoManager.h"
#include <dxgidebug.h>
#include <memory>



#pragma comment(lib, "dxguid.lib")
namespace SP::RHI {

DxgiInfoManager& DxgiInfoManager::Instance()
{
    static DxgiInfoManager dxgiInfoManager;
    return dxgiInfoManager;
}

DxgiInfoManager::DxgiInfoManager()
{
    // define function signature of DXGIGetDebugInterface
    typedef HRESULT(WINAPI * DXGIGetDebugInterface)(REFIID, void**);

    // load the dll that contains the function DXGIGetDebugInterface
    const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    ASSERT(hModDxgiDebug != nullptr);

    // get address of DXGIGetDebugInterface in dll
    const auto DxgiGetDebugInterface =
        reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));
    ASSERT(DxgiGetDebugInterface != nullptr);

    HRESULT hr;
    DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDxgiInfoQueue);
}

void DxgiInfoManager::Set() noexcept
{
    // set the index (next) so that the next all to GetMessages()
    // will only get errors generated after this call
    next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
    std::vector<std::string> messages;
    const auto               end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    for (auto i = next; i < end; i++) {
        HRESULT hr;
        SIZE_T  messageLength;
        // get the size of message i in bytes
        DX_ASSERT(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
        // allocate memory for message
        auto bytes    = std::make_unique<byte[]>(messageLength);
        auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
        // get the message and push its description into the vector
        DX_ASSERT(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
        messages.emplace_back(pMessage->pDescription);
    }
    return messages;
}
}   // namespace SP::RHI