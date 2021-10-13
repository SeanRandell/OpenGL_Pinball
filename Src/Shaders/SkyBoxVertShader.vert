#version 460
layout (location = 0) in vec3 aPosition;

out vec3 TextureCoordinates;

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;

void main()
{
    TextureCoordinates = aPosition;
    vec4 position = projectionMatrixUniform * viewMatrixUniform * vec4(aPosition, 1.0);
    gl_Position = position.xyww;
}  