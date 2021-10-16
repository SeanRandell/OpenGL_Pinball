#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

void main()
{
    gl_Position = projectionMatrixUniform * viewMatrixUniform * modelMatrixUniform * vec4(aPos, 1.0);
} 