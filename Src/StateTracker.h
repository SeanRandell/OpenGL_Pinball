#pragma once

//#include "Sponge.h"
#include "Camera.h"
#include "RTRShader.h"
#include "Cube.h"
#include "Block.h"
#include "SkyBox.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Debug.h"
#include "Quad.h"
#include <sdl/SDL_timer.h>
#include <string>

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

class StateTracker {
public:

    Camera* camera;
    
    glm::mat4 modelMatrix{ 1.0 };
    glm::mat4 projectionMatrix{ 1.0 };
    glm::mat4 viewMatrix{ 1.0 };

    Cube* cube;
    SkyBox* skyBox;
    std::vector<Block*> blocks;
    std::vector<Sphere*> spheres;

    Cylinder* cylinder;
    Debug* debugObject;
    Quad* quad;

    float bloomExposure;
    bool isBloomOn;



    //int lightSourcesCount;
    //int maxPointLightCount;


    //bool isLightingOn;
    //bool isDepthTestingOn;
    //bool isFullHUDOn;

    RTRShader* cubeShader{ nullptr };
    RTRShader* normalShader{ nullptr };
    RTRShader* skyBoxShader{ nullptr };
    RTRShader* lightingShader{ nullptr };
    RTRShader* blockShader{ nullptr };
    RTRShader* sphereShader{ nullptr };
    RTRShader* directionalLightShader{ nullptr };
    RTRShader* blurShader{ nullptr };
    RTRShader* bloomShader{ nullptr };
    RTRShader* simpleDepthShader{ nullptr };
    RTRShader* debugDepthQuadShader{ nullptr };

    LightingModel* lightModel;

    //bloom buffers
    unsigned int hdrFBO{0};
    unsigned int rboDepth{0};
    unsigned int pingpongFBO[2]{0,0};
    unsigned int pingpongColorbuffers[2]{0,0};
    unsigned int colorBuffers[2]{0,0};

    // directional shadows buffers
    unsigned int depthMapFBO{ 0 };
    unsigned int depthMap{0};

    //screen size
    int windowWidth, windowHeight;
    int screenWidth, screenHeight;


    StateTracker(int screenWidth, int screenHeight);
    ~StateTracker();

    void Init();

    std::string GetSettingString(bool boolToCheck);
    void InitBuffers(int screenWidth, int screenHeight);
    void BuildGameObjects();
    
private:

};