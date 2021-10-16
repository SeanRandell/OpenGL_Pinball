#version 460
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VertexData {
    vec3 FragmentPosition;
} gs_in[];

const float MAGNITUDE = 0.6;

struct Light {
    int type;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
//    float cutoff;
//    float OuterCutoff;
};

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 modelMatrixUniform;

uniform Light lightUniform;

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

    vec3 lightDirectionVector = normalize(lightUniform.direction);
    mat3 lightDirectionMatrix = mat3(transpose(inverse(viewMatrixUniform * modelMatrixUniform)));
    lightDirectionVector = normalize(vec3(vec4(lightDirectionMatrix * lightDirectionVector, 0.0)));

    GenerateLine(0, lightDirectionVector); // first vertex normal
    GenerateLine(1, lightDirectionVector); // second vertex normal
    GenerateLine(2, lightDirectionVector); // third vertex normal
}

//layout (triangles) in;
//layout (line_strip, max_vertices = 6) out;
//
//in VertexData {
//    vec3 FragmentPosition;
//} gs_in[];
//
//const float MAGNITUDE = 0.6;
//
//struct Light {
//    int type;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//    vec3 direction;
//    vec3 position;
//    float constant;
//    float linear;
//    float quadratic;
////    float cutoff;
////    float OuterCutoff;
//};
//
//uniform mat4 projectionMatrixUniform;
//uniform mat4 viewMatrixUniform;
//uniform mat4 modelMatrixUniform;
//
//uniform Light lightUniform;
//
//void GenerateLine(int index, vec3 normal)
//{
//    gl_Position = projectionMatrixUniform * gl_in[index].gl_Position;
//    EmitVertex();
//    gl_Position = projectionMatrixUniform * (gl_in[index].gl_Position + vec4(lightUniform.direction, 0.0f) * MAGNITUDE);
//    EmitVertex();
//    EndPrimitive();
//}
//
//void main()
//{
//    vec3 a = vec3(gs_in[1].FragmentPosition - gs_in[0].FragmentPosition);
//    vec3 b = vec3(gs_in[2].FragmentPosition - gs_in[0].FragmentPosition);
//    vec3 normal = normalize(cross(a, b));
//    mat3 normalMatrix = mat3(transpose(inverse(viewMatrixUniform * modelMatrixUniform)));
//    normal = normalize(vec3(vec4(normalMatrix * normal, 0.0)));
//
//    GenerateLine(0, normal); // first vertex normal
//    GenerateLine(1, normal); // second vertex normal
//    GenerateLine(2, normal); // third vertex normal
//}