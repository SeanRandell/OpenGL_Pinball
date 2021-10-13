#version 460
out vec4 FragmentColor;

in vec3 TextureCoordinates;

uniform samplerCube skybox;

void main()
{    
    FragmentColor = texture(skybox, TextureCoordinates);
}