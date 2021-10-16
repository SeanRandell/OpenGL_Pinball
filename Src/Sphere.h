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
    Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18, bool smooth = true);
    ~Sphere() {}
    virtual void Init();
    virtual void Render(RTRShader* shader, unsigned int cubeMapTexture);
    virtual void End();

private:
    // memeber vars
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    bool smooth;
    std::vector<float> vertices;
    std::vector<float> normals;
    //std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;

    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)

    virtual const char* GetName();
    void BuildVerticesSmooth();
    void BuildInterleavedVertices();
    void ClearArrays();
    void AddVertex(float x, float y, float z);
    void AddNormal(float x, float y, float z);
    void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    int GetInterleavedStride() const { return interleavedStride; }   // should be 32 bytes
    unsigned int GetInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); }    // # of bytes
    const float* GetInterleavedVertices() const { return interleavedVertices.data(); }
    unsigned int GetIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const unsigned int* GetIndices() const { return indices.data(); }
    unsigned int GetIndexCount() const { return (unsigned int)indices.size(); }
};
