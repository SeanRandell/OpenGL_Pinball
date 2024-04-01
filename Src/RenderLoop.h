#pragma once
#include <glad/glad.h>
#include <sdl/SDL.h>
#include "StateTracker.h"
#include "Lighting.h"
#include "Physics.h"
#include <algorithm>

// Class for taking away some of the clutter from the application class
class RenderLoop
{
public:
    //screen size
    int windowWidth, windowHeight;
    int screenWidth, screenHeight;

    Physics* physicsMethods;

    RenderLoop(int screenWidth, int screenHeight);
    ~RenderLoop();
    void Init(StateTracker* stateTracker, int screenWidth, int screenHeight);
    void CheckInput(StateTracker* stateTracker, bool* quitApp, float deltaTime);
    void UpdateState(StateTracker* stateTracker, float deltaTime, Quadtree* quadtree);
    void RenderFrame(StateTracker* stateTracker, Quadtree* quadtree);
private:

};
