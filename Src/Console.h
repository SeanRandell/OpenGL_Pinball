#pragma once

#include "StateTracker.h"
class Console
{
public:
    Console() {}
    ~Console() {}
    void Init();
    void End();
    void Render(const char* title, int fps, float pos_x, float pos_y, float pos_z, float yaw, float pitch, StateTracker* stateTracker);

private:
    char* stringToSet{ nullptr };
    std::string IsBallReady(StateTracker* stateTracker);
};
