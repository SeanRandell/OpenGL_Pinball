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
    //float deltaTime;

    //int verticesCount;
    //int facesCount;

    //int countedFrames;
    //int refreshFramesCounter;
    //Uint32 timeTicks;
    //float averageFPS;

    //bool isLightingOn;
    //bool isDepthTestingOn;
    //bool isBackFaceCullingOn;
    //bool isFullHUDOn;
    //bool stateChanged;

    //unsigned int VBO;
    //unsigned int VAO;
    ////unsigned int EBO;

    //unsigned int lightVAO;
    ////unsigned int lightEBO;

    //unsigned int shaderProgram;
    //unsigned int fragmentShader;
    //unsigned int vertexShader;
    //
    RTRShader* cubeShader{ nullptr };
    RTRShader* normalShader{ nullptr };
    RTRShader* skyBoxShader{ nullptr };
    RTRShader* lightingShader{ nullptr };
    RTRShader* blockShader{ nullptr };
    RTRShader* sphereShader{ nullptr };
    RTRShader* directionalLightShader{ nullptr };
    RTRShader* blurShader{ nullptr };
    RTRShader* bloomShader{ nullptr };

    LightingModel* lightModel;

    unsigned int hdrFBO;
    unsigned int rboDepth;
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
    unsigned int colorBuffers[2];

    StateTracker(int screenWidth, int screenHeight);
    ~StateTracker();

    void Init();

    std::string GetSettingString(bool boolToCheck);
    void InitBuffers(int screenWidth, int screenHeight);
    void BuildGameObjects();
    
private:

};