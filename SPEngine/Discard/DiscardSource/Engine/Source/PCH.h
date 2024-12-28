#pragma once

// clang-format off
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string_view>
#include <sstream>
#include <any>
#include <functional>
#include <tuple>
#include <fstream>
#include <filesystem>
#include <variant> 
#include <ctime>
#include <string>
#include <chrono>
#include <array>
#include <initializer_list>
#include <bitset>
#include <queue>

namespace SP
{   
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using Scope = std::unique_ptr<T>;

    // 完美转发 创建Ref
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    // Ref向上转换
    template<typename T1, typename T2>
    constexpr Ref<T1> RefCastUp(const Ref<T2>& other) noexcept
    {
        return std::static_pointer_cast<T1>(other);
    }
}

#include "ConfigSP.h"
#include "Core/_Macro_.h"
#include "Core/_Type.h"
#include "Core/Macro.h"
#include "Core/Exception.h"
#include "Core/Log.h"
#include "Misc/KeyCode.h"
#include "Misc/Input.h"
#include "Misc/Const.h"
#include "Misc/KeyCode.h"

#include "Graphics/RHIConst.h"
#include "Core/Exception.h"






// clang-format on