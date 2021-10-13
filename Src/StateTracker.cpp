#include "StateTracker.h"

StateTracker::StateTracker(int screenWidth, int screenHeight)
{
    this->camera = new Camera(1.5, 2.0, 10.0, 0.0, 1.0, 0.0, -90.0f, 0.0f, screenWidth, screenHeight);
    //this->sponge = new Sponge();

    lightModel = new LightingModel();

    cube = new Cube();
    block = new Block();
    skyBox = new SkyBox();

    //this->maxPointLightCount = 7;
    //this->lightSourcesCount = 4;
    //this->deltaTime = 0;
    //this->isLightingOn = true;
    //this->isDepthTestingOn = true;
    //this->isBackFaceCullingOn = true;
    //this->isFullHUDOn = true;
}

void StateTracker::Init()
{
    this->cubeShader = new RTRShader(
        "../Assignment2/Src/Shaders/CubeVertShader.vert",
        "../Assignment2/Src/Shaders/CubeFragShader.frag",
        "../Assignment2/Src/Shaders/CubeGeomShader.geom"
    );
    this->blockShader = new RTRShader(
        "../Assignment2/Src/Shaders/BlockVertShader.vert",
        "../Assignment2/Src/Shaders/BlockFragShader.frag",
        "../Assignment2/Src/Shaders/BlockGeomShader.geom"
    );
    this->normalShader = new RTRShader(
        "../Assignment2/Src/Shaders/NormalVertShader.vert",
        "../Assignment2/Src/Shaders/NormalFragShader.frag",
        "../Assignment2/Src/Shaders/NormalGeomShader.geom"
    );
    this->skyBoxShader = new RTRShader(
        "../Assignment2/Src/Shaders/SkyBoxVertShader.vert",
        "../Assignment2/Src/Shaders/SkyBoxFragShader.frag"
    );

    //this->lightingShader = new RTRShader(
    //    "../Assignment2/Src/Shaders/LightingVertexShader.vert",
    //    "../Assignment2/Src/Shaders/LightingFragShader.frag"
    //);
}

StateTracker::~StateTracker()
{
    delete camera;
    delete cubeShader;
    delete normalShader;
    delete skyBoxShader;
    //delete lightingShader;
    delete blockShader;
    delete lightModel;

    cube->End();
    delete cube;

    block->End();
    delete block;

    skyBox->End();
    delete skyBox;
}

std::string StateTracker::GetSettingString(bool boolToCheck)
{
    std::string returnString;
    if (boolToCheck) {
        returnString.append("On");
    }
    else {
        returnString.append("Off");
    }
    return returnString;
}
