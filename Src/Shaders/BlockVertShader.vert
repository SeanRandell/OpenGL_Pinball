#version 460
layout (location = 0) in vec3 aPosition;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTextureCoordinate;

out VertexData{
    vec3 FragmentPosition;
    vec2 TextureCoordinates;
} vertexShaderOut;

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

void main()
{
    vertexShaderOut.FragmentPosition = vec3(modelMatrixUniform * vec4(aPosition, 1.0f));
    vertexShaderOut.TextureCoordinates = aTextureCoordinate;
    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4(vertexShaderOut.FragmentPosition, 1.0f);
}    