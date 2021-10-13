#pragma once

//#include "Sponge.h"
#include "Camera.h"
#include "RTRShader.h"
#include "Cube.h"
#include "Block.h"
#include "SkyBox.h"
#include <sdl/SDL_timer.h>
#include <string>

class StateTracker {
public:

    Camera* camera;
    
    glm::mat4 modelMatrix{ 1.0 };
    glm::mat4 projectionMatrix{ 1.0 };
    glm::mat4 viewMatrix{ 1.0 };

    Cube* cube;
    Block* block;
    SkyBox* skyBox;
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

    LightingModel* lightModel;

    StateTracker(int screenWidth, int screenHeight);
    ~StateTracker();

    void Init();

    std::string GetSettingString(bool boolToCheck);

    
private:

};