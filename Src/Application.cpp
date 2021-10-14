//#define STB_IMAGE_IMPLEMENTATION
#include "Application.h"

Application::Application(const char* title, bool isFullscreen, int width, int height)
{
    stateTracker = new StateTracker(windowWidth, windowHeight);
    renderLoop = new RenderLoop();
    mainWindowTitle = title;
    this->isFullScreen = isFullscreen;
    windowWidth = width;
    windowHeight = height;
    screenWidth = 0;
    screenHeight = 0;
    SDLWindow = nullptr;
    SDLRenderer = nullptr;
    GLContext = nullptr;
    quitApp = false;
}

void Application::Run() {
    bool quit_app = false;
    while (quit_app == false) {

        static unsigned int fpsCounter = 0;
        static unsigned int fpsTime = 0;
        static unsigned int lastTime = 0;

        unsigned int localCurrentTime = SDL_GetTicks();
        unsigned int localTimeDeltaMilliseconds = localCurrentTime - lastTime;

        lastTime = localCurrentTime;
        fpsCounter++;
        fpsTime += localTimeDeltaMilliseconds;

        if (fpsCounter == 100) {
            fps = (int)((double)fpsCounter / (double)fpsTime * 1000.0f);
            fpsCounter = 0;
            fpsTime = 0;
        }

        currentTime = localCurrentTime;
        timeDelta = localTimeDeltaMilliseconds;

        //convert timeDelta to seconds
        float tempTimeDelta = timeDelta / 1000.0f;
        timeDelta = tempTimeDelta;

        quit_app = Tick();
    }
}

bool Application::Tick()
{
    quitApp = false;
    CheckInput();
    UpdateState();
    //update models and shaders here

    RenderFrame();
    return quitApp;
}

void Application::CheckInput()
{
    /*   renderLoop->CheckInput(stateTracker, &quitApp);
       if (stateTracker->camera->moveFoward) {
           std::cout << "move forward Second" << std::boolalpha << stateTracker->camera->moveFoward << std::endl;
       }*/

    SDL_Event keyEvent;
    if (SDL_PollEvent(&keyEvent))
    {
        std::cout << "key pressed" << std::endl;
        switch (keyEvent.type)
        {
        case SDL_QUIT:
            quitApp = true;
            break;
        case SDL_KEYDOWN:
            switch (keyEvent.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                quitApp = true;
                break;
            case SDLK_w:
                stateTracker->camera->moveFoward = true;
                std::cout << "move forward First" << std::boolalpha << stateTracker->camera->moveFoward << std::endl;
                break;
            case SDLK_s:
                stateTracker->camera->moveBackward = true;
                break;
            case SDLK_a:
                stateTracker->camera->moveLeft = true;
                break;
            case SDLK_d:
                stateTracker->camera->moveRight = true;
                break;
            case SDLK_LEFT:
                stateTracker->camera->turnLeft = true;
                break;
            case SDLK_RIGHT:
                stateTracker->camera->turnRight = true;
                break;
            case SDLK_UP:
                stateTracker->camera->tiltUp = true;
                break;
            case SDLK_DOWN:
                stateTracker->camera->tiltDown = true;
                break;
            }
            break;
        case SDL_KEYUP:
            switch (keyEvent.key.keysym.sym)
            {
            case SDLK_w:
                stateTracker->camera->moveFoward = false;
                break;
            case SDLK_s:
                stateTracker->camera->moveBackward = false;
                break;
            case SDLK_a:
                stateTracker->camera->moveLeft = false;
                break;
            case SDLK_d:
                stateTracker->camera->moveRight = false;
                break;
            case SDLK_LEFT:
                stateTracker->camera->turnLeft = false;
                break;
            case SDLK_RIGHT:
                stateTracker->camera->turnRight = false;
                break;
            case SDLK_UP:
                stateTracker->camera->tiltUp = false;
                break;
            case SDLK_DOWN:
                stateTracker->camera->tiltDown = false;
                break;
            }
            break;
        }
    }
}

void Application::UpdateState()
{
    //renderLoop->UpdateState(stateTracker, timeDelta);

    stateTracker->camera->ProcessCameraMoving(timeDelta);
    stateTracker->camera->ProcessCameraTurning(timeDelta);

    // Update directional camera to align with camera forward direction
    stateTracker->lightModel->GetLight(0)->direction = stateTracker->camera->front;

    stateTracker->modelMatrix = glm::mat4(1.0f);
    stateTracker->viewMatrix = stateTracker->camera->GetViewMatrix();
}

