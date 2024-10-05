#pragma once

namespace SP {

struct Const
{
    static constexpr int   WindowWidthInitial  = 1600;
    static constexpr int   WindowHeightInitial = 1200;
    static constexpr float Near                = 0.0f;
    static constexpr float Far                 = 1000.0f;
};

// clang-format off
struct SquareMesh
{

    static constexpr float Vertex_XYZ_RGB[] =
    {
        // posisition            colors
        -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,
    };
    static constexpr float Vertex_XYZ_RGB_UV[] =
    {
        // posisition            colors             UV
        -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
    static constexpr uint32_t indices[] = {0, 1, 2, 0, 2, 3};
};
// clang-format on



}   // namespace SP