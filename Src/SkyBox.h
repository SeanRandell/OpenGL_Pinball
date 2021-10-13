#pragma once
#include "Object.h"
//#include <stb/stb_image.h>
#include <vector>
//-----------------------------------------------------------------------------
class SkyBox : public Object
{
public:
    std::vector<std::string> skyBoxFaces;
    unsigned int cubemapTexture;

    SkyBox() : Object() {};
    ~SkyBox() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual void End();
    virtual const char* GetName();
    unsigned int LoadCubemap();
};
