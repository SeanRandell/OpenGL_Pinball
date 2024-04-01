#define STB_IMAGE_IMPLEMENTATION
#include "TextureObject.h"

TextureObject::TextureObject(std::string* filePath, unsigned int wrapS, unsigned int wrapT,
    unsigned int filterMin, unsigned int filterMax, unsigned int internalFormat, unsigned int imageFormat, bool useDataForFormat)
{
    this->filePath = filePath;
    this->textureID = 0;

    this->wrapS = wrapS;
    this->wrapT = wrapT;
    this->filterMin = filterMin;
    this->filterMax = filterMax;
    this->internalFormat = internalFormat;
    this->imageFormat = imageFormat;
    this->useDataForFormat = useDataForFormat;
}

void TextureObject::SetTextureID()
{
    this->textureID = LoadTexture();
}

unsigned int TextureObject::LoadTexture()
{

    unsigned int textureID;
    glGenTextures(1, &textureID);

    // load image, create texture and generate mipmaps
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(filePath->c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);

        GLenum format = 0;
        // if object formats are negative (i.e. didnt set them)
        if (useDataForFormat)
        {
            if (nrChannels == 1)
            {
                format = GL_RED;
            }
            else if (nrChannels == 3)
            {
                format = GL_RGB;
            }
            else if (nrChannels == 4)
            {
                format = GL_RGBA;
            }
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_REPEAT

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin); // GL_LINEAR_MIPMAP_LINEAR
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax); // GL_LINEAR

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
