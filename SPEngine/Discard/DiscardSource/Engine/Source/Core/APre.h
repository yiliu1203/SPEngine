#pragma once
// clang-format off
#include "ConfigSP.h"
#include "_Macro_.h"
#include "_Type.h"
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
#include "Macro.h"
#include "Exception.h"
#include "Log.h"
#include "Misc/KeyCode.h"
#include "Misc/Input.h"
#include "Misc/Const.h"
#include <ctime>
#include <string>
#include <chrono>
#include "Misc/KeyCode.h"



namespace SP
{   
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using Scope = std::unique_ptr<T>;

}




// clang-format on