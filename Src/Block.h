#pragma once
#include "Object.h"
#include "SkyBox.h"

struct VertexAndTexturePoint {
    float x, y, z, u, v;
};

class Block : public Object
{
public:
    bool isBumper;
    bool isScenery;
    bool isActiveFlipper;
    
    Block();
    Block(bool isBumper);
    Block(glm::vec3 postion, glm::vec3 rotation, glm::vec3 scale, bool isBumper);
    ~Block() {}

    void InitConstructorValues();
    virtual void Init();
    virtual void Render(ShaderObject* shader, SkyBox* skybox, TextureObject* diffuseMap, TextureObject* specularMap, TextureObject* reflectionMap);
    void RenderNormalBlock(ShaderObject* shader, SkyBox* skybox, TextureObject* diffuseMap);
    virtual void End();
    virtual const char* GetName();
    bool IsContainedWithin(Rectangle* boundary);

private:
    VertexAndTexturePoint* vertexAndTexturePoints;
};
