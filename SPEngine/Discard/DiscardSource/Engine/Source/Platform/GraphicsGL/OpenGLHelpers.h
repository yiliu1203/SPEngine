#pragma once
#include "PCH.h"
#include "Graphics/RHIConst.h"
#include "glad/glad.h"
namespace SP::RHI {

template <E_DataType dt>
struct OpenGLDataType
{
    /* data */
};



template <>
struct OpenGLDataType<E_DataType::Floatx1>
{
    constexpr static GLenum GLDataType = GL_FLOAT;
    constexpr static size_t ByteSize   = sizeof(float);
};
template <>
struct OpenGLDataType<E_DataType::Floatx2>
{
    constexpr static GLenum GLDataType = GL_FLOAT;
    constexpr static size_t ByteSize   = sizeof(float) * 3;
};

template <>
struct OpenGLDataType<E_DataType::Floatx3>
{
    constexpr static GLenum GLDataType = GL_FLOAT;
    constexpr static size_t ByteSize   = sizeof(float) * 3;
};

template <>
struct OpenGLDataType<E_DataType::Floatx4>
{
    constexpr static GLenum GLDataType = GL_FLOAT;
    constexpr static size_t ByteSize   = sizeof(float) * 4;
};

using OpenGLDataConfig = std::tuple<OpenGLDataType<E_DataType::Floatx1>, OpenGLDataType<E_DataType::Floatx2>,
                                    OpenGLDataType<E_DataType::Floatx3>, OpenGLDataType<E_DataType::Floatx4> >;

struct TexutreDataFormateInfo
{
    E_TextureDataFormat index;
    GLenum              InternalFormat;
    GLenum              DataFormate;
    uint32              bpp;
};


static constexpr TexutreDataFormateInfo g_texture_data_formate_config[] = {{E_TextureDataFormat::None, 0, 0, 0},
                                                                           {E_TextureDataFormat::RGB8, GL_RGB8, GL_RGB, 3},
                                                                           {E_TextureDataFormat::RGBA8, GL_RGBA8, GL_RGBA, 4}};
static_assert(static_cast<size_t>(E_TextureDataFormat::END) ==
              sizeof(g_texture_data_formate_config) / sizeof(TexutreDataFormateInfo));



struct GLDataInfo
{
    GLenum GLDataType;
    size_t stride;
};

GLDataInfo GetGLDataType(E_DataType datatype);

// ShaderType

struct ShaderTypeInfo
{
    E_ShaderType SP_ShaderType;
    GLenum       GL_ShaderType;
};
static constexpr ShaderTypeInfo g_shader_type_config[] = {{E_ShaderType::VertexShader, GL_VERTEX_SHADER},
                                                          {E_ShaderType::FragmentShader, GL_FRAGMENT_SHADER}};


}   // namespace SP::RHI
