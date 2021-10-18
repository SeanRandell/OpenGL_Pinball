#include "StateTracker.h"

StateTracker::StateTracker(int screenWidth, int screenHeight)
{
    this->camera = new Camera(1.5, 2.0, 10.0, 0.0, 1.0, 0.0, -90.0f, 0.0f, screenWidth, screenHeight);
    //this->sponge = new Sponge();

    lightModel = new LightingModel();

    cube = new Cube();
    //sphere = new Sphere(1.0, 5, 5, true);
    //sphere = new Sphere();
    cylinder = new Cylinder();
    skyBox = new SkyBox();
    debugObject = new Debug();
    quad = new Quad();

    bloomExposure = 1.0f;
    isBloomOn = true;
    //this->maxPointLightCount = 7;
    //this->lightSourcesCount = 4;
    //this->deltaTime = 0;
    //this->isLightingOn = true;
    //this->isDepthTestingOn = true;
    //this->isBackFaceCullingOn = true;
    //this->isFullHUDOn = true;

    //BuildGameObjects();
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
    delete lightModel;

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

}

void StateTracker::BuildGameObjects()
{
    cube->Init();

    skyBox->Init();

    Block* floorBlock = new Block(glm::vec3(0.0, -4.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(12.5f, 0.5f, 12.5f));
    Block* block1 = new Block();

    blocks.push_back(floorBlock);
    blocks.push_back(block1);

    for (size_t i = 0; i < blocks.size(); i++)
    {
        blocks[i]->Init();
    }

    Sphere* ball1 = new Sphere();
    Sphere* ball2 = new Sphere();

    spheres.push_back(ball1);
    spheres.push_back(ball2);

    for (size_t i = 0; i < spheres.size(); i++)
    {
        spheres[i]->Init();
    }

    cylinder->Init();
    debugObject->Init();
    quad->Init();
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
}


