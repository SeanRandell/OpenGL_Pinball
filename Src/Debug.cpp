#include "Debug.h"

void Debug::Init()
{
    numberOfVertices = 2;
    vertexPoints = new VertexPoint[]{
    { 0, 0, 0 }, // 0
    { 0, 0, 0 }, // 1
    };

    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(vertexPoints), vertexPoints, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Debug::Render(ShaderObject* shader)
{
    //shader->SetMaterial("objectMaterialUniform", material);

    glBindVertexArray(vertexArray);
    glDrawArrays(GL_POINTS, 0, numberOfVertices);
    glBindVertexArray(0);
}

void Debug::End()
{
    Object::End();
}

const char* Debug::GetName() {
    return "Debug";
}