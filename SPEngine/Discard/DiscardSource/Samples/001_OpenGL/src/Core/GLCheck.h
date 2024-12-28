#pragma once

#include "APre.h"
#include "Exception.h"
#include <GL/glew.h>
#include <functional>
#include <string>











NS_BEGIN

void         GLClearError();
unsigned int GLCheckError();
unsigned int GLLastError();

#define GLCHECK(X)  \
    GLClearError(); \
    X;              \
    ASSERT(GLCheckError() == 0, #X, " [---->with Error Code = " + std::to_string(GLLastError()) + " <-----]")

NS_END