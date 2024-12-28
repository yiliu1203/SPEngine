#pragma once
#include "PCH.h"
#include "Math/mathfwd.h"
namespace SP {

using ShaderUniformValueType = std::variant<int, float, math::vec2, math::vec3, math::vec4, math::mat4>;

class UniformParam
{
public:
    using iterator                               = std::map<std::string, ShaderUniformValueType>::iterator;
    using const_iterator                         = std::map<std::string, ShaderUniformValueType>::const_iterator;
    UniformParam()                               = default;
    UniformParam(const UniformParam&)            = default;
    UniformParam& operator=(const UniformParam&) = default;
    explicit UniformParam(const std::string& key, const ShaderUniformValueType& value) { add(key, value); }

    template <typename T>
    void add(const std::string& key, const T& value)
    {
        SP_ASSERT(m_container.find(key) == m_container.end());
        m_container.insert({key, ShaderUniformValueType{value}});
    }

    const_iterator begin() const { return m_container.begin(); }
    const_iterator end() const { return m_container.end(); }

private:
    std::map<std::string, ShaderUniformValueType> m_container;
};
}   // namespace SP