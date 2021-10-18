#pragma once
#include "Object.h"

class Debug : public Object
{
public:
    ~Debug() {}
    virtual void Init();
    virtual void Render(RTRShader* shader);
    virtual void End();
    virtual const char* GetName();
};
