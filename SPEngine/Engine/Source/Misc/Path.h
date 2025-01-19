#pragma once
#include "PCH.h"
#include "ConfigSP.h"



namespace SP {
class Path
{

public:
    static const std::filesystem::path project_root;
    static const std::filesystem::path engine_root;
    static const std::filesystem::path res_root;
    static const std::filesystem::path shader_dir;

    static const std::string res_root_string;



    static void InitModule() {}
};
}   // namespace SP