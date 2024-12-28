#include "Shader.h"
#include "Graphics/Device.h"
#include "Misc/Path.h"
#include "Platform/GraphicsGL/OpenGLContext.h"
#include "glad/glad.h"


namespace SP::RHI {
Shader::Shader(const std::string& name, const AssertURI& filepath)
    : name{name}
{
    std::ifstream     fstream(filepath.get_abs_path(), std::ios_base::in);
    std::string       line;
    std::stringstream ss[E_ShaderType::ShaderTypeEnd];

    uint32 shaderTypeIndex = 0;
    while (std::getline(fstream, line)) {
        if (line.find("#type") != line.npos) {
            if (line.find("#type vertex") != line.npos) {
                shaderTypeIndex = E_ShaderType::VertexShader;
            }
            else if (line.find("#type fragment") != line.npos) {
                shaderTypeIndex = E_ShaderType::FragmentShader;
            }
        }
        else {
            if (shaderTypeIndex >= 0) {
                ss[shaderTypeIndex] << line << "\n";
            }
        }
    };
    for (uint32 i = 0; i < E_ShaderType::ShaderTypeEnd; i++) {
        E_ShaderType shadertype = static_cast<E_ShaderType>(i);
        m_shader_source.insert({shadertype, ss[i].str()});
    }
    // m_vertex_src = ss[0].str();
    // m_frag_src   = ss[1].str();
}

Ref<Shader> Shader::Create(const std::string& name, const AssertURI& filename)
{
    return Device::s_device->CreateShader(name, filename);
}

Ref<Shader> Shader::Create(const AssertURI& filename)
{
    return Device::s_device->CreateShader(filename.get_pure_name(), filename);
}

#if 0
Shader::Shader(const std::string& vertex_source, const std::string& frag_source)
{
    std::cout << "path" << Path::shader_dir << std::endl;
    m_vertex_src = vertex_source;
    m_frag_src   = frag_source;
    SP_ASSERT(CompileAndLink(), "Shader Error!!!");
}


Shader::~Shader()
{
    if (m_renderid > 0) {
        glDeleteProgram(m_renderid);
        // glDeleteShader  ...
        m_renderid = 0;
    }
}

void Shader::Bind()
{
    glUseProgram(m_renderid);
}
void Shader::UnBind()
{
    glUseProgram(0);
}
bool Shader::CompileAndLink()
{

    // Read our shaders into the appropriate buffers
    // std::string     vertexSource   =   // Get source code for vertex shader.
    //     std::string fragmentSource =   // Get source code for fragment shader.

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