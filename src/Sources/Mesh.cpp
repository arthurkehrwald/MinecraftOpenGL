#include "Mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(const std::vector<float>& coords, VertexArrayFormat vertexArrayFormat, const std::vector<int>& indices) : coordsSize(coords.size() * sizeof(float)), indicesSize(indices.size() * sizeof(int))
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, coordsSize, coords.data(), GL_STATIC_DRAW);
    
    if (indices == std::vector<int>())
    {
        useIndexedDrawing = false;
        EBO = 0;
    }
    else
    {
        useIndexedDrawing = true;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_STATIC_DRAW);
    }

    switch (vertexArrayFormat)
    {
    case VertexArrayFormat::pos:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        break;
    case VertexArrayFormat::posNormal:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        break;
    case VertexArrayFormat::posUV:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        break;
    case VertexArrayFormat::posNormalUV:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        break;
    }

    glBindVertexArray(0);
}

void Mesh::activate()
{
    glBindVertexArray(VAO);
}

void Mesh::draw()
{
    if (useIndexedDrawing)
    {
        glDrawElements(GL_TRIANGLES, indicesSize / sizeof(int), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, coordsSize);
    }
}