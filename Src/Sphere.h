#pragma once
#include "Object.h"
#include <vector>
//-----------------------------------------------------------------------------

// Ideas for sphere class sourced from AUTHOR: Song Ho Ahn - http://www.songho.ca/opengl/gl_sphere.html

// constants //////////////////////////////////////////////////////////////////
const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

class Sphere : public Object
{
public:

    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    bool smooth;
    std::vector<float> vertices;
    std::vector<float> normals;
    //std::vector<float> texCoords;
    //std::vector<unsigned int> indices;
    //std::vector<unsigned int> lineIndices;

    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)

    Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18, bool smooth = true);
    ~Sphere() {}
    virtual void Init();
    virtual void Render(RTRShader* shader, unsigned int cubeMapTexture);
    virtual void End();
    virtual const char* GetName();
    void BuildVerticesSmooth();
    void BuildInterleavedVertices();
    void ClearArrays();
    void AddVertex(float x, float y, float z);
    void AddNormal(float x, float y, float z);
    int GetInterleavedStride() const { return interleavedStride; }   // should be 32 bytes
    unsigned int GetInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytes
    const float* GetInterleavedVertices() const { return interleavedVertices.data(); }
};
