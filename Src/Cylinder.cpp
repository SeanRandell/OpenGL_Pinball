#include "Cylinder.h"

Cylinder::Cylinder(float baseRadius, float topRadius, float height, int sectors,
    int stacks, bool smooth)
{
    this->baseRadius = baseRadius;
    this->topRadius = topRadius;
    this->height = height;
    this->sectorCount = sectors;
    if (sectors < MIN_CYLINDER_SECTOR_COUNT)
    {
        this->sectorCount = MIN_CYLINDER_SECTOR_COUNT;
    }
    this->stackCount = stacks;
    if (stacks < MIN_CYLINDER_STACK_COUNT)
    {
        this->stackCount = MIN_CYLINDER_STACK_COUNT;
    }
    this->smooth = smooth;

    this->mass = 1.0f;
    this->velocity = glm::vec2(0.0, 0.0);
    this->acceleration = glm::vec2(0.0, 0.0);

    // light variables
    this->lightCooldown = 1.0;
    this->cooldownTimer = 0.0;
    this->pegHasBeenHit = false;

    // generate unit circle vertices first
    BuildUnitCircleVertices();

    BuildVerticesSmooth();
}

void Cylinder::Init()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, GetInterleavedVertexSize(), GetInterleavedVertices(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // bind vbo for smooth sphere
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // set attrib arrays using glVertexAttribPointer()
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(attribVertexTexCoord, 2, GL_FLOAT, false, stride, (void*)(6 * sizeof(float)));

    glGenBuffers(1, &faceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexSize(), GetIndices(), GL_STATIC_DRAW);

}

//void Cylinder::Render(ShaderObject* shader, unsigned int cubeMapTexture)
void Cylinder::Render(ShaderObject* shader)
{
    shader->SetMaterial("objectMaterialUniform", material);
    glBindVertexArray(vertexArray);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    //glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);


    // draw center sphere
    glDrawElements(GL_TRIANGLES,            // primitive type
        GetIndexCount(), // # of indices
        GL_UNSIGNED_INT,         // data type
        (void*)0);               // ptr to indices

    glBindVertexArray(0);
}

void Cylinder::End()
{
    Object::End();
}

void Cylinder::ClearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

///////////////////////////////////////////////////////////////////////////////
// build vertices of cylinder with smooth shading
// where v: sector angle (0 <= v <= 360)
///////////////////////////////////////////////////////////////////////////////
void Cylinder::BuildVerticesSmooth()
{
    // clear memory of prev arrays
    ClearArrays();

    float x, y, z;                                  // vertex position
    //float s, t;                                     // texCoord
    float radius;                                   // radius for each stack

    // get normals for cylinder sides
    std::vector<float> sideNormals = GetSideNormals();

    // put vertices of side cylinder to array by scaling unit circle
    for (int i = 0; i <= stackCount; ++i)
    {
        z = -(height * 0.5f) + (float)i / stackCount * height;      // vertex position z
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);     // lerp
        float t = 1.0f - (float)i / stackCount;   // top-to-bottom

        for (int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            x = unitCircleVertices[k];
            y = unitCircleVertices[k + 1];
            AddVertex(x * radius, y * radius, z);   // position
            AddNormal(sideNormals[k], sideNormals[k + 1], sideNormals[k + 2]); // normal
            //AddTexCoord((float)j / sectorCount, t); // tex coord
        }
    }

    // remember where the base.top vertices start
    unsigned int baseVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of base of cylinder
    z = -height * 0.5f;
    AddVertex(0, 0, z);
    AddNormal(0, 0, -1);
    //AddTexCoord(0.5f, 0.5f);
    for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j + 1];
        AddVertex(x * baseRadius, y * baseRadius, z);
        AddNormal(0, 0, -1);
        //AddTexCoord(-x * 0.5f + 0.5f, -y * 0.5f + 0.5f);    // flip horizontal
    }

    // remember where the base vertices start
    unsigned int topVertexIndex = (unsigned int)vertices.size() / 3;

    // put vertices of top of cylinder
    z = height * 0.5f;
    AddVertex(0, 0, z);
    AddNormal(0, 0, 1);
    //AddTexCoord(0.5f, 0.5f);
    for (int i = 0, j = 0; i < sectorCount; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j + 1];
        AddVertex(x * topRadius, y * topRadius, z);
        AddNormal(0, 0, 1);
        //addTexCoord(x * 0.5f + 0.5f, -y * 0.5f + 0.5f);
    }

    // put indices for sides
    unsigned int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // bebinning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 trianles per sector
            AddIndices(k1, k1 + 1, k2);
            AddIndices(k2, k1 + 1, k2 + 1);

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            // horizontal lines
            lineIndices.push_back(k2);
            lineIndices.push_back(k2 + 1);
            if (i == 0)
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // remember where the base indices start
    baseIndex = (unsigned int)indices.size();

    // put indices for base
    for (int i = 0, k = baseVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < (sectorCount - 1))
            AddIndices(baseVertexIndex, k + 1, k);
        else    // last triangle
            AddIndices(baseVertexIndex, baseVertexIndex + 1, k);
    }

    // remember where the base indices start
    topIndex = (unsigned int)indices.size();

    for (int i = 0, k = topVertexIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < (sectorCount - 1))
            AddIndices(topVertexIndex, k, k + 1);
        else
            AddIndices(topVertexIndex, k, topVertexIndex + 1);
    }

    // generate interleaved vertex array as well
    BuildInterleavedVertices();
}

