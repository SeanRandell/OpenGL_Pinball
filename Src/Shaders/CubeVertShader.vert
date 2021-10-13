#version 460
layout (location = 0) in vec3 aPosition;   // the position variable has attribute position 0
//layout (location = 1) in vec3 aNormal;

out VertexData{
    vec3 FragmentPosition;
} vertexShaderOut;

//out vec3 Normal;

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

void main()
{
    vertexShaderOut.FragmentPosition = vec3(modelMatrixUniform * vec4(aPosition, 1.0));
//    Normal = mat3(transpose(inverse(modelMatrixUniform))) * aNormal;  
//    gl_Position = transform * vec4(aPos, 1.0);
    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4(vertexShaderOut.FragmentPosition, 1.0);
//    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}    