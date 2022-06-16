#pragma once
#include <glad/glad.h>
#include <vector>

class Mesh
{
public:
    Mesh();
    static const enum class VertexArrayFormat { pos, posNormal, posUV, posNormalUV };
    Mesh(const std::vector<float>& coords, VertexArrayFormat vertexArrayFormat, const std::vector<int>& indices = std::vector<int>());
    void activate();
    void draw();

private:
    bool useIndexedDrawing;
    unsigned int VAO;
    unsigned int VBO;
    int coordsSize;
    unsigned int EBO;
    int indicesSize;
};