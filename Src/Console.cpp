#include <string>
#include <glad/glad.h>
#define GLT_IMPLEMENTATION
#include <gltext/gltext.h>
#include "Console.h"

#define MAX_STRING 1024
GLTtext* onScreenDisplay = nullptr;

void Console::Init()
{
    stringToSet = new char[MAX_STRING + 1];
    gltInit();
    onScreenDisplay = gltCreateText();
}

void Console::End()
{
    gltDeleteText(onScreenDisplay);
    gltTerminate();
    delete stringToSet;
}

void Console::Render(const char* title, int fps,
    float positionX, float positionY, float positionZ, float pitch, float yaw, StateTracker* stateTracker)
{
    std::string ballReadyString = IsBallReady(stateTracker);

    gltBeginDraw();

    sprintf_s(stringToSet, MAX_STRING, title);
    gltSetText(onScreenDisplay, stringToSet);
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(onScreenDisplay, 10, 10, 2.0);
    if ((stateTracker->isDebugHUDOn || stateTracker->isDebugOn) && stateTracker->localIsDebugHUDOnToggle)
    {
        sprintf_s(stringToSet, MAX_STRING,
            "\n"
            "FPS: %d\n"
            "Cam Pos: %.2f, %.2f, %.2f\n"
            "Cam Yaw: %.2f\n"
            "Cam Pitch: %.2f\n"
            "Quadtree: %s\n"
            "Physics Objects Count: %d\n"
            "Physics: %s\n"
            "Display Bounding Boxes: %s\n"
            "Display Lighting Boxes %s\n"
            "Display Vertex Normals: %s\n"
            "Ball Launcher: %s\n"
            , fps, positionX, positionY, positionZ, yaw, pitch,
            stateTracker->GetSettingString(stateTracker->isQuadTreeOn).c_str(),
            stateTracker->physicsObjectCount,
            stateTracker->GetSettingString(stateTracker->isPhysicsOn).c_str(),
            stateTracker->GetSettingString(stateTracker->isBoundingBoxesOn).c_str(),
            stateTracker->GetSettingString(stateTracker->isLightingBoxesOn).c_str(),
            stateTracker->GetSettingString(stateTracker->isVertexNormalsDisplayOn).c_str(),
            ballReadyString.c_str());
    }
    else
    {
        sprintf_s(stringToSet, MAX_STRING,
            "Press H for debug HUD\n"
            "Ball Launcher: %s\n"
            , ballReadyString.c_str());
    }

    gltSetText(onScreenDisplay, stringToSet);
    gltColor(0.5f, 0.5f, 0.5f, 1.0f);
    gltDrawText2D(onScreenDisplay, 10, 10, 2.0);
    gltEndDraw();
    glUseProgram(0);
}

std::string Console::IsBallReady(StateTracker* stateTracker)
{
    std::string returnString = "Off";
    if (stateTracker->launchCountdown >= stateTracker->launchCooldown)
    {
        returnString = "On";
    }
    return returnString;
}