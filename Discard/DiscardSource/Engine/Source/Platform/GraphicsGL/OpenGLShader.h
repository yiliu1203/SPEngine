#pragma once
#include "PCH.h"
#include "Graphics/Shader.h"
#include "Misc/AssertURI.h"


namespace SP::RHI {
class OpenGLShader : public Shader
{
public:
    friend class OpenGLDevice;

    virtual ~OpenGLShader();

    virtual void SetShaderParam(const std::string& name, ShaderUniformValueType shader_param) override;
    virtual void SetInt(const std::string& name, int value) override;
    virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
    virtual void SetFloat(const std::string& name, float value) override;
    virtual void SetFloat2(const std::string& name, const vec2& value) override;
    virtual void SetFloat3(const std::string& name, const vec3& value) override;
    virtual void SetFloat4(const std::string& name, const vec4& value) override;
    virtual void SetMat4(const std::string& name, const mat4& value) override;

    virtual void Bind() override;
    virtual void UnBind() override;

public:
    OpenGLShader(const std::string& name, const AssertURI& uri);
    virtual bool CompileAndLink() override;

private:
    bool     m_compilelinked;
    uint32_t m_renderid;
};

}   // namespace SP::RHI