///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
void Cylinder::BuildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i;
    std::size_t count = vertices.size();
    for (i = 0; i < count; i += 3)
    {
        //interleavedVertices.push_back(vertices[i]);
        //interleavedVertices.push_back(vertices[i + 1]);
        //interleavedVertices.push_back(vertices[i + 2]);
        interleavedVertices.insert(interleavedVertices.end(), &vertices[i], &vertices[i] + 3);

        //interleavedVertices.push_back(normals[i]);
        //interleavedVertices.push_back(normals[i + 1]);
        //interleavedVertices.push_back(normals[i + 2]);
        interleavedVertices.insert(interleavedVertices.end(), &normals[i], &normals[i] + 3);

        //interleavedVertices.push_back(texCoords[j]);
        //interleavedVertices.push_back(texCoords[j + 1]);
    }
}

///////////////////////////////////////////////////////////////////////////////
// generate 3D vertices of a unit circle on XY plance
///////////////////////////////////////////////////////////////////////////////
void Cylinder::BuildUnitCircleVertices()
{
    // TODO check conversion
    const float PI = (float)acos(-1);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    std::vector<float>().swap(unitCircleVertices);
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }
}


void Cylinder::AddVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void Cylinder::AddNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

void Cylinder::AddIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

///////////////////////////////////////////////////////////////////////////////
// generate shared normal vectors of the side of cylinder
///////////////////////////////////////////////////////////////////////////////
std::vector<float> Cylinder::GetSideNormals()
{
    // TODO check conversion
    const float PI = (float)acos(-1);
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    // compute the normal vector at 0 degree first
    // tanA = (baseRadius-topRadius) / height
    float zAngle = atan2(baseRadius - topRadius, height);
    float x0 = cos(zAngle);     // nx
    float y0 = 0;               // ny
    float z0 = sin(zAngle);     // nz

    // rotate (x0,y0,z0) per sector angle
    std::vector<float> normals;
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        normals.push_back(cos(sectorAngle) * x0 - sin(sectorAngle) * y0);   // nx
        normals.push_back(sin(sectorAngle) * x0 + cos(sectorAngle) * y0);   // ny
        normals.push_back(z0);  // nz
        /*
        //debug
        float nx = cos(sectorAngle)*x0 - sin(sectorAngle)*y0;
        float ny = sin(sectorAngle)*x0 + cos(sectorAngle)*y0;
        std::cout << "normal=(" << nx << ", " << ny << ", " << z0
                  << "), length=" << sqrtf(nx*nx + ny*ny + z0*z0) << std::endl;
        */
    }
    return normals;
}

///////////////////////////////////////////////////////////////////////////////
// return face normal of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
///////////////////////////////////////////////////////////////////////////////
std::vector<float> Cylinder::ComputeFaceNormal(float x1, float y1, float z1,  // v1
    float x2, float y2, float z2,  // v2
    float x3, float y3, float z3)  // v3
{
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}

const char* Cylinder::GetName()
{
    return "Peg";
}

bool Cylinder::IsContainedWithin(Rectangle* boundary) {
    return (
        this->position.x + this->topRadius >= boundary->x - boundary->width &&
        this->position.x + this->topRadius <= boundary->x + boundary->width &&
        this->position.y + this->topRadius >= boundary->y - boundary->width &&
        this->position.y + this->topRadius <= boundary->y + boundary->width
        );
}

//void Cylinder::UpdatePegLights(Light* light, float deltaTime)
void Cylinder::UpdatePegLights(float deltaTime)
{
    if (pegLight == nullptr)
    {
        return;
    }
    // if the peg bool is false and the countdown is greater than the cooldown then return
    //light is false
    if (!pegHasBeenHit && cooldownTimer >= lightCooldown)
    {
        //light->isLightOn = false;
        pegLight->isLightOn = false;
        return;
    }
    //if the peg bool has been reset = true
    //reset the countdown to 0 and make the peg bool false
    // light is true
    if (pegHasBeenHit)
    {
        //light->isLightOn = true;
        pegLight->isLightOn = true;
        ResetCooldownTimer();
        pegHasBeenHit = false;
    }

    //if countdown is less than cooldown then + deltatime to countdown 
    if (cooldownTimer < lightCooldown)
    {
        cooldownTimer += deltaTime;
    }
}

void Cylinder::ResetCooldownTimer()
{
    this->cooldownTimer = 0.0f;
}
