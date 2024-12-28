#include "VertexFormateLayout.h"

namespace SP::RHI {

VFLConfig g_vfl_config;


VFLInfo::VFLInfo(E_VertexFormateLayout layout_value, const std::string_view& layout_str)
    : comp_count{VFLGetCompCount(layout_str)}
{
    int    begin     = 4;
    size_t end       = layout_str.npos;
    int    byte_size = 0;
    while ((end = layout_str.find_first_of("_", begin)) != layout_str.npos) {
        const std::string_view sub{layout_str.begin() + begin, layout_str.begin() + end};
        auto                   subcomp = VertexCompInfo{sub};
        comp_list.push_back(subcomp);
        begin = end + 1;
        byte_size += subcomp.element_byte_size * subcomp.element_count;
    }
    const std::string_view sub{layout_str.begin() + begin, layout_str.end()};
    auto                   subcomp = VertexCompInfo{sub};
    comp_list.push_back(subcomp);
    byte_size += subcomp.element_byte_size * subcomp.element_count;
    stride_bytes = byte_size;
    SP_ASSERT(comp_list.size() == comp_count);
}
}   // namespace SP::RHI