void Application::UpdateShaders()
{
    // //glGenVertexArrays(1, &stateTracker->VAO);
    // // 1. bind Vertex Array Object
    // glBindVertexArray(stateTracker->VAO);

    // glGenBuffers(1, &stateTracker->VBO);
    // //glGenBuffers(1, &stateTracker->EBO);
    // //glGenBuffers(1, &stateTracker->lightEBO);

    // // 2. copy our vertices array in a vertex buffer for OpenGL to use
    // glBindBuffer(GL_ARRAY_BUFFER, stateTracker->VBO);
    // //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ////glBindBuffer(GL_ARRAY_BUFFER, stateTracker->VBO);
    //// glBufferData(GL_ARRAY_BUFFER, stateTracker->vertices.size() * sizeof(float), &stateTracker->vertices.front(), GL_STATIC_DRAW);

    // //glBindBuffer(GL_ARRAY_BUFFER, stateTracker->VBO);
    // //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // // ebo stuff
    // //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stateTracker->EBO);
    // //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // // position attributes
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // // normal attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    //  //light cube 
    // glGenVertexArrays(1, &stateTracker->lightVAO);
    // glBindVertexArray(stateTracker->lightVAO);

    // glBindBuffer(GL_ARRAY_BUFFER, stateTracker->VBO);

    // // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
}
void Application::LoadTexture()
{
    //// load and create a texture 
    //    // -------------------------
    //unsigned int texture1;
    //// texture 1
    //// ---------
    //glGenTextures(1, &texture1);
    //glBindTexture(GL_TEXTURE_2D, texture1);

    //// set the texture wrapping parameters
    //// set texture wrapping to GL_REPEAT (default wrapping method)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //// set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //// load image, create texture and generate mipmaps
    //int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    //// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //unsigned char* data = stbi_load("container.jpg2", &width, &height, &nrChannels, 0);
    //if (data)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;

    //}
    //stbi_image_free(data);
}

void Application::RenderFrame()
{
    // Print out all debug info
    //renderLoop->RenderFrame(stateTracker);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //block
    stateTracker->blockShader->Use();
    stateTracker->blockShader->SetFloat("objectMaterialUniform.shininess", stateTracker->block->material.shininess);
    stateTracker->blockShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->blockShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->blockShader->SetCamera("cameraUniform", *stateTracker->camera);
    stateTracker->blockShader->SetLightingModel(*stateTracker->lightModel);

    stateTracker->blockShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    stateTracker->block->Render(stateTracker->blockShader);

    stateTracker->normalShader->Use();
    stateTracker->normalShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->normalShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    stateTracker->block->Render(stateTracker->normalShader);

    // cube
    stateTracker->cubeShader->Use();
    stateTracker->cubeShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->cubeShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->cubeShader->SetCamera("cameraUniform", *stateTracker->camera);
    stateTracker->cubeShader->SetLightingModel(*stateTracker->lightModel);

    stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, glm::vec3(3.0, 0.0, 0.0));
    stateTracker->cubeShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    stateTracker->cube->Render(stateTracker->cubeShader);

    stateTracker->normalShader->Use();
    stateTracker->normalShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->normalShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    stateTracker->cube->Render(stateTracker->normalShader);

    // sphere
    stateTracker->sphereShader->Use();
    stateTracker->sphereShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->sphereShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->sphereShader->SetCamera("cameraUniform", *stateTracker->camera);
    stateTracker->sphereShader->SetLightingModel(*stateTracker->lightModel);

    stateTracker->modelMatrix = glm::mat4(1.0f);
    stateTracker->modelMatrix = glm::translate(stateTracker->modelMatrix, glm::vec3(6.0, 0.0, 0.0));
    stateTracker->sphereShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    stateTracker->sphere->Render(stateTracker->sphereShader, stateTracker->skyBox->cubemapTexture);

    stateTracker->normalShader->Use();
    stateTracker->normalShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->normalShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->normalShader->SetMat4("modelMatrixUniform", stateTracker->modelMatrix);
    stateTracker->sphere->Render(stateTracker->normalShader, stateTracker->skyBox->cubemapTexture);

    // draw skybox as last
    glDepthFunc(GL_LESS); // set depth function back to default
    stateTracker->skyBoxShader->Use();
    // remove translation from the view matrix
    stateTracker->viewMatrix = glm::mat4(glm::mat3(stateTracker->camera->GetViewMatrix()));
    stateTracker->skyBoxShader->SetMat4("viewMatrixUniform", stateTracker->viewMatrix);
    stateTracker->skyBoxShader->SetMat4("projectionMatrixUniform", stateTracker->projectionMatrix);
    stateTracker->skyBox->Render(stateTracker->skyBoxShader);
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

    console->Render("DEBUG", fps,
        stateTracker->camera->position.x, stateTracker->camera->position.y, stateTracker->camera->position.z,
        stateTracker->camera->yaw, stateTracker->camera->pitch);

    SDL_GL_SwapWindow(SDLWindow);
}

