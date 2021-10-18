#include "Quad.h"

void Quad::Init()
{
    numberOfVertices = 4;
    vertices = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    //vertices = {
    //    // positions        // texture Coords
    //    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    //    - 1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    //     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    //     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    //};

    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() *sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Quad::Render(RTRShader* shader)
{
    //shader->SetMaterial("objectMaterialUniform", material);

    glBindVertexArray(vertexArray);
    //glDrawElements(GL_TRIANGLES, numberOfFaces * 3, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numberOfVertices);
    glBindVertexArray(0);
}

void Quad::End()
{
    Object::End();
}

const char* Quad::GetName() {
    return "Quad";
}