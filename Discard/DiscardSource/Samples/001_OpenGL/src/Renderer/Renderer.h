#pragma once
#include "Core/APre.h"
#include "Core/GLCheck.h"
#include "GL/glew.h"


NS_BEGIN

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
    static void OnWindowResize(int x, int y);

private:
public:
    void Clear();
    void Draw(const VertexArray& vb, const IndexBuffer& ib, const Shader& shader);
};

NS_END