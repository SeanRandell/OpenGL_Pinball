#pragma once

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <string>
#include <iostream>

class TextureObject
{
public:
    // constructor (sets default texture modes)
    TextureObject(std::string* filePath, unsigned int wrapS, unsigned int wrapT,
        unsigned int filterMin, unsigned int filterMax, unsigned int internalFormat, unsigned int imageFormat, bool useDataForFormat);
    unsigned int LoadTexture();
    void SetTextureID();
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;

private:
    unsigned int textureID;
    std::string* filePath;
    unsigned int wrapS; // wrapping mode on S axis
    unsigned int wrapT; // wrapping mode on T axis
    unsigned int filterMin; // filtering mode if texture pixels < screen pixels
    unsigned int filterMax; // filtering mode if texture pixels > screen pixels
    // texture Format
    unsigned int internalFormat; // format of texture object
    unsigned int imageFormat; // format of loaded image
    bool useDataForFormat;
};