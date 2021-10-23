//-----------------------------------------------------------------------------
// RTRObject.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "Object.h"

// README / TODO - This is just a simple start.
// * For your assignment, think carefully about how Shaders, Materials and Objects will hang together.
// * Have a hierarchy of differnet object types (static object, moving object, hierarchical object, object with force, etc...
// * Don't worry too much about optimising the buffer handling by, for example, merging all objects into a single 
// vertex buffer. Or sorting object based on shader or material to minimize switches. Spend your time on more imprtant 
// optimisation techniques that will add marks - for example your uniform grid implementation. 
// * For this assignment you can hardcode the vertex buffers for the differnet shapes (see cube) and then 
// scale, place and animate them with transformation matrices.

Object::Object() {
    numberOfVertices = 0;
    numberOfFaces = 0;
    vertexPoints = nullptr;
    faces = nullptr;
    material = { {0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 128.0 };
    vertexBuffer = 0;
    vertexArray = 0;
    faceElementBuffer = 0;
    //std::cout << "Base called" << std::endl;

    position = OBJECT_DEFAULT_POSITION;
    rotation = OBJECT_DEFAULT_ROTATION;
    scale = OBJECT_DEFAULT_SCALE;
    size = OBJECT_DEFAULT_SIZE;
}

void Object::Init()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(VertexPoint), vertexPoints, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &faceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfFaces * sizeof(ObjectFace), faces, GL_STATIC_DRAW);
}

void Object::Render(RTRShader* shader)
{
    shader->SetMaterial("objectMaterialUniform", material);
    glBindVertexArray(vertexArray);
    glDrawElements(GL_TRIANGLES, numberOfFaces * 3, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
    glBindVertexArray(0);
}

void Object::End()
{
    glDeleteVertexArrays(1, &vertexArray); vertexArray = 0;
    glDeleteBuffers(1, &vertexBuffer); vertexBuffer = 0;
    glDeleteBuffers(1, &faceElementBuffer); faceElementBuffer = 0;

    if (vertexPoints != nullptr)
    {
        delete vertexPoints;
        vertexPoints = nullptr;
    }
    if (faces != nullptr)
    {
        delete faces;
        faces = nullptr;
    }
}

void Object::SetMaterial(ObjectMaterial material) {
    this->material = material;
}

const char* Object::GetName()
{
    return "Object";;
}
