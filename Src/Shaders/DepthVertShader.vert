#version 460
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrixUniform;
uniform mat4 modelMatrixUniform;

void main()
{
    gl_Position = lightSpaceMatrixUniform * modelMatrixUniform * vec4(aPos, 1.0);
}