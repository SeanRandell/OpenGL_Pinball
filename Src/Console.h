//-----------------------------------------------------------------------------
// Console.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once

class Console
{
public: 
    Console() {}
    ~Console() {}
    void Init();
    void End();
    void Render(const char *title, int fps, float pos_x, float pos_y, float pos_z, float yaw, float pitch);
private:
    char* stringToSet{ nullptr };
};
