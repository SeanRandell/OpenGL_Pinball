#pragma once
#include "RenderLoop.h"
#include "Console.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

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
    bool isFullScreen;
    int windowWidth;
    int windowHeight;
    int screenWidth;
    int screenHeight;
    std::string mainWindowTitle; 

    unsigned int currentTime{ 0 };
    float timeDelta{ 0 };
    int fps{ 0 };

    Console* console{ nullptr };
    SDL_Window* SDLWindow;
    SDL_Renderer* SDLRenderer;
    SDL_GLContext GLContext;

    void CheckInput();
    void UpdateState(Quadtree* quadtree);
    void RenderFrame(Quadtree* quadtree);
};