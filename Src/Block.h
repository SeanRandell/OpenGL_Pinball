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
    bool isBumper;
    bool isScenery;
    
    Block();
    Block(bool isBumper);
    Block(glm::vec3 postion, glm::vec3 rotation, glm::vec3 scale, bool isBumper);
    //Block(float positionX, float positionY, float positionZ, 
    //    float rotationX, float rotationY, float rotationZ,

    //);

    ~Block() {}
    void InitConstructorValues();
    virtual void Init();
    //virtual void Render(RTRShader* shader, SkyBox* skybox);
    //virtual void Render(RTRShader* shader, SkyBox* skybox, glm::mat4 modelMatrix, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, int instanceCount);
    virtual void Render(RTRShader* shader, SkyBox* skybox, glm::mat4 modelMatrix);
    virtual void End();
    //unsigned int LoadTexture(char const* path);
    unsigned int LoadTexture(std::string* path);
    virtual const char* GetName();
    bool IsContainedWithin(Rectangle* boundary);
};
