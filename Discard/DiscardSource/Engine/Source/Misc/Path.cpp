#include "Path.h"

namespace SP {
const std::filesystem::path Path::project_root = PROJECT_ROOT;
const std::filesystem::path Path::engine_root  = Path::project_root / "Source" / "Engine";
const std::filesystem::path Path::res_root     = Path::project_root / "Source" / "Resource";
const std::filesystem::path Path::shader_dir   = Path::res_root / "shaders";

const std::string Path::res_root_string = std::string(PROJECT_ROOT) + "/Source/Resource";

}   // namespace SP