#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <glad/glad.h>
#include "Core/GLCheck.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


using namespace SP::GL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
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
    constexpr size_t vertex_size           = vertex_count * 5;
    float            vertices[vertex_size] = {
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f,   // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left,
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f    // top left

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
    {

        // // 索引如何跟顶点数组关联起来的？ 原理还是还是状态机，enalbed indexbuffer and vertexbuffer
        VertexArray  va;
        VertexBuffer vb(vertices, sizeof(vertices));

        IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));

        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        VertexBufferLayout vbl;
        vbl.Push<float>(3);
        vbl.Push<float>(2);
        va.AddBuffer(vb, vbl);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // !!! 记住：当 VAO 处于活动状态时，不要解除 EBO 的绑定，因为绑定的元素缓冲区对象存储在 VAO 中；保持 EBO 的绑定。
        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);   // 之类可以unbind, 因为总是要在draw 前bind, 并且也可以防止 vao被修改
        Texture texture("/res/textures/grass.png");

        Shader shader("/res/shaders/basic.shader");
        shader.Bind();
        Renderer renderer;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);


        // render loop
        // -----------
        float color     = 0.0f;
        float timeStamp = 0.0;
        while (!glfwWindowShouldClose(window)) {
            // input
            // -----
            timeStamp += 0.01;
            processInput(window);
            renderer.Clear();
            color = timeStamp - int(timeStamp);
            // GLCHECK(glUniform4f(uniformIndexColor, color, color, color, 1.0f));
            texture.Bind();
            shader.SetUniform4f("u_Color", color, color, color, 1.0f);
            shader.SetUniformi("u_Texture", 0);

            renderer.Draw(va, ib, shader);
            // GLCHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        // GLCHECK(glDeleteProgram(programID));
    }
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