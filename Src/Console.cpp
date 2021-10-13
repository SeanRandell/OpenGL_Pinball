//-----------------------------------------------------------------------------
// Console.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include <string>
#include <glad/glad.h>
#define GLT_IMPLEMENTATION
#include <gltext/gltext.h>
#include "Console.h"

#define MAX_STRING 1024
GLTtext* onScreenDisplay = nullptr;

void Console::Init()
{
    stringToSet = new char[MAX_STRING+1];
    gltInit();
    onScreenDisplay = gltCreateText();
}

void Console::End()
{
    gltDeleteText(onScreenDisplay);
    gltTerminate();
    delete stringToSet;
}

void Console::Render(const char *title, int fps, float positionX, float positionY, float positionZ, float pitch, float yaw)
{
    gltBeginDraw();

    sprintf_s(stringToSet, MAX_STRING, title);
    gltSetText(onScreenDisplay, stringToSet);
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(onScreenDisplay, 10, 10, 2.0);

    sprintf_s(stringToSet, MAX_STRING,
        "\n"
        "FPS: %d\n"
        "Cam Pos: %.2f, %.2f, %.2f\n"
        "Cam Yaw: %.2f\n"
        "Cam Pitch: %.2f\n"
        , fps, positionX, positionY, positionZ, yaw, pitch);
    gltSetText(onScreenDisplay, stringToSet);
    gltColor(0.5f, 0.5f, 0.5f, 1.0f);
    gltDrawText2D(onScreenDisplay, 10, 10, 2.0);
    gltEndDraw();
    glUseProgram(0);
}
