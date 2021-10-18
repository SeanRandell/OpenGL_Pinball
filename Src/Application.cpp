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
    renderLoop->CheckInput(stateTracker, &quitApp);
}

void Application::UpdateState()
{
    renderLoop->UpdateState(stateTracker, timeDelta);
}

void Application::RenderFrame()
{
    // Print out all debug info
    renderLoop->RenderFrame(stateTracker);

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

    renderLoop->Init(stateTracker, screenWidth, screenHeight);

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
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

//void Application::InitBuffers()
//{
//    glGenFramebuffers(1, &hdrFBO);
//    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
//    // create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
//    unsigned int colorBuffers[2];
//    glGenTextures(2, colorBuffers);
//    for (unsigned int i = 0; i < 2; i++)
//    {
//        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        // attach texture to framebuffer
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
//    }
//    // create and attach depth buffer (renderbuffer)
//    unsigned int rboDepth;
//    glGenRenderbuffers(1, &rboDepth);
//    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
//    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
//    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
//    glDrawBuffers(2, attachments);
//    // finally check if framebuffer is complete
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//        std::cout << "Framebuffer not complete!" << std::endl;
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    // ping-pong-framebuffer for blurring
//    unsigned int pingpongFBO[2];
//    unsigned int pingpongColorbuffers[2];
//    glGenFramebuffers(2, pingpongFBO);
//    glGenTextures(2, pingpongColorbuffers);
//    for (unsigned int i = 0; i < 2; i++)
//    {
//        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
//        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
//        // also check if framebuffers are complete (no need for depth buffer)
//        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//            std::cout << "Framebuffer not complete!" << std::endl;
//    }
//}