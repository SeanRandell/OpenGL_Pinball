#include "Block.h"

Block::Block()
{
    position = OBJECT_DEFAULT_POSITION;
    rotation = OBJECT_DEFAULT_ROTATION;
    scale = OBJECT_DEFAULT_SCALE;
    this->isBumper = false;
    this->isScenery = false;
    this->isActiveFlipper = false;
    InitConstructorValues();
}

Block::Block(bool isBumper)
{
    position = OBJECT_DEFAULT_POSITION;
    rotation = OBJECT_DEFAULT_ROTATION;
    scale = OBJECT_DEFAULT_SCALE;
    this->isBumper = isBumper;
    this->isScenery = false;
    this->isActiveFlipper = false;
    InitConstructorValues();
}

Block::Block(glm::vec3 newPosition, glm::vec3 rotation, glm::vec3 scale, bool isScenery)
{
    this->position = newPosition;
    this->rotation = rotation;
    this->scale = scale;
    this->isBumper = false;
    this->isActiveFlipper = false;
    this->isScenery = isScenery;
    InitConstructorValues();
}

void Block::InitConstructorValues()
{
    glm::vec3 length = glm::vec3(1.0, 1.0, 1.0);
    glm::vec2 rep = glm::vec2(1.0, 1.0);

    float xl = length.x / 2.0f;
    float yl = length.y / 2.0f;
    float zl = length.z / 2.0f;
    float rr = rep.r;
    float sr = rep.s;
    numberOfVertices = 36;
    //material = { {1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 }, 64.0f };
    material = { {0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 }, { 0.508273, 0.508273, 0.508273 }, 64.0f };
    vertexAndTexturePoints = new VertexAndTexturePoint[]{
        // points tex_coords
        { -xl, -yl, zl, 0, 0 }, // front
        { xl, -yl, zl, rr, 0 },
        { xl, yl, zl, rr, sr },
        { xl, yl, zl, rr, sr },
        { -xl, yl, zl, 0, sr },
        { -xl, -yl, zl, 0, 0 },

        { xl, -yl, zl, 0, 0 }, // right
        { xl, -yl, -zl, rr, 0 },
        { xl, yl, -zl, rr, sr },
        { xl, yl, -zl, rr, sr },
        { xl, yl, zl, 0, sr },
        { xl, -yl, zl, 0, 0 },

        { xl, -yl, -zl, 0, 0 }, // back
        { -xl, -yl, -zl, rr, 0 },
        { -xl, yl, -zl, rr, sr },
        { -xl, yl, -zl, rr, sr },
        { xl, yl, -zl, 0, sr },
        { xl, -yl, -zl, 0, 0 },

        { -xl, -yl, -zl, 0, 0 }, // left
        { -xl, -yl, zl, rr, 0 },
        { -xl, yl, zl, rr, sr },
        { -xl, yl, zl, rr, sr },
        { -xl, yl, -zl, 0, sr },
        { -xl, -yl, -zl, 0, 0 },

        { -xl, yl, zl, 0, 0 }, // top
        { xl, yl, zl, rr, 0 },
        { xl, yl, -zl, rr, sr },
        { xl, yl, -zl, rr, sr },
        { -xl, yl, -zl, 0, sr },
        { -xl, yl, zl, 0, 0 },

        { -xl, -yl, -zl, 0, 0 }, // bottom
        { xl, -yl, -zl, rr, 0 },
        { xl, -yl, zl, rr, sr },
        { xl, -yl, zl, rr, sr },
        { -xl, -yl, zl, 0, sr },
        { -xl, -yl, -zl, 0, 0 },
    };
}

void Block::Init()
{

    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(VertexAndTexturePoint), vertexAndTexturePoints, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Block::Render(
    RTRShader* shader, SkyBox* skybox, TextureObject* diffuseMap, TextureObject* specularMap, TextureObject* reflectionMap)
{
    //shader->SetMaterial("objectMaterialUniform", material);
    shader->SetVec3("objectMaterialUniform.ambient", material.ambient);
    shader->SetFloat("objectMaterialUniform.shininess", material.shininess);

    //modelMatrix = glm::mat4(1.0f);
    //glm::translate(modelMatrix, position);
    //glm::scale(modelMatrix, scale);
    //shader->SetMat4("modelMatrixUniform", modelMatrix);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    diffuseMap->Bind();

    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    specularMap->Bind();

    glActiveTexture(GL_TEXTURE2);
    reflectionMap->Bind();

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemapTexture);

    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
    glBindVertexArray(0);
}

void Block::RenderNormalBlock(RTRShader* shader, SkyBox* skybox, TextureObject* diffuseMap)
{
    shader->SetVec3("objectDefaultMaterialUniform.specular", material.specular);
    shader->SetFloat("objectDefaultMaterialUniform.shininess", material.shininess);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE4);
    diffuseMap->Bind();

    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
    glBindVertexArray(0);
}

void Block::End()
{
    glDeleteVertexArrays(1, &vertexArray); vertexArray = 0;
    glDeleteBuffers(1, &vertexBuffer); vertexBuffer = 0;

    if (vertexAndTexturePoints != nullptr)
    {
        delete vertexAndTexturePoints;
        vertexAndTexturePoints = nullptr;
    }
}

const char* Block::GetName() {
    return "Block";
}

bool Block::IsContainedWithin(Rectangle* boundary) {
    float rectALeft = boundary->left;
    float rectARight = boundary->right;
    float rectBLeft = this->position.x - this->scale.x / 2;
    float rectBRight = this->position.x + this->scale.x / 2;

    bool x = rectALeft < rectBRight && rectARight > rectBLeft;

    float rectATop = boundary->top;
    float rectABottom = boundary->bottom;
    float rectBTop = this->position.y + this->scale.y / 2;
    float rectBBottom = this->position.y - this->scale.y / 2;

    bool y = rectATop > rectBBottom && rectABottom < rectBTop;

    return x && y;
}