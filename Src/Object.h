//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "Lighting.h"
#include "RTRShader.h"
#include <stb/stb_image.h>

struct VertexPoint {
    float x, y, z;
};

struct ObjectFace {
    unsigned int v0, v1, v2;
};

class Object
{
public:
    Object();
    ~Object() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual void End();
    void SetMaterial(ObjectMaterial material);
    virtual const char* GetName();

    unsigned int numberOfVertices;
    unsigned int numberOfFaces;
    VertexPoint* vertexPoints;
    ObjectFace* faces;
    ObjectMaterial material;
    unsigned int vertexBuffer;
    unsigned int vertexArray;
    unsigned int faceElementBuffer;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

};