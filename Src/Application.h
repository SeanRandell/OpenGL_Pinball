#pragma once
#include "RenderLoop.h"
#include "Console.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Application
{
public:
    Application(const char* title, bool fullscreen, int width, int height);
    int Init();
    int InitTest();
    void Run();
    void Done();
    bool Tick();

    StateTracker* stateTracker;
    RenderLoop* renderLoop;
private:

    bool quitApp;

    Console* console{ nullptr };

    void CheckInput();
    void UpdateState();
    void RenderFrame();
    //void UpdateShaders();
    //void LoadTexture();
    //void InitBuffers();

    std::string mainWindowTitle;
    bool isFullScreen;
    int windowWidth;
    int windowHeight;
    int screenWidth;
    int screenHeight;
    SDL_Window* SDLWindow;
    SDL_Renderer* SDLRenderer;
    SDL_GLContext GLContext;

    //unsigned int hdrFBO;
    //unsigned int rboDepth;
    //unsigned int pingpongFBO[2];
    //unsigned int pingpongColorbuffers[2];

    unsigned int currentTime{ 0 };
    float timeDelta{ 0 };
    int fps{ 0 };
};