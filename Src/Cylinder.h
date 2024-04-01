// Cylinder for OpenGL with (base radius, top radius, height, sectors, stacks)
// The min number of sectors (slices) is 3 and the min number of stacks are 1.
// base radius : the radius of the cylinder at z = -height / 2
// - top radius : the radiusof the cylinder at z = height/2
// - height     : the height of the cylinder along z-axis
// - sectors    : the number of slices of the base and top caps
// - stacks     : the number of subdivisions along z-axis

// Ideas for Cylinder class sourced from AUTHOR: Song Ho Ahn - http://www.songho.ca/opengl/gl_cylinder.html

#pragma once
#include "Object.h"
#include <vector>

const int MIN_CYLINDER_SECTOR_COUNT = 3;
const int MIN_CYLINDER_STACK_COUNT = 1;

class Cylinder : public Object
{
public:
    // memeber vars
    float baseRadius;
    float topRadius;
    float mass;
    glm::vec2 velocity;
    glm::vec2 acceleration;

    // light Variables
    float lightCooldown;
    float cooldownTimer;
    bool pegHasBeenHit;
    Light* pegLight{ nullptr };

    // ctor/dtor
    Cylinder(float baseRadius = 0.5f, float topRadius = 0.5f, float height = 1.0f,
        int sectorCount = 36, int stackCount = 1, bool smooth = true);
    ~Cylinder() {}

    virtual void Init();
    virtual void Render(ShaderObject* shader);
    virtual void End();

    // for vertex data
    unsigned int GetVertexCount() const { return (unsigned int)vertices.size() / 3; }
    unsigned int GetIndexCount() const { return (unsigned int)indices.size(); }
    unsigned int GetIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const unsigned int* GetIndices() const { return indices.data(); }

    // for interleaved vertices: V/N/T
    unsigned int GetInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(unsigned int); }    // # of bytes
    const float* GetInterleavedVertices() const { return &interleavedVertices[0]; }

    const char* GetName();
    bool IsContainedWithin(Rectangle* boundary);
    void UpdatePegLights(float deltaTime);
    void ResetCooldownTimer();

private:
    // member functions
    void ClearArrays();
    void BuildVerticesSmooth();
    void BuildInterleavedVertices();
    void BuildUnitCircleVertices();
    void AddVertex(float x, float y, float z);
    void AddNormal(float x, float y, float z);
    void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> GetSideNormals();
    std::vector<float> ComputeFaceNormal(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3);

    float height;
    int sectorCount;                        // # of slices
    int stackCount;                         // # of stacks
    unsigned int baseIndex;                 // starting index of base
    unsigned int topIndex;                  // starting index of top
    bool smooth;
    std::vector<float> unitCircleVertices;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;

    // interleaved
    std::vector<float> interleavedVertices;
    int interleavedStride;                  // # of bytes to hop to the next vertex (should be 32 bytes)
};

