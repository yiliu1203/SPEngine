#pragma once
#include "PCH.h"
#include <dxgidebug.h>
#include <vector>
#include <wrl.h>

namespace SP::RHI {
class DxgiInfoManager
{
public:
    static DxgiInfoManager& Instance();
    NoCopyable(DxgiInfoManager);
    DxgiInfoManager();
    ~DxgiInfoManager() = default;

    void                     Set() noexcept;
    std::vector<std::string> GetMessages() const;

private:
    unsigned long long                     next = 0u;
    Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};
}   // namespace SP::RHI