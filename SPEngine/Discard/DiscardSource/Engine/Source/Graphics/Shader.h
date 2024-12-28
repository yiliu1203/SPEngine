#pragma once
#include "PCH.h"
#include "Graphics/UniformParam.h"
#include "Math/mathfwd.h"
#include "Misc/AssertURI.h"



namespace SP::RHI {
using namespace SP::math;
class SP_API Shader
{
    NoCopyable(Shader);

public:
    static std::shared_ptr<Shader> Create(const std::string& name, const AssertURI& filename);
    static std::shared_ptr<Shader> Create(const AssertURI& filename);

    virtual ~Shader()     = default;
    virtual void Bind()   = 0;
    virtual void UnBind() = 0;

    virtual void SetShaderParam(const std::string& name, ShaderUniformValueType shader_param) = 0;
    virtual void SetInt(const std::string& name, int value)                                   = 0;
    virtual void SetIntArray(const std::string& name, int* values, uint32_t count)            = 0;
    virtual void SetFloat(const std::string& name, float value)                               = 0;
    virtual void SetFloat2(const std::string& name, const vec2& value)                        = 0;
    virtual void SetFloat3(const std::string& name, const vec3& value)                        = 0;
    virtual void SetFloat4(const std::string& name, const vec4& value)                        = 0;
    virtual void SetMat4(const std::string& name, const mat4& value)                          = 0;

public:
    const std::string& GetName() const { return name; }

protected:
    virtual bool CompileAndLink() { return false; };
    Shader(const std::string& name, const AssertURI& filepath);

    std::string                         m_vertex_src;
    std::string                         m_frag_src;
    std::map<E_ShaderType, std::string> m_shader_source;
    std::string                         name;
    // uint32_t    m_renderid;
};


class SP_API ShaderLibary
{
public:
    static ShaderLibary& instance()
    {
        static ShaderLibary inst;
        return inst;
    }

    ShaderLibary() = default;
    void add(const Ref<Shader>& shader)
    {
        std::string name = shader->GetName();
        SP_ASSERT(m_shaders.find(name) == m_shaders.end(), "load name repeated");
        m_shaders[name] = shader;
    }

    Ref<Shader> load(const std::string& name, const std::string& uri)
    {
        auto shader = Shader::Create(name, uri);
        add(shader);
        return shader;
    }

    Ref<Shader> get(const std::string& name)
    {
        SP_ASSERT(m_shaders.find(name) != m_shaders.end(), "Not Find Shader, name: {0}", name);
        return m_shaders[name];
    }


private:
    std::map<std::string, Ref<Shader>> m_shaders;
};

}   // namespace SP::RHI
