#version 460
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VertexData {
    vec3 FragmentPosition;
} gs_in[];

const float MAGNITUDE = 0.2;

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 modelMatrixUniform;

void GenerateLine(int index, vec3 normal)
{
    gl_Position = projectionMatrixUniform * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projectionMatrixUniform * (gl_in[index].gl_Position + vec4(normal, 0.0f) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec3 a = vec3(gs_in[1].FragmentPosition - gs_in[0].FragmentPosition);
    vec3 b = vec3(gs_in[2].FragmentPosition - gs_in[0].FragmentPosition);
    vec3 normal = normalize(cross(a, b));
    mat3 normalMatrix = mat3(transpose(inverse(viewMatrixUniform * modelMatrixUniform)));
    normal = normalize(vec3(vec4(normalMatrix * normal, 0.0)));

    GenerateLine(0, normal); // first vertex normal
    GenerateLine(1, normal); // second vertex normal
    GenerateLine(2, normal); // third vertex normal
}