#pragma once

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <string>
#include <iostream>

class TextureObject {
public:
    // constructor (sets default texture modes)
    TextureObject(std::string* filePath);
    unsigned int LoadTexture();
    void SetTextureID();
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;

private:
    unsigned int textureID;
    std::string* filePath;
    //unsigned int Wrap_S; // wrapping mode on S axis
    //unsigned int Wrap_T; // wrapping mode on T axis
    //unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    //unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
};