// GLSL version
#version 460
// mesh attributes
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

// per instance attribute
//layout (location = 2) in mat4 aInstanceModeMatrix;

out VertexData{
    vec3 FragmentPosition;
    vec3 Normal;
} vertexShaderOut;

// uniforms
uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

#define MAX_SPHERE_COUNT 300
uniform vec3 offsets[300];

void main()
{
    vec3 offset = offsets[gl_InstanceID];
    vertexShaderOut.FragmentPosition = aPosition + offset;
    vertexShaderOut.Normal = mat3(transpose(inverse(modelMatrixUniform))) * aNormal;
    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4(vertexShaderOut.FragmentPosition, 1.0);
}