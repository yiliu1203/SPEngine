#include "GLCheck.h"
#include <iostream>
#include <string>

NS_BEGIN
static unsigned int g_last_error = 0;

void GLClearError()
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cout << "GLGetError:" << error;
    };
}


unsigned int GLCheckError()
{
    auto error = glGetError();
    if (error != GL_NO_ERROR) {
        g_last_error = error;
        return error;
    }
    g_last_error = 0;
    return 0;
}

unsigned int GLLastError()
{
    return g_last_error;
}
NS_END