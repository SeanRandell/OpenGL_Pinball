#version 460 core
out vec4 FragmentColor;

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

uniform Light lightUniform;

void main()
{
    FragmentColor = vec4(lightUniform.diffuse, 1.0); // set alle 4 vector values to 1.0
}