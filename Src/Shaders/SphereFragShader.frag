// GLSL version
#version 460
out vec4 FragmentColor;

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

#define MAX_NUMBER_OF_LIGHTS 10

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


void main()
{
    vec3 viewDirectionVector = normalize(fragmentShaderIn.FragmentPosition - cameraUniform.Position);
    vec3 reflectionVector = reflect(viewDirectionVector, normalize(fragmentShaderIn.Normal));

    // set frag color
    FragmentColor = vec4(texture(skyBoxUniform, reflectionVector).rgb, 1.0);
}

// REFLECT 
//uniform vec3 cameraPos;
//uniform samplerCube skybox;
//
//void main()
//{    
//    vec3 I = normalize(Position - cameraPos);
//    vec3 R = reflect(I, normalize(Normal));
//    FragColor = vec4(texture(skybox, R).rgb, 1.0);
//}



//SPHERE SHADER
//// uniforms
//uniform vec4 lightPosition;             // should be in the eye space
//uniform vec4 lightAmbient;              // light ambient color
//uniform vec4 lightDiffuse;              // light diffuse color
//uniform vec4 lightSpecular;             // light specular color
//uniform vec4 materialAmbient;           // material ambient color
//uniform vec4 materialDiffuse;           // material diffuse color
//uniform vec4 materialSpecular;          // material specular color
//uniform float materialShininess;        // material specular shininess
//uniform sampler2D map0;                 // texture map #1
//uniform bool textureUsed;               // flag for texture
//
//// varyings
//varying vec3 esVertex, esNormal;
//varying vec2 texCoord0;
//
//void main()
//{
//    vec3 normal = normalize(esNormal);
//    vec3 light;
//    if(lightPosition.w == 0.0)
//    {
//        light = normalize(lightPosition.xyz);
//    }
//    else
//    {
//        light = normalize(lightPosition.xyz - esVertex);
//    }
//    vec3 view = normalize(-esVertex);
//    vec3 halfv = normalize(light + view);
//
//    vec3 color = lightAmbient.rgb * materialAmbient.rgb;        // begin with ambient
//    float dotNL = max(dot(normal, light), 0.0);
//    color += lightDiffuse.rgb * materialDiffuse.rgb * dotNL;    // add diffuse
//    if(textureUsed)
//        color *= texture2D(map0, texCoord0).rgb;                // modulate texture map
//    float dotNH = max(dot(normal, halfv), 0.0);
//    color += pow(dotNH, materialShininess) * lightSpecular.rgb * materialSpecular.rgb; // add specular
//
//    // set frag color
//    gl_FragColor = vec4(color, materialDiffuse.a);
//}