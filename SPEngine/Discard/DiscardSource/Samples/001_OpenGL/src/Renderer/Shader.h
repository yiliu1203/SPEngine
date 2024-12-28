#pragma once
#include "Core/APre.h"
#include <CORE/AssertRUI.h>
#include <GL/glew.h>


NS_BEGIN

// read from file
// compile
// set uniform
// use shader



class Shader
{


private:
    unsigned int                       m_programID;
    const AssertRUI                    m_FilePath;
    mutable std::map<std::string, int> m_UniformMap;

    struct ShaderProgramSource
    {
        std::string vertexSource;
        std::string fragmentSource;
    };
    const ShaderProgramSource ParseShader() const;
    unsigned int              CompileShader(unsigned int shaderType, const std::string& shaderString) const;

public:
    Shader(const std::string& filename);
    ~Shader();
    unsigned int CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) const;
    void         Bind() const;
    void         UnBind() const;
    unsigned int GetUniformLocation(const std::string& name) const;
    void         SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
    void         SetUniformi(const std::string& name, int v) const;
};


NS_END