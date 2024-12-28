#include "OpenGLShader.h"
#include "OpenGLHelpers.h"
#include "glad/glad.h"


namespace SP::RHI {


OpenGLShader::OpenGLShader(const std::string& name, const AssertURI& filepath)
    : Shader{name, filepath}
    , m_renderid{0}
    , m_compilelinked{false}
{}

void OpenGLShader::SetShaderParam(const std::string& name, ShaderUniformValueType value)
{
    if (const int* pval = std::get_if<int>(&value)) {
        SetInt(name, *pval);
    }
    else if (const float* pval = std::get_if<float>(&value)) {
        SetFloat(name, *pval);
    }
    else if (const vec2* pval = std::get_if<vec2>(&value)) {
        SetFloat2(name, *pval);
    }
    else if (const vec3* pval = std::get_if<vec3>(&value)) {
        SetFloat3(name, *pval);
    }
    else if (const vec4* pval = std::get_if<vec4>(&value)) {
        SetFloat4(name, *pval);
    }
    else if (const mat4* pval = std::get_if<mat4>(&value)) {
        SetMat4(name, *pval);
    }
    else {
        SP_ASSERT("Shader Param ValueType Error!! {}", name);
    }
}

void OpenGLShader::SetInt(const std::string& name, int value)
{
    GLint location = glGetUniformLocation(m_renderid, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) {}

void OpenGLShader::SetFloat(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(m_renderid, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::SetFloat2(const std::string& name, const vec2& value)
{
    GLint location = glGetUniformLocation(m_renderid, name.c_str());
    glUniform2fv(location, 1, value.v);
}

void OpenGLShader::SetFloat3(const std::string& name, const vec3& value)
{
    GLint location = glGetUniformLocation(m_renderid, name.c_str());
    glUniform3fv(location, 1, value.v);
}

void OpenGLShader::SetFloat4(const std::string& name, const vec4& value)
{
    GLint location = glGetUniformLocation(m_renderid, name.c_str());
    glUniform4fv(location, 4, value.v);
}

void OpenGLShader::SetMat4(const std::string& name, const mat4& value)
{
    GLint location  = glGetUniformLocation(m_renderid, name.c_str());
    GLint location2 = glGetUniformLocation(m_renderid, "u_emission2");
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

OpenGLShader::~OpenGLShader()
{
    if (m_renderid > 0) {
        glDeleteProgram(m_renderid);
        m_renderid = 0;
    }
}


void OpenGLShader::Bind()
{
    if (!m_compilelinked) {
        CompileAndLink();
    }
    glUseProgram(m_renderid);
}
void OpenGLShader::UnBind()
{
    SP_ASSERT(m_compilelinked && m_renderid > 0, "may be an error!");
    glUseProgram(0);
}

bool OpenGLShader::CompileAndLink()
{
    SP_ASSERT(m_shader_source.find(E_ShaderType::VertexShader) != m_shader_source.end(), "not find vertex shader!!");
    SP_ASSERT(m_shader_source.find(E_ShaderType::FragmentShader) != m_shader_source.end(), "not find fragment shader!!");

    std::array<GLuint, E_ShaderType::ShaderTypeEnd> shaderids;
    std::array<GLuint, E_ShaderType::ShaderTypeEnd> compiled_results;

    for (auto& item : m_shader_source) {
        GLenum        gl_shadertype = g_shader_type_config[item.first].GL_ShaderType;
        GLuint        shaderid      = glCreateShader(gl_shadertype);
        const GLchar* source        = (const GLchar*)item.second.c_str();
        glShaderSource(shaderid, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(shaderid);

        GLint isCompiled = 0;
        glGetShaderiv(shaderid, GL_COMPILE_STATUS, &isCompiled);
        compiled_results[item.first] = isCompiled;
        shaderids[item.first]        = shaderid;

        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shaderid, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(shaderid);

            SP_ASSERT(false, "compile compiled error!");
            return false;
        }
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.

    m_renderid     = glCreateProgram();
    GLuint program = m_renderid;
    // Attach our shaders to our program
    for (auto& item : m_shader_source) {
        glAttachShader(program, shaderids[item.first]);
    }

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        // glDeleteShader(vertexShader);
        // glDeleteShader(fragmentShader);

        for (auto& item : m_shader_source) {
            glDeleteShader(shaderids[item.first]);
        }

        // Use the infoLog as you see fit.

        // In this simple program, we'll just leave
        SP_ASSERT(false, "Link program error!");

        return false;
    }
    // Always detach shaders after a successful link.
    for (auto& item : m_shader_source) {
        glDetachShader(program, shaderids[item.first]);
    }

    return true;
}

#if 0
bool OpenGLShader::CompileAndLink_Old()
{
    // Create an empty vertex shader handle

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = (const GLchar*)m_vertex_src.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        SP_ASSERT(false, "compile vertexShader error!");
        return false;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar*)m_frag_src.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.

        // In this simple program, we'll just leave
        SP_ASSERT(false, "compile fragmentShader error!");
        return false;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_renderid     = glCreateProgram();
    GLuint program = m_renderid;
    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Use the infoLog as you see fit.

        // In this simple program, we'll just leave
        SP_ASSERT(false, "Link program error!");

        return false;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    return true;
}
#endif

}   // namespace SP::RHI
