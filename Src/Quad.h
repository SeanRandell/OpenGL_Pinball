#pragma once
#include "Object.h"
#include <vector>

class Quad : public Object
{
public:
    std::vector<float> vertices;

    ~Quad() {}
    virtual void Init();
    virtual void Render(ShaderObject* shader);
    void RenderParticle(ShaderObject* shader);
    virtual void End();
    virtual const char* GetName();
};
