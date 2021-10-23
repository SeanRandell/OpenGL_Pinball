#include "StateTracker.h"

StateTracker::StateTracker(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    //TODO fix this so that it does not break non fullscreen
    windowWidth = screenWidth;
    windowHeight = windowHeight;

    particleAmountPerBall = 500;

    this->camera = new Camera(0, 2.0, 10.0, 0.0, 1.0, 0.0, -90.0f, 0.0f, screenWidth, screenHeight);
    //this->sponge = new Sponge();

    lightModel = new LightingModel();
    particleGenerator = new ParticleGenerator(particleAmountPerBall);

    //boundary = new Rectangle(0, 0, 10 , 10);
    //quadtree = new Quadtree(boundary, 4);

    cube = new Cube();
    cylinder = new Cylinder();
    skyBox = new SkyBox();
    debugObject = new Debug();
    quad = new Quad();

    bloomExposure = 0.9f;
    isBloomOn = true;
    canLaunchBall = false;
    moveLeftFlipper = false;
    moveRightFlipper = false;
    isDebugOn = false;

    launchCooldown = 1.0f;
    launchCountdown = 1.0f;
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
    this->sphereShader = new RTRShader(
        "../Assignment2/Src/Shaders/SphereVertShader.vert",
        "../Assignment2/Src/Shaders/SphereFragShader.frag"
    );
    this->lightingShader = new RTRShader(
        "../Assignment2/Src/Shaders/LightingVertexShader.vert",
        "../Assignment2/Src/Shaders/LightingFragShader.frag"
    );
    this->directionalLightShader = new RTRShader(
        "../Assignment2/Src/Shaders/DirectionalVertShader.vert",
        "../Assignment2/Src/Shaders/DirectionalFragShader.frag",
        "../Assignment2/Src/Shaders/DirectionalGeomShader.geom"
    );
    this->blurShader = new RTRShader(
        "../Assignment2/Src/Shaders/BlurVertShader.vert",
        "../Assignment2/Src/Shaders/BlurFragShader.frag"
    );
    this->bloomShader = new RTRShader(
        "../Assignment2/Src/Shaders/BloomFinalVertShader.vert",
        "../Assignment2/Src/Shaders/BloomFinalFragShader.frag"
    );
    this->simpleDepthShader = new RTRShader(
        "../Assignment2/Src/Shaders/DepthVertShader.vert",
        "../Assignment2/Src/Shaders/DepthFragShader.frag"
    );
    this->debugDepthQuadShader = new RTRShader(
        "../Assignment2/Src/Shaders/DebugQuadDepthVertShader.vert",
        "../Assignment2/Src/Shaders/DebugQuadDepthFragShader.frag"
    );
    this->particleShader = new RTRShader(
        "../Assignment2/Src/Shaders/ParticleVertShader.vert",
        "../Assignment2/Src/Shaders/ParticleFragShader.frag"
    );
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
    //delete boundary;
    //delete quadtree;


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

    cylinder->End();
    delete cylinder;

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

    //glm::vec3 vector1 = glm::vec3(0.0, -4.0, 0.0);
    //glm::vec3 vector2 = glm::vec3(0.0, 0.0, 0.0);
    //glm::vec3 vector3 = glm::vec3(12.5f, 0.5f, 12.5f);
    Block* floorBlock = new Block(glm::vec3(0.0, -4.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(12.5f, 0.5f, 12.5f));
    //Block* floorBlock = new Block(vector1, vector2, vector3);
    Block* block1 = new Block();
    block1->position = glm::vec3(0.0, 2.0, 0.0);
    //block1->scale = glm::vec3(2.0, 2.0, 2.0);
    //block1->scale = glm::vec3(1.0, 1.0, 1.0);
    Block* block2 = new Block();
    block2->position = glm::vec3(0.0, -2.0, 0.0);
    Block* block3 = new Block();
    block3->position = glm::vec3(-2.0, 0.0, 0.0);
    Block* block4 = new Block();
    block4->position = glm::vec3(2.0, 0.0, 0.0);
    Block* block5 = new Block();
    block5->position = glm::vec3(0.0, 2.0, 0.0);
    Block* block6 = new Block();
    block6->position = glm::vec3(2.0, 2.0, 0.0);
    Block* block7 = new Block();
    block7->position = glm::vec3(-2.0, 2.0, 0.0);
    Block* block8 = new Block();
    block8->position = glm::vec3(2.0, -2.0, 0.0);
    Block* block9 = new Block();
    block9->position = glm::vec3(-2.0, -2.0, 0.0);
    //Block* block6 = new Block();
    //block6->position = glm::vec3(0.0, 3.0, 0.0);
    //block4->scale = glm::vec3(1.0, 0.0, 1.0);
    //blocks.push_back(floorBlock);
    blocks.push_back(block1);
    blocks.push_back(block2);
    blocks.push_back(block3);
    blocks.push_back(block4);
    blocks.push_back(block5);
    blocks.push_back(block6);
    blocks.push_back(block7);
    blocks.push_back(block8);
    blocks.push_back(block9);


    for (size_t i = 0; i < blocks.size(); i++)
    {
        blocks[i]->Init();
    }

    //Sphere* ball1 = new Sphere(-1);
    //ball1->position = glm::vec3(0.5f, 5.0f, 0.0f);
    //ball1->velocity = glm::vec2(0.5, -1.0);
    //Sphere* ball2 = new Sphere(-2);
    //ball2->position = glm::vec3(3.0f, 5.0f, 0.0f);
    //ball2->velocity = glm::vec2(-2, 1.0);

    Sphere* ball2 = new Sphere(-3);
    ball2->position = glm::vec3(0.0, 0.0, 0.0);
    Sphere* ball3 = new Sphere(-4);
    ball3->position = glm::vec3(3.0, 0.0, 0.0);
    Sphere* ball4 = new Sphere(-5);
    ball4->position = glm::vec3(2.0, 1.0, 0.0);
    Sphere* ball5 = new Sphere(-6);
    ball5->position = glm::vec3(1.0, 2.0, 0.0);
    Sphere* ball6 = new Sphere(-7);
    ball6->position = glm::vec3(0.0, 3.0, 0.0);

    //spheres.push_back(ball2);
    //spheres.push_back(ball3);
    //spheres.push_back(ball4);
    //spheres.push_back(ball5);
    //spheres.push_back(ball6);

    for (int i = 0; i < spheres.size(); i++)
    {
        spheres[i]->Init();
    }

    cylinder->Init();
    debugObject->Init();
    quad->Init(); 
}

void StateTracker::LaunchBall()
{
    int newSphereId = spheres.size();
    Sphere* newball = new Sphere(newSphereId);
    newball->Init();
    newball->velocity = glm::vec2(8.0, 4.0);
    //newball->scale = glm::vec3(2.0f, 2.0f, 2.0f);
    spheres.push_back(newball);
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


    // configure depth map FBO
// -----------------------
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


