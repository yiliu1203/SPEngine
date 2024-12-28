#pragma once

namespace SP {
namespace RHI {

#ifndef API_CURRENT
#    define API_CURRENT 0
#endif

enum E_API
{
    None   = 0,
    OpenGL = 1,
    DX11   = 2,
};
constexpr int g_api = API_CURRENT;


enum class E_DataType
{
    None    = 0,
    Floatx1 = 1,
    Floatx2 = 2,
    Floatx3 = 3,
    Floatx4 = 4,
};

enum class E_CameraOrtho
{
    None        = 0,
    Orthogonal  = 1,
    Perspective = 2,
};


enum class E_TextureDataFormat
{
    None  = 0,
    RGB8  = 1,
    RGBA8 = 2,
    END
};

enum E_ShaderType
{
    VertexShader   = 0,
    FragmentShader = 1,
    ShaderTypeEnd  = 2,
};


enum class FramebuffTextureFormat
{
    None = 0,
    RGBA8,
    DEPTH24STENCIL8,
    END,

};


}   // namespace RHI
}   // namespace SP