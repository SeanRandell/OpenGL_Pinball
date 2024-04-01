#include "StateTracker.h"

StateTracker::StateTracker(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    windowWidth = screenWidth;
    windowHeight = screenHeight;

    particleAmount = 1000;

    shaderSources = new ShaderSources();
    camera = new Camera(0.0, 2.0, 30.0, 0.0, 1.0, 0.0, -90.0f, 0.0f, screenWidth, screenHeight);
    lightModel = new LightingModel();
    particleGenerator = new ParticleGenerator(particleAmount);

    cube = new Cube();
    skyBox = new SkyBox();
    debugObject = new Debug();
    quad = new Quad();
    InstancedSphere = new Sphere(-10);

    leftFlipper = new Block(true);
    rightFlipper = new Block(true);

    bloomExposure = 1.0f;
    isBloomOn = true;
    isQuadTreeOn = true;
    canLaunchBall = false;
    moveLeftFlipper = false;
    moveRightFlipper = false;
    leftFlipperMoving = false;
    rightFlipperMoving = false;
    isDebugOn = false;
    isDebugHUDOn = false;
    localIsDebugHUDOnToggle = false;
    isPhysicsOn = true;
    isVertexNormalsDisplayOn = false;
    isLightingBoxesOn = false;
    isBoundingBoxesOn = true;
    isLaunchLightMovingUp = true;

    launchCooldown = 0.1f;
    launchCountdown = 0.1f;
}

void StateTracker::InitShadersAndTextures()
{
    this->cubeShader = new ShaderObject(
        shaderSources->CubeVertShader,
        shaderSources->CubeFragShader,
        shaderSources->CubeGeomShader
    );

    this->blockShader = new ShaderObject(
        shaderSources->BlockVertShader,
        shaderSources->BlockFragShader,
        shaderSources->BlockGeomShader
    );
    this->normalShader = new ShaderObject(
        shaderSources->NormalVertShader,
        shaderSources->NormalFragShader,
        shaderSources->NormalGeomShader
    );
    this->skyBoxShader = new ShaderObject(
        shaderSources->SkyBoxVertShader,
        shaderSources->SkyBoxFragShader
    );
    this->sphereShader = new ShaderObject(
        shaderSources->SphereVertShader,
        shaderSources->SphereFragShader
    );
    this->lightingShader = new ShaderObject(
        shaderSources->LightingVertexShader,
        shaderSources->LightingFragShader
    );
    this->directionalLightShader = new ShaderObject(
        shaderSources->DirectionalVertShader,
        shaderSources->DirectionalFragShader,
        shaderSources->DirectionalGeomShader
    );
    this->blurShader = new ShaderObject(
        shaderSources->BlurVertShader,
        shaderSources->BlurFragShader
    );
    this->bloomShader = new ShaderObject(
        shaderSources->BloomFinalVertShader,
        shaderSources->BloomFinalFragShader
    );
    this->simpleDepthShader = new ShaderObject(
        shaderSources->DepthVertShader,
        shaderSources->DepthFragShader
    );
    this->debugDepthQuadShader = new ShaderObject(
        shaderSources->DebugQuadDepthVertShader,
        shaderSources->DebugQuadDepthFragShader
    );
    this->particleShader = new ShaderObject(
        shaderSources->ParticleVertShader,
        shaderSources->ParticleFragShader
    );

    std::string diffuseMapPath = "Images/Textures/container2.png";
    std::string specularMapPath = "Images/Textures/container2_specular.png";
    std::string reflectionMapPath = "Images/Textures/container2_specular.png";
    std::string bumperBlockTexturePath = "Images/Textures/SpaceFloor.jpg";
    std::string wallBlockTexturePath = "Images/Textures/Concrete.jpg";
    std::string particleTexturePath = "Images/Textures/particle.png";

    // TODO - Rework constructors
    this->diffuseMapTexture = new TextureObject(&diffuseMapPath, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 0, 0, true);
    this->specularMapTexture = new TextureObject(&specularMapPath, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 0, 0, true);
    this->reflectionMaptexture = new TextureObject(&reflectionMapPath, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 0, 0, true);
    this->bumperBlockTexture = new TextureObject(&bumperBlockTexturePath, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 0, 0, true);
    this->wallBlockTexture = new TextureObject(&wallBlockTexturePath, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 0, 0, true);
    this->particleTexture = new TextureObject(&particleTexturePath, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, GL_RGBA, GL_RGBA, false);

    this->diffuseMapTexture->SetTextureID();
    this->specularMapTexture->SetTextureID();
    this->reflectionMaptexture->SetTextureID();
    this->bumperBlockTexture->SetTextureID();
    this->wallBlockTexture->SetTextureID();
    this->particleTexture->SetTextureID();
}

StateTracker::~StateTracker()
{
    delete camera;
    delete cubeShader;
    delete normalShader;
    delete skyBoxShader;
    delete lightingShader;
    delete blockShader;
    delete sphereShader;
    delete directionalLightShader;
    delete blurShader;
    delete bloomShader;
    delete simpleDepthShader;
    delete debugDepthQuadShader;
    delete particleShader;
    delete lightModel;

    delete diffuseMapTexture;
    delete specularMapTexture;
    delete reflectionMaptexture;
    delete bumperBlockTexture;
    delete wallBlockTexture;
    delete particleTexture;

    cube->End();
    delete cube;

    for (int i = 0; i < blocks.size(); i++)
    {
        blocks[i]->End();
        delete blocks[i];
    }

    for (int i = 0; i < spheres.size(); i++)
    {
        spheres[i]->End();
        delete spheres[i];
    }

    for (int i = 0; i < pegs.size(); i++)
    {
        pegs[i]->End();
        delete pegs[i];
    }

    InstancedSphere->End();
    delete InstancedSphere;

    skyBox->End();
    delete skyBox;

    debugObject->End();
    delete debugObject;

    glDeleteFramebuffers(1, &hdrFBO);
    glDeleteFramebuffers(1, &rboDepth);
    glDeleteFramebuffers(2, pingpongFBO);
    glDeleteFramebuffers(1, &depthMapFBO);
}

