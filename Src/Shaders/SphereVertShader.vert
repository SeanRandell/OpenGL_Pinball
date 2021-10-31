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


uniform vec3 offsets[200];

// vertex attribs (input)
//attribute vec3 vertexPosition;
//attribute vec3 vertexNormal;
//attribute vec2 vertexTexCoord;

// varyings (output)
//varying vec3 esVertex, esNormal;
//varying vec2 texCoord0;

void main()
{
//    esVertex = vec3(matrixModelView * vec4(vertexPosition, 1.0));
//    esNormal = vec3(matrixNormal * vec4(vertexNormal, 1.0));
//    texCoord0 = vertexTexCoord;
//    gl_Position = matrixModelViewProjection * vec4(vertexPosition, 1.0);

//    vertexShaderOut.FragmentPosition = vec3(modelMatrixUniform * vec4(aPosition, 1.0));
    vec3 offset = offsets[gl_InstanceID];
    vertexShaderOut.FragmentPosition = aPosition + offset;
//    vertexShaderOut.Normal = mat3(transpose(inverse(modelMatrixUniform))) * aNormal;
    vertexShaderOut.Normal = mat3(transpose(inverse(modelMatrixUniform))) * aNormal;
//    vertexShaderOut.FragmentPosition = vec3(aInstanceModeMatrix * vec4(aPosition, 1.0));
//    vertexShaderOut.Normal = mat3(transpose(inverse(aInstanceModeMatrix))) * aNormal;  

    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4(vertexShaderOut.FragmentPosition, 1.0);
//     gl_Position = projectionMatrixUniform * viewMatrixUniform * modelMatrixUniform * vec4(vertexShaderOut.FragmentPosition, 1.0);
}