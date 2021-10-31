#version 460
layout (points) in;
layout (line_strip, max_vertices = 2) out;

const float MAGNITUDE = 1;

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 modelMatrixUniform;

uniform vec3 endVector;

void main()
{
    vec3 endDirectionVector = normalize(endVector);
    mat3 endDirectionMatrix = mat3(transpose(inverse(viewMatrixUniform * modelMatrixUniform)));
    endDirectionVector = normalize(vec3(vec4(endDirectionMatrix * endDirectionVector, 0.0)));

    gl_Position = projectionMatrixUniform * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = projectionMatrixUniform * (gl_in[0].gl_Position + vec4(endDirectionVector, 0.0f) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}