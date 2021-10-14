#include "Block.h"

Block::Block(glm::vec3 length, glm::vec2 rep)
{
    float xl = length.x / 2.0f;
    float yl = length.y / 2.0f;
    float zl = length.z / 2.0f;
    float rr = rep.r;
    float sr = rep.s;
    numberOfVertices = 36;
    diffuseMap = 0;
    specularMap = 0;
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
    std::cout << "child called" << std::endl;
}

void Block::Init()
{
    diffusePath = "../Assignment2/Src/container2.png";
    specularPath = "../Assignment2/Src/container2_specular.png";
    //const char* testString = "../Assignment2/Src/container2.png";
    //const char* testString1 = "../Assignment2/Src/container2_specular.png";
    diffuseMap = LoadTexture(&diffusePath);
    specularMap = LoadTexture(&specularPath);

    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(VertexAndTexturePoint), vertexAndTexturePoints, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Block::Render(RTRShader* shader)
{
    //shader->SetMaterial("objectMaterialUniform", material);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glBindVertexArray(vertexArray);
    //glDrawElements(GL_TRIANGLES, numberOfFaces * 3, GL_UNSIGNED_INT, 0);
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

// TODO - should add texture param for different blocks
unsigned int Block::LoadTexture(std::string* path) {

    //std::cout << testString << std::endl;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    // load image, create texture and generate mipmaps
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
     unsigned char* data = stbi_load(path->c_str(), &width, &height, &nrChannels, 0);
    //unsigned char* data = stbi_load(skyBoxFaces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        }
        else if (nrChannels == 3) {
            format = GL_RGB;
        }
        else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

const char* Block::GetName() {
    return "Block";
}