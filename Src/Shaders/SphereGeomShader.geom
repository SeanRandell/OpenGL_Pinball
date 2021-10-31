#version 460
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VertexData {
    vec3 FragmentPosition;
} gs_in[];

out VertexData {
    vec3 FragmentPosition;
    vec3 Normal;
} gs_out;

void main() {
    vec3 a = vec3(gs_in[1].FragmentPosition - gs_in[0].FragmentPosition);
    vec3 b = vec3(gs_in[2].FragmentPosition - gs_in[0].FragmentPosition);
    vec3 normal = normalize(cross(a, b));

    gl_Position = gl_in[0].gl_Position;
    gs_out.FragmentPosition = gs_in[0].FragmentPosition;
    gs_out.Normal = normal;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position;
    gs_out.FragmentPosition = gs_in[1].FragmentPosition;
    gs_out.Normal = normal;
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position;
    gs_out.FragmentPosition = gs_in[2].FragmentPosition;
    gs_out.Normal = normal;
    EmitVertex();
 
    EndPrimitive();
}
