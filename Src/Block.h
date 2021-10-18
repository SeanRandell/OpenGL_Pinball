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
    unsigned int diffuseMap;
    unsigned int specularMap;
    unsigned int reflectionMap;
    std::string diffusePath;
    std::string specularPath;

    Block(
        glm::vec3 postion = glm::vec3(0.0, 0.0, 0.0), 
        glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0),
        glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0),
        glm::vec3 length = glm::vec3(1.0, 1.0, 1.0),
        glm::vec2 rep = glm::vec2(1.0, 1.0)
    );
    ~Block() {}
    virtual void Init();
    //virtual void Render(RTRShader* shader, SkyBox* skybox);
    //virtual void Render(RTRShader* shader, SkyBox* skybox, glm::mat4 modelMatrix, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, int instanceCount);
    virtual void Render(RTRShader* shader, SkyBox* skybox, glm::mat4 modelMatrix);
    virtual void End();
    //unsigned int LoadTexture(char const* path);
    unsigned int LoadTexture(std::string* path);
    virtual const char* GetName();
};
