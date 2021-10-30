// GLSL version
#version 460
layout (location = 0) out vec4 FragmentColor;
layout (location = 1) out vec4 BrightColor;

in VertexData {
    vec3 FragmentPosition;
    vec3 Normal;
} fragmentShaderIn;

// uniforms
struct Camera {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

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

#define MAX_NUMBER_OF_LIGHTS 12

#define DirectionalLight    0
#define PointLight          1
#define SpotLight           2

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

uniform int numberOfLightsUniform;
uniform Light lightsUniform[MAX_NUMBER_OF_LIGHTS];
uniform Material objectMaterialUniform;
uniform Camera cameraUniform;
uniform samplerCube skyBoxUniform;
uniform bool debugUniform;

void main()
{
    // TODO add light colours to final color
    vec3 viewDirectionVector = normalize(fragmentShaderIn.FragmentPosition - cameraUniform.Position);
    vec3 reflectionVector = reflect(viewDirectionVector, normalize(fragmentShaderIn.Normal));
    vec3 finalColor = texture(skyBoxUniform, reflectionVector).rgb;

    float brightness = dot(finalColor, vec3(0.4126, 0.9152, 0.0922));
    if(brightness > 3.0)
    {
        BrightColor = vec4(finalColor, 1.0);
    }
    else
    {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    if(debugUniform)
    {
        finalColor = vec3(0.0,1.0,0.0);
    }

    // set frag color
    FragmentColor = vec4(finalColor, 1.0);
}