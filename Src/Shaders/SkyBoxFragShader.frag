#version 460
out vec4 FragmentColor;

in vec3 TextureCoordinates;

uniform samplerCube skyBoxUniform;

void main()
{    
    FragmentColor = texture(skyBoxUniform, TextureCoordinates);
}