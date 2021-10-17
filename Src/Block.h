#pragma once
#include "Object.h"
#include "SkyBox.h"
//-----------------------------------------------------------------------------

struct VertexAndTexturePoint {
    float x, y, z, u, v;
};

class Block : public Object
{
public:
    VertexAndTexturePoint* vertexAndTexturePoints;
    //unsigned int texture;
    unsigned int diffuseMap;
    unsigned int specularMap;
    unsigned int reflectionMap;
    std::string diffusePath;
    std::string specularPath;
    //std::string reflectionPath;

    Block(glm::vec3 length = glm::vec3(1, 1, 1), glm::vec2 rep = glm::vec2(1, 1));
    ~Block() {}
    virtual void Init();
    virtual void Render(RTRShader* shader, SkyBox* skybox);
    virtual void End();
    //unsigned int LoadTexture(char const* path);
    unsigned int LoadTexture(std::string* path);
    virtual const char* GetName();
};
