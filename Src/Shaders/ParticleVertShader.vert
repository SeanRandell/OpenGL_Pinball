#version 460
layout (location = 0) in vec3 aPosition;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTextureCoordinate;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform vec2 offset;
uniform vec4 color;

void main()
{
    float scale = 1.0f;
//    float scale = 10.0f;
    TexCoords = aTextureCoordinate;
    ParticleColor = color;
    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4((aPosition.xy * scale) + offset , 0.0, 1.0);
}