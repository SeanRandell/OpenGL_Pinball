//-----------------------------------------------------------------------------
// RTRLighting.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <glm/glm.hpp>

constexpr auto MAX_LIGHTS = 13;

enum class LightType : int
{
    DirectionalLight = 0,
    PointLight,
    SpotLight
};

// : Support for spot lights
struct Light
{
    LightType type;      // Used by light type ...
    glm::vec3 ambient;      //   Directional, Point, Spot
    glm::vec3 diffuse;      //   Directional, Point, Spot
    glm::vec3 specular;     //   Directional, Point, Spot
    glm::vec3 direction;    //   Directional, Spot
    glm::vec3 position;     //   Point, Spot
    float constant;         //   Point, Spot
    float linear;           //   Point, Spot
    float quadratic;        //   Point, Spot
    bool isLightOn;
    //float Cutoff;           //   Spot
    //float OuterCutoff;      //   Spot
};

struct ObjectMaterial
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct SpecularMaterial
{
    unsigned int diffuse;
    unsigned int specular;
    float shininess;
};

class LightingModel
{
public:
    LightingModel();
    ~LightingModel() {}
    int GetNumberOfLights();
    Light* GetLight(int index);
    int AddLight(Light light);
    Light* GetLights();

protected:
    int numberOfLights;
    Light lights[MAX_LIGHTS]{ };
};