void StateTracker::BuildGameObjects()
{
    cube->Init();

    skyBox->Init();

    Block* floorBlock = new Block(glm::vec3(2.0, 0.0, -1.1), glm::vec3(0.0, 0.0, 0.0), glm::vec3(30.0f, 27.0f, 0.0f), true);

    Block* TopBlock = new Block();
    TopBlock->position = glm::vec3(2.1, 11.0, 0.0);
    TopBlock->scale = glm::vec3(30.5, 6.0f, 2.0);

    Block* leftWallBlock = new Block();
    leftWallBlock->position = glm::vec3(-10.1, -1.0, 0.0);
    leftWallBlock->scale = glm::vec3(6.0, 18.0f, 2.0);

    Block* rightLaunchWall = new Block();
    rightLaunchWall->position = glm::vec3(14.4, 0.0, 0.0);
    rightLaunchWall->scale = glm::vec3(6.0, 16.0f, 2.0);

    Block* leftLaunchWallBlock = new Block();
    leftLaunchWallBlock->position = glm::vec3(8.1, -4.1, 0.0);
    leftLaunchWallBlock->scale = glm::vec3(2.0, 12.0f, 2.0);

    //Test Block to keep balls from despawning
    //Block* TestBlock = new Block();
    //TestBlock->position = glm::vec3(0.0, -8.0, 0.0);
    //TestBlock->scale = glm::vec3(6.0, 3.0f, 2.0);

    blocks.push_back(floorBlock);
    blocks.push_back(TopBlock);
    blocks.push_back(leftWallBlock);
    blocks.push_back(rightLaunchWall);
    blocks.push_back(leftLaunchWallBlock);

    //blocks.push_back(TestBlock);

    leftFlipper->scale = glm::vec3(4.0, 3.0, 1.0);
    rightFlipper->scale = glm::vec3(4.0, 3.0, 1.0);
    leftFlipper->position = glm::vec3(-5.0, -8.0, 0.0);
    rightFlipper->position = glm::vec3(5.0, -8.0, 0.0);
    blocks.push_back(leftFlipper);
    blocks.push_back(rightFlipper);

    for (size_t i = 0; i < blocks.size(); i++)
    {
        blocks[i]->Init();
    }

    Cylinder* launchPeg = new Cylinder();
    Cylinder* TopPeg1 = new Cylinder();
    Cylinder* TopPeg2 = new Cylinder();
    Cylinder* TopPeg3 = new Cylinder();
    Cylinder* TopPeg4 = new Cylinder();
    Cylinder* MiddlePeg1 = new Cylinder();
    Cylinder* MiddlePeg2 = new Cylinder();
    Cylinder* MiddlePeg3 = new Cylinder();
    Cylinder* BottomPeg1 = new Cylinder();
    Cylinder* BottomPeg2 = new Cylinder();
    Cylinder* BottomPeg3 = new Cylinder();
    Cylinder* BottomPeg4 = new Cylinder();

    launchPeg->position = glm::vec3(11.0, 6.0, -0.1);

    TopPeg1->position = glm::vec3(-4.5, 3.0, 0.0);
    TopPeg2->position = glm::vec3(-1.5, 3.0, 0.0);
    TopPeg3->position = glm::vec3(1.5, 3.0, 0.0);
    TopPeg4->position = glm::vec3(4.5, 3.0, 0.0);

    MiddlePeg1->position = glm::vec3(-3.0, 0.0, 0.0);
    MiddlePeg2->position = glm::vec3(0.0, 0.0, 0.0);
    MiddlePeg3->position = glm::vec3(3.0, 0.0, 0.0);

    BottomPeg1->position = glm::vec3(-4.5, -3.0, 0.0);
    BottomPeg2->position = glm::vec3(-1.5, -3.0, 0.0);
    BottomPeg3->position = glm::vec3(1.5, -3.0, 0.0);
    BottomPeg4->position = glm::vec3(4.5, -3.0, 0.0);

    //pushed to vector in order of lights array
    pegs.push_back(launchPeg);
    // red lights
    pegs.push_back(TopPeg1);
    pegs.push_back(MiddlePeg2);
    pegs.push_back(BottomPeg4);

    //green lights
    pegs.push_back(TopPeg4);
    pegs.push_back(BottomPeg1);
    pegs.push_back(BottomPeg3);

    // blue lights
    pegs.push_back(TopPeg2);
    pegs.push_back(MiddlePeg3);
    pegs.push_back(BottomPeg2);

    //purple lights
    pegs.push_back(TopPeg3);
    pegs.push_back(MiddlePeg1);

    for (int i = 0; i < pegs.size(); i++)
    {
        pegs[i]->Init();
    }

    debugObject->Init();
    quad->Init();
    InstancedSphere->Init();
}

void StateTracker::LaunchBall()
{
    // TODO check conversion
    int newSphereId = (int)spheres.size();
    Sphere* newball = new Sphere(newSphereId);
    newball->Init();
    newball->velocity = glm::vec2(0.0, 50.0);
    newball->position = glm::vec3(10.5f, -8.0f, 0.0f);
    spheres.push_back(newball);
}

void StateTracker::InitBuffers(int screenWidth, int screenHeight)
{
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    // create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
    //unsigned int colorBuffers[2];
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }
    // create and attach depth buffer (renderbuffer)
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ping-pong-framebuffer for blurring
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        // also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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