#pragma once
#include <glad/glad.h>
#include <sdl/SDL.h>
#include "StateTracker.h"
#include "Lighting.h"

// Class for taking away some of the clutter from the application class
class RenderLoop
{
public:

    void Init(StateTracker* stateTracker, int screenWidth, int screenHeight);
    void CheckInput(StateTracker* stateTracker, bool *quitApp);
    void UpdateState(StateTracker* stateTracker, float deltaTime);
    //void RenderOnScreenDisplay();
    void RenderFrame(StateTracker* stateTracker);
private:

};
