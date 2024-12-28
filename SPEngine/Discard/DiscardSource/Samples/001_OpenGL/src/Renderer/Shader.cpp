#include "Shader.h"
#include "Core/GLCheck.h"
#include <fstream>


NS_BEGIN

Shader::Shader(const std::string& filePath)
    : m_FilePath{filePath}
    , m_programID{0}
{
    auto result = ParseShader();
    m_programID = CreateShaderProgram(result.vertexSource, result.fragmentSource);
}

unsigned int Shader::CompileShader(unsigned int shaderType, const std::string& shaderString) const
{
    GLCHECK(unsigned int shaderID = glCreateShader(shaderType));
    const char* shaderStr = shaderString.c_str();
    GLCHECK(glShaderSource(shaderID, 1, &shaderStr, NULL));
    GLCHECK(glCompileShader(shaderID));

    int result;
    GLCHECK(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(sizeof(char) * length);
        glGetShaderInfoLog(shaderID, length, &length, message);
        std::cout << "Shader Failed To Compile " << (shaderType == GL_VERTEX_SHADER ? " vertexshader" : "fragmentshader")
                  << std::endl
                  << message << std::endl;
    }
    return shaderID;
}

const Shader::ShaderProgramSource Shader::ParseShader() const
{
    // std::cout << "m_FilePath -->" << m_FilePath.path() << std::endl;
    std::ifstream     fstream(m_FilePath.path());
    std::string       line;
    std::stringstream ss[2];

    size_t shaderTypeIndex = -1;
    while (std::getline(fstream, line)) {
        if (line.find("#shader") != line.npos) {
            if (line.find("#shader vertex") != line.npos) {
                shaderTypeIndex = 0;
            }
            else if (line.find("#shader fragment") != line.npos) {
                shaderTypeIndex = 1;
            }
        }
        else {
            if (shaderTypeIndex >= 0) {
                ss[shaderTypeIndex] << line << "\n";
            }
        }
    };
    return {ss[0].str(), ss[1].str()};
}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
    m_programID = 0;
}

unsigned int Shader::CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) const
{
    unsigned int vertexShaderID   = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    // attachprogram link validate program
    GLCHECK(unsigned int programID = glCreateProgram());
    GLCHECK(glAttachShader(programID, vertexShaderID));
    GLCHECK(glAttachShader(programID, fragmentShaderID));
    GLCHECK(glLinkProgram(programID));
    // glValidateProgram(programID);

    int  linkSuccess = 0;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cout << "[Program Link Error]" << std::endl << infoLog << std::endl;
    }

    GLCHECK(glDeleteShader(vertexShaderID));
    GLCHECK(glDeleteShader(fragmentShaderID));
    return programID;
}

void Shader::Bind() const
{
    GLCHECK(glUseProgram(m_programID));
}

void Shader::UnBind() const
{
    GLCHECK(glUseProgram(0));
}

unsigned int Shader::GetUniformLocation(const std::string& name) const
{
    ASSERT(m_programID);
    int         uniformIndex = -1;
    auto const& iter         = m_UniformMap.find(name);
    if (iter == m_UniformMap.end()) {
        uniformIndex = glGetUniformLocation(m_programID, name.c_str());
        m_UniformMap.insert({name, uniformIndex});
    }
    else {
        uniformIndex = iter->second;
    }
    return uniformIndex;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    GLCHECK(unsigned int uniformIndex = GetUniformLocation(name));
    // GLCHECK(glUniform4f(uniformIndexColor, 0.1f, 0.2f, 0.3f, 1.0f));
    GLCHECK(glUniform4f(uniformIndex, v0, v1, v2, v3));
}

void Shader::SetUniformi(const std::string& name, int v) const
{
    GLCHECK(unsigned int uniformIndex = GetUniformLocation(name));
    GLCHECK(glUniform1i(uniformIndex, v));
}

NS_END