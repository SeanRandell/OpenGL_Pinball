#pragma once
#include "Object.h"
#include <vector>

class SkyBox : public Object
{
public:
    std::vector<std::string> skyBoxFaces;
    unsigned int cubemapTexture{0};

    SkyBox() : Object() {};
    ~SkyBox() {}
    virtual void Init();
    virtual void Render(ShaderObject* shader);
    virtual void End();
    virtual const char* GetName();
    unsigned int LoadCubemap();
};
