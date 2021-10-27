#define STB_IMAGE_IMPLEMENTATION
#include "TextureObject.h"

TextureObject::TextureObject(std::string filepath)
{
    this->filePath = filePath;
}

unsigned int TextureObject::LoadTexture() {

    //std::cout << testString << std::endl;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    // load image, create texture and generate mipmaps
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
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



void TextureObject::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->textureID);
}
