#include "Sphere.h"

Sphere::Sphere(int id)
{
    radius = 0.5f;
    sectorCount = 36;
    smooth = true;
    stackCount = 18;
    this->velocity = glm::vec2(0.0f);
    this->acceleration = glm::vec2(0.0f);
    this->id = id;
    InitConstructorValues();
}

Sphere::Sphere(float radius, float mass, glm::vec2 velocity, int sectors, int stacks, bool smooth, int id) {
    this->radius = radius;
    this->sectorCount = sectors;
    this->stackCount = stacks;
    this->smooth = smooth;

    this->velocity = velocity;
    this->acceleration = glm::vec2(0.0f);
    this->id = id;
    InitConstructorValues();
}

void Sphere::InitConstructorValues()
{
    this->mass = radius * 5.0f;
    if (sectorCount < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;

    if (stackCount < MIN_STACK_COUNT)
        this->stackCount = MIN_STACK_COUNT;
    BuildVerticesSmooth();
}

void Sphere::Init()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, GetInterleavedVertexSize(), GetInterleavedVertices(), GL_STATIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, GetInterleavedVertexSize() + sizeof(normals), sizeof(tex),GL_DYNAMIC_DRAW);
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // bind vbo for smooth sphere (center and right)
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // set attrib arrays using glVertexAttribPointer()
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(attribVertexTexCoord, 2, GL_FLOAT, false, stride, (void*)(6 * sizeof(float)));

    //modelmatrix


    glGenBuffers(1, &faceElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexSize(), GetIndices(), GL_STATIC_DRAW);

}

void Sphere::Render(RTRShader* shader, unsigned int cubeMapTexture, unsigned int sphereCount)
{
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBindVertexArray(vertexArray);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    //glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);


    // draw center sphere
    //glDrawElements(GL_TRIANGLES,            // primitive type
    //    GetIndexCount(), // # of indices
    //    GL_UNSIGNED_INT,         // data type
    //    (void*)0);               // ptr to indices

    glDrawElementsInstanced(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, (void*)0, sphereCount);

    glBindVertexArray(0);
}

void Sphere::End()
{
    Object::End();
}

void Sphere::BuildVerticesSmooth()
{
    const float PI = acos(-1);

    // clear memory of prev arrays
    ClearArrays();

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    //float s, t;                                     // texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            AddVertex(x, y, z);
            numberOfVertices++;

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            AddNormal(nx, ny, nz);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
            {
                AddIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
            }

            if (i != (stackCount - 1))
            {
                AddIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
            }

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // generate interleaved vertex array as well
    BuildInterleavedVertices();

    //int k = 0;
    //int j = 1;
    //fprintf(stderr, "Start\n");
    //for (size_t i = 0; i < interleavedVertices.size(); i++)
    //{
    //    //fprintf(stderr, "mouse x: %d, y: %d\n", keyEvent.button.x, keyEvent.button.y);
    //    fprintf(stderr, "%f, ", interleavedVertices[i]);
    //    k++;
    //    //fprintf(stderr, " - index: %lu\n, ", i);
    //    if (k == 6) {
    //        fprintf(stderr, " - index: %d,\n", j);
    //        j++;
    //        k = 0;
    //    }
    //}
    //fprintf(stderr, "End\n");
}

///////////////////////////////////////////////////////////////////////////////
// generate interleaved vertices: V/N/T
// stride must be 32 bytes
///////////////////////////////////////////////////////////////////////////////
void Sphere::BuildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i;
    std::size_t count = vertices.size();
    for (i = 0; i < count; i += 3)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i + 1]);
        interleavedVertices.push_back(vertices[i + 2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i + 1]);
        interleavedVertices.push_back(normals[i + 2]);

        //interleavedVertices.push_back(texCoords[j]);
        //interleavedVertices.push_back(texCoords[j + 1]);
    }
}

///////////////////////////////////////////////////////////////////////////////
// dealloc vectors
///////////////////////////////////////////////////////////////////////////////
void Sphere::ClearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

///////////////////////////////////////////////////////////////////////////////
// add single vertex to array
///////////////////////////////////////////////////////////////////////////////
void Sphere::AddVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

///////////////////////////////////////////////////////////////////////////////
// add single normal to array
///////////////////////////////////////////////////////////////////////////////
void Sphere::AddNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

void Sphere::AddIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

const char* Sphere::GetName()
{
    return "Sphere";
}

bool Sphere::IsContainedWithin(Rectangle* boundary) {
    return (
        this->position.x + this->radius >= boundary->x - boundary->width &&
        this->position.x + this->radius <= boundary->x + boundary->width &&
        this->position.y + this->radius >= boundary->y - boundary->width &&
        this->position.y + this->radius <= boundary->y + boundary->width
        );
}