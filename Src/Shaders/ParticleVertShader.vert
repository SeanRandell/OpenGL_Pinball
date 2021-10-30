#version 460
layout (location = 0) in vec3 aPosition;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTextureCoordinate;

struct Camera {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
};

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform vec2 offset;
uniform vec4 color;

uniform Camera cameraUniform;

void main()
{
    float scale = 0.2f;
//    float scale = 10.0f;
    TexCoords = aTextureCoordinate;
    ParticleColor = color;
    vec3 vertexPositionWolrdSpace = vec3(offset,0.0) + cameraUniform.Right * (aPosition.x * scale) +
    cameraUniform.Up * (aPosition.y * scale);
    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4(vertexPositionWolrdSpace, 1.0);
//    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4((aPosition.xy * scale) + offset , 0.0, 1.0);
}