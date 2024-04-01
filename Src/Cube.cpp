#include "Cube.h"

void Cube::Init()
{
    numberOfVertices = 8;
    numberOfFaces = 12;
    vertexPoints = new VertexPoint[]{
        { -1, -1,  1 }, // 0
        {  1, -1,  1 }, // 1
        {  1,  1,  1 }, // 2
        { -1,  1,  1 }, // 3
        {  1, -1, -1 }, // 4
        { -1, -1, -1 }, // 5 
        { -1,  1, -1 }, // 6
        {  1,  1, -1 } // 7
    };
    faces = new ObjectFace[]{
        { 1, 4, 7 }, { 1, 7, 2 },   // +x
        { 5, 0, 3 }, { 5, 3, 6 },   // -x
        { 3, 2, 7 }, { 3, 7, 6 },   // +y
        { 5, 4, 1 }, { 5, 1, 0 },   // -y
        { 0, 1, 2 }, { 0, 2, 3 },   // +z
        { 4, 5, 6 }, { 4, 6, 7 }    // -z
    };
    Object::Init();
}

const char* Cube::GetName()
{
    return "Cube";
}