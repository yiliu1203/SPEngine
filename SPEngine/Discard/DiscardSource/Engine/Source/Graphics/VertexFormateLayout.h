#pragma once
#include "PCH.h"
#include "Core/FixedString.h"
#include "Refl/Refl.h"


namespace SP::RHI {

enum class E_VertexFormateLayout
{
    VFL_none = 0,
    VFL_f3xyz,               // float xyz
    VFL_f3xyz_f3rgb,         // float xyz, float rgb
    VFL_f3xyz_f4rgba,        // float xyz, float rgba
    VFL_f3xyz_f2uv,          // float xyz, float uv
    VFL_f3xyz_f3rgb_f2uv,    // float xyz, float rgb, float uv
    VFL_f3xyz_f4rgba_f2uv,   // float xyz, float rgba, float uv
    VFL_end,
};




constexpr bool IsNormaized(const std::string_view& vfl_comp_str)
{
    return false;
}


constexpr E_DataType VFLGetElementType(const std::string_view& vfl_comp_str)
{
    if (vfl_comp_str.starts_with("f1"))
        return E_DataType::Floatx1;
    else if (vfl_comp_str.starts_with("f2"))
        return E_DataType::Floatx2;
    if (vfl_comp_str.starts_with("f3"))
        return E_DataType::Floatx3;
    else if (vfl_comp_str.starts_with("f4"))
        return E_DataType::Floatx4;
    return E_DataType::None;
}

constexpr size_t VFLGetElementCount(const std::string_view& vfl_comp_str)
{
    if (vfl_comp_str.starts_with("f1"))
        return 1;
    else if (vfl_comp_str.starts_with("f2"))
        return 2;
    if (vfl_comp_str.starts_with("f3"))
        return 3;
    else if (vfl_comp_str.starts_with("f4"))
        return 4;
    return 0;
}

constexpr size_t VFLGetElementByteSize(const std::string_view& vfl_comp_str)
{
    if (vfl_comp_str.starts_with("f1"))
        return sizeof(float);
    else if (vfl_comp_str.starts_with("f2"))
        return sizeof(float);
    if (vfl_comp_str.starts_with("f3"))
        return sizeof(float);
    else if (vfl_comp_str.starts_with("f4"))
        return sizeof(float);
    return 0;
}


struct VertexCompInfo
{
    constexpr VertexCompInfo(const std::string_view& vfl_comp_str)
        : is_normaized{IsNormaized(vfl_comp_str)}
        , data_type{VFLGetElementType(vfl_comp_str)}
        , element_count{VFLGetElementCount(vfl_comp_str)}
        , element_byte_size{VFLGetElementByteSize(vfl_comp_str)}
    {}
    const bool       is_normaized;
    const E_DataType data_type;
    const size_t     element_count;
    const size_t     element_byte_size;
};





struct VertexFormateTraitsBase
{
    static constexpr size_t           value      = 0;
    static constexpr std::string_view name       = "";
    static constexpr size_t           comp_count = 0;
};


constexpr size_t VFLGetCompCount(const std::string_view& vertex_formate_str)
{
    int count = 0;
    int begin = 3;
    while (vertex_formate_str.find_first_of("_", begin) != vertex_formate_str.npos) {
        begin = vertex_formate_str.find_first_of("_", begin) + 1;
        count++;
    }
    return count;
}




template <E_VertexFormateLayout vfl, typename VertexFormateInfo = refl::EnumItemInfo<E_VertexFormateLayout, vfl>>
struct VertexFormateLayoutTraits : public VertexFormateTraitsBase
{
    static constexpr size_t           value      = VertexFormateInfo::value;
    static constexpr std::string_view name       = VertexFormateInfo::item_name;   // -> VFL_fxyz
    static constexpr size_t           comp_count = VFLGetCompCount(name);
};

// struct VFLConfig
// {
//     using none_t       = VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_none>;
//     using f3xyz_t      = VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz>;
//     using f3xyz_f2uv_t = VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz_f2uv>;
// };


struct VFLInfo
{
    // VFLInfo() = default;
    size_t                      comp_count;
    size_t                      stride_bytes;
    std::vector<VertexCompInfo> comp_list;
    VFLInfo(E_VertexFormateLayout layout_value, const std::string_view& layout_str);
};

struct VFLConfig
{
    VFLInfo config[(size_t)E_VertexFormateLayout::VFL_end];
    VFLConfig()
        : config{
              VFLInfo{E_VertexFormateLayout::VFL_none, VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_none>::name},
              VFLInfo{E_VertexFormateLayout::VFL_f3xyz, VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz>::name},
              VFLInfo{E_VertexFormateLayout::VFL_f3xyz_f3rgb,
                      VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz_f3rgb>::name},
              VFLInfo{E_VertexFormateLayout::VFL_f3xyz_f4rgba,
                      VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz_f4rgba>::name},
              VFLInfo{E_VertexFormateLayout::VFL_f3xyz_f2uv,
                      VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz_f2uv>::name},
              VFLInfo{E_VertexFormateLayout::VFL_f3xyz_f3rgb_f2uv,
                      VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz_f3rgb_f2uv>::name},
              VFLInfo{E_VertexFormateLayout::VFL_f3xyz_f4rgba_f2uv,
                      VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz_f4rgba_f2uv>::name},
          }
    {}
    const VFLInfo& operator()(E_VertexFormateLayout index) { return config[(size_t)index]; }
};

extern VFLConfig g_vfl_config;


struct VertexFormateLayoutBaseInfo
{
    E_VertexFormateLayout index;
    uint8                 comp_count;
    std::string_view      name;
};
static VertexFormateLayoutBaseInfo g_vfl_layout_base_info[] = {
    {E_VertexFormateLayout::VFL_none, 0, ""},
    {E_VertexFormateLayout::VFL_f3xyz,
     VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz>::comp_count,
     VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz>::name},


};   // namespace SP::RHI


using VertexFormateLayoutTraits_VFL_fxyz = VertexFormateLayoutTraits<E_VertexFormateLayout::VFL_f3xyz>;


}   // namespace SP::RHI