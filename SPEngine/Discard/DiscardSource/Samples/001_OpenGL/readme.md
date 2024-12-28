## xmake 使用

### 生成解决方案

    [参考](https://xmake.io/mirror/zh-cn/plugin/builtin_plugins.html)


- 生成vs解决方案
  
    ``` xmake project -k vsxmake -m "debug,release"```

- 生成CMakelist.txt 
  
    ```$ xmake project -k cmakelists ``

- 生成makefile 
  
    ```$ xmake project -k makefile``

- 生成CompileCommands, 避免vscode 中include 目录的问题报错，会根据compile_commands 找到 include 目录

``` xmake project -k compile_commands ```


- 默认安装的静态库， 如果要启用动态库， 可以配置如下：

``` add_requires("zlib", {configs = {shared = true}}) ```
当然，前提是这个包的定义里面，有对 package:config("shared") 判断处理，官方 xmake-repo 仓库里面，通常都是严格区分支持的。
    [参考](https://xmake.io/mirror/zh-cn/manual/global_interfaces.html)

## 依赖库

### glew

    使用静态链接, 要 加宏 GLEW_STATIC

### glfw

    使用静态链接


## opengl  01 第一个简单的简单的例子


```
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>



const std::string root = "D:/myWorkVirtualZ/SPingEngine/HazelEngineFrom0/HazelEngine/src";
void              framebuffer_size_callback(GLFWwindow* window, int width, int height);
void              processInput(GLFWwindow* window);

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string fragmentSource;
};

static void GLClearError()
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cout << "GLGetError:" << error;
    };
}

static void GLCheckError()
{
    while (auto error = glGetError()) {
        std::cout << "[GlGetError:] " << error << std::endl;
    }
}

#define GLCHECK(X)  \
    GLClearError(); \
    X;              \
    GLCheckError();


static unsigned int CompileShader(unsigned int type, const std::string& shaderString)
{
    unsigned int shaderID  = glCreateShader(type);
    const char*  shaderStr = shaderString.c_str();
    glShaderSource(shaderID, 1, &shaderStr, NULL);
    glCompileShader(shaderID);

    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(sizeof(char) * length);
        glGetShaderInfoLog(shaderID, length, &length, message);
        std::cout << "Shader Failed To Compile " << (type == GL_VERTEX_SHADER ? " vertexshader" : "fragmentshader") << std::endl
                  << message << std::endl;
    }
    return shaderID;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int programID = glCreateProgram();

    // vertexshader
    unsigned int vertextShaderID = CompileShader(GL_VERTEX_SHADER, vertexShader);

    // pixelshader
    unsigned int fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attachprogram link validate program
    glAttachShader(programID, vertextShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    // glValidateProgram(programID);

    int  linkSuccess = 0;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cout << "[Program Link Error]" << std::endl << infoLog << std::endl;
    }

    glDeleteShader(vertextShaderID);
    glDeleteShader(fragmentShaderID);
    return programID;
}


const ShaderProgramSource ParseShader(const std::string& shaderFilePath)
{
    std::ifstream     fstream(root + shaderFilePath);
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

// settings
const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);   // !!! core_profile 不会自动创建VAO
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 0;
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    constexpr size_t vertex_count          = 4;
    constexpr size_t vertex_size           = vertex_count * 3;
    float            vertices[vertex_size] = {
        0.5f,
        0.5f,
        0.0f,   // top right
        0.5f,
        -0.5f,
        0.0f,   // bottom right
        -0.5f,
        -0.5f,
        0.0f,   // bottom left
        -0.5f,
        0.5f,
        0.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0,
        1,
        3,   // first Triangle
        1,
        2,
        3   // second Triangle
    };

    // // 索引如何跟顶点数组关联起来的？ 原理还是还是状态机，enalbed indexbuffer and vertexbuffer

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // !!bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // !!! 记住：当 VAO 处于活动状态时，不要解除 EBO 的绑定，因为绑定的元素缓冲区对象存储在 VAO 中；保持 EBO 的绑定。
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);   // 之类可以unbind, 因为总是要在draw 前bind, 并且也可以防止 vao被修改

    auto         shaderSource = ParseShader("/res/shaders/basic.shader");
    unsigned int programID    = CreateShader(shaderSource.VertexSource, shaderSource.fragmentSource);

    glUseProgram(programID);

    GLCHECK(int uniformIndexColor = glGetUniformLocation(programID, "u_Color"));
    GLCHECK(glUniform4f(uniformIndexColor, 0.1f, 0.2f, 0.3f, 1.0f));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        GLClearError();

        glBindVertexArray(VAO);
        GLCHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glDeleteProgram(programID);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



```

其中 basic.shader:

```

#shader vertex
#version 410 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}


#shader fragment
#version 410 core

layout(location=0) out vec4 FragColor;
uniform vec4 u_Color
void main()
{
    FragColor = u_Color;
}

```

特别注意 VAO 的使用， EBO 不能在VAO 前 unbind

### VAO

    顶点数组对象帮助我们记住了 buffer 、 vertexlayout， 程序只需要在使用的时候 bind Vao, 对应的buff 和 vertexlayout 不需要重新指定了。
    从 gluseprogram ->bindvertextbuff->VertexAttrib->bindindexbuffer, 使用顶点数组之后
    gluseprogram -> bind vao ->bindIndexbuffer.

    

## opengl 02