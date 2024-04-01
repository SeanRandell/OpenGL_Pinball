#pragma once

//#include "Sponge.h"
#include "Camera.h"
#include "ShaderObject.h"
#include "Cube.h"
#include "Block.h"
#include "SkyBox.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Debug.h"
#include "Quad.h"
#include "ParticleGenerator.h"
#include "QuadTree.h"
#include <sdl/SDL_timer.h>
#include <string>
#include "ShaderSources.h"

//const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

class StateTracker {
public:
    //objects
    Camera* camera;
    Cube* cube;
    SkyBox* skyBox;
    Block* leftFlipper;
    Block* rightFlipper;
    Debug* debugObject;
    Quad* quad;
    Sphere* InstancedSphere;
    ShaderSources* shaderSources;

    //object collections
    std::vector<Block*> blocks;
    std::vector<Sphere*> spheres;
    std::vector<Cylinder*> pegs;

    // matrices
    glm::mat4 modelMatrix{ 1.0 };
    glm::mat4 projectionMatrix{ 1.0 };
    glm::mat4 viewMatrix{ 1.0 };


    // settings
    float bloomExposure;
    int particleAmount;
    unsigned int physicsObjectCount{ 0 };

    bool canLaunchBall;
    float launchCooldown;
    float launchCountdown;

    bool moveLeftFlipper;
    bool moveRightFlipper;
    bool leftFlipperMoving;
    bool rightFlipperMoving;

    bool isBloomOn;
    bool isDebugOn;
    bool isQuadTreeOn;
    bool isDebugHUDOn;
    bool localIsDebugHUDOnToggle;
    bool isPhysicsOn;
    bool isVertexNormalsDisplayOn;
    bool isLightingBoxesOn;
    bool isBoundingBoxesOn;
    bool isLaunchLightMovingUp;

    //shaders
    ShaderObject* cubeShader{ nullptr };
    ShaderObject* normalShader{ nullptr };
    ShaderObject* skyBoxShader{ nullptr };
    ShaderObject* lightingShader{ nullptr };
    ShaderObject* blockShader{ nullptr };
    ShaderObject* sphereShader{ nullptr };
    ShaderObject* directionalLightShader{ nullptr };
    ShaderObject* blurShader{ nullptr };
    ShaderObject* bloomShader{ nullptr };
    ShaderObject* simpleDepthShader{ nullptr };
    ShaderObject* debugDepthQuadShader{ nullptr };
    ShaderObject* particleShader{ nullptr };

    //Subroutine Indexes
    //Block subroutines
    GLuint OneTextureIndex{ 0 };
    GLuint ThreeTextureIndex{ 0 };

    //Textures
    // block Textures
    TextureObject* diffuseMapTexture{ nullptr };
    TextureObject* specularMapTexture{ nullptr };
    TextureObject* reflectionMaptexture{ nullptr };
    TextureObject* bumperBlockTexture{ nullptr };
    TextureObject* wallBlockTexture{ nullptr };
    //particle
    TextureObject* particleTexture{ nullptr };

    // rendering objects
    LightingModel* lightModel;
    ParticleGenerator* particleGenerator;

    //bloom buffers
    unsigned int hdrFBO{ 0 };
    unsigned int rboDepth{ 0 };
    unsigned int pingpongFBO[2]{ 0,0 };
    unsigned int pingpongColorbuffers[2]{ 0,0 };
    unsigned int colorBuffers[2]{ 0,0 };

    // directional shadows buffers
    unsigned int depthMapFBO{ 0 };
    unsigned int depthMap{ 0 };

    //screen size
    int windowWidth, windowHeight;
    int screenWidth, screenHeight;

    StateTracker(int screenWidth, int screenHeight);
    ~StateTracker();

    void InitShadersAndTextures();
    std::string GetSettingString(bool boolToCheck);
    void InitBuffers(int screenWidth, int screenHeight);
    void BuildGameObjects();
    void LaunchBall();

private:

};