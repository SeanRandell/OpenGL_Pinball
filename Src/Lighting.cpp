#include "Lighting.h"

LightingModel::LightingModel() {
    numberOfLights = 0;
}

int LightingModel::GetNumberOfLights() {
    return numberOfLights;
}

Light* LightingModel::GetLight(int index) {
    return &lights[index];
}

int LightingModel::AddLight(Light light) {
    if (numberOfLights == MAX_LIGHTS) return -1;
    lights[numberOfLights] = light;
    numberOfLights++;
    return 0;
}

Light* LightingModel::GetLights()
{
    return lights;
}
