#pragma once
#include "Object.h"

class Cube : public Object
{
public:
    Cube() : Object() {};
    ~Cube() {}
    virtual void Init();
    virtual const char* GetName();
};