int Application::Init()
{
    if (int err = InitTest() != 0) {
        return err;
    }

    stateTracker->projectionMatrix = glm::perspective(45.0f, ((GLfloat)screenWidth / (GLfloat)screenHeight), 0.1f, 100.0f);
    glViewport(0, 0, windowWidth, windowHeight);

    console = new Console();
    console->Init();
    stateTracker->Init();
    stateTracker->cube->Init();
    stateTracker->skyBox->Init();
    stateTracker->block->Init();
    stateTracker->sphere->Init();


    stateTracker->blockShader->Use();
    stateTracker->blockShader->SetInt("objectMaterialUniform.diffuse", 0);
    stateTracker->blockShader->SetInt("objectMaterialUniform.specular", 1);

    stateTracker->skyBoxShader->Use();
    stateTracker->skyBoxShader->SetInt("skyBoxUniform", 0);

    stateTracker->sphereShader->Use();
    stateTracker->sphereShader->SetInt("skyBoxUniform", 0);


    // Add directional light to follow camera view direction
    stateTracker->lightModel->AddLight({
        .type = LightType::DirectionalLight,
        .ambient = glm::vec3(0.2, 0.2, 0.2),
        .diffuse = glm::vec3(1.0, 1.0, 1.0),
        .specular = glm::vec3(0.5, 0.5, 0.5),
        .direction = glm::vec3(1.0, 0.0, 0.0)
        });
    // Add x-axis red spot lights (palce all at x+3.0 so they are positioned around shaded cube)
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(1.0, 0.0, 0.0),
        .diffuse = glm::vec3(1.0, 0.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(2.0 + 3.0, 0.0, 0.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(1.0, 0.0, 0.0),
        .diffuse = glm::vec3(1.0, 0.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(-2.0 + 3.0, 0.0, 0.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    //// Add y-axis green spot lights
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.2, 0.0),
        .diffuse = glm::vec3(0.0, 1.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(0.0 + 3.0, 2.0, 0.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.2, 0.0),
        .diffuse = glm::vec3(0.0, 1.0, 0.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(0.0 + 3.0, -2.0, 0.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    //// Add z-axis blue spot lights
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.0, 0.2),
        .diffuse = glm::vec3(0.0, 0.0, 1.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(0.0 + 3.0, 0.0, 2.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });
    stateTracker->lightModel->AddLight({
        .type = LightType::PointLight,
        .ambient = glm::vec3(0.0, 0.0, 0.2),
        .diffuse = glm::vec3(0.0, 0.0, 1.0),
        .specular = glm::vec3(1.0, 1.0, 1.0),
        .position = glm::vec3(0.0 + 3.0, 0.0, -2.0),
        .constant = 1.0f,
        .linear = 0.35f,
        .quadratic = 0.44f
        });

    return 0;
}

int Application::InitTest() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "RTR:ERROR: SDL2 video subsystem couldn't be initialized. Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    if (isFullScreen == true) {
        SDLWindow = SDL_CreateWindow(
            mainWindowTitle.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            0, 0,
            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    }
    else {
        SDLWindow = SDL_CreateWindow(
            mainWindowTitle.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth, windowHeight,
            SDL_WINDOW_OPENGL);
    }

    SDLRenderer = SDL_CreateRenderer(SDLWindow, -1, SDL_RENDERER_ACCELERATED);
    if (SDLRenderer == nullptr) {
        std::cerr << "RTR:ERROR: SDL2 Renderer couldn't be created. Error: " << SDL_GetError() << std::endl;
        return -2;
    }

    GLContext = SDL_GL_CreateContext(SDLWindow);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "RTR:ERROR: Failed to initialize the OpenGL context." << std::endl;
        return -3;
    }

    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
        screenWidth = dm.w;
        screenHeight = dm.h;
        if (isFullScreen == true) {
            windowWidth = screenWidth;
            windowHeight = screenHeight;
        }
    }
    else {
        std::cerr << "RTR:WARNING: SDL coudn't retrieve current display mode details." << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    std::cout << "RTR:MESSAGE: OpenGL version " << GLVersion.major << "." << GLVersion.minor << " initialised." << std::endl;

    return 0;
}

void Application::Done()
{

    //glDeleteVertexArrays(1, &stateTracker->VAO);
    //glDeleteVertexArrays(1, &stateTracker->lightVAO);
    //glDeleteBuffers(1, &stateTracker->VBO);
    ////glDeleteBuffers(1, &stateTracker->EBO);
    //glDeleteProgram(stateTracker->shaderProgram);

    console->End();
    delete console;

    delete stateTracker;

    SDL_GL_DeleteContext(GLContext);
    SDL_DestroyRenderer(SDLRenderer);
    SDL_DestroyWindow(SDLWindow);
    SDL_Quit();
    SDLWindow = nullptr;
    SDLRenderer = nullptr;
    GLContext = nullptr;
}