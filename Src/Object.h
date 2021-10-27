//-----------------------------------------------------------------------------
// RTRObject.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "Lighting.h"
#include "RTRShader.h"
#include "TextureObject.h"
#include "Rectangle.h"

const glm::vec3 OBJECT_DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 OBJECT_DEFAULT_ROTATION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 OBJECT_DEFAULT_SCALE = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec2 OBJECT_DEFAULT_SIZE = glm::vec2(1.0f, 1.0f);

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
    virtual bool IsContainedWithin(Rectangle* boundary);

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
    glm::vec2 size;

};