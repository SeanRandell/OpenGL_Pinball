// GLSL version
#version 460
layout (location = 0) out vec4 FragmentColor;
layout (location = 1) out vec4 BrightColor;

in VertexData {
    vec3 FragmentPosition;
    vec3 Normal;
} fragmentShaderIn;

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
    bool isLightOn;
//    float cutoff;
//    float OuterCutoff;
};

#define MAX_NUMBER_OF_LIGHTS 13

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
    vec3 finalColor = vec3(0.0, 0.0, 0.0);
    vec3 viewDirectionVector = normalize(fragmentShaderIn.FragmentPosition - cameraUniform.Position);
    vec3 reflectionVector = reflect(viewDirectionVector, normalize(fragmentShaderIn.Normal));
    finalColor = texture(skyBoxUniform, reflectionVector).rgb;

    vec3 normalVector = normalize(transpose(inverse(mat3(modelMatrixUniform))) * fragmentShaderIn.Normal);
    
    for (int currentLight = 0; currentLight < numberOfLightsUniform; currentLight++) 
    {
        if(lightsUniform[currentLight].isLightOn == false)
        {
            continue;
        }
        float attenuation = 1.0;

        // calc ambient
        vec3 ambient = vec3(0.0, 0.0, 0.0);
        if(lightsUniform[currentLight].type == PointLight)
        {
            ambient = (lightsUniform[currentLight].ambient * 4) * objectMaterialUniform.ambient;
        }
        else
        {
             ambient = lightsUniform[currentLight].ambient * objectMaterialUniform.ambient;
        }

        // calc diffuse
        vec3 lightDirectionVector;
        if (lightsUniform[currentLight].type == DirectionalLight) {
            lightDirectionVector = -lightsUniform[currentLight].direction;
        }
        else {
            lightDirectionVector = lightsUniform[currentLight].position - fragmentShaderIn.FragmentPosition;
            float lightDirectionVectorLength = length(lightDirectionVector);
            attenuation = min(
                1.0 / (
                  (lightsUniform[currentLight].constant) +
                  (lightsUniform[currentLight].linear * lightDirectionVectorLength) +
                  (lightsUniform[currentLight].quadratic * lightDirectionVectorLength * lightDirectionVectorLength)
                ), 1);
        }
        lightDirectionVector = normalize(lightDirectionVector);
        float diffuseFloat = max(dot(normalVector, lightDirectionVector), 0.0);

        vec3 diffuseVector = vec3(0.0, 0.0, 0.0);
        if(lightsUniform[currentLight].type == PointLight)
        {
            diffuseVector = (lightsUniform[currentLight].diffuse * 4) * diffuseFloat * objectMaterialUniform.diffuse;
        }
        else 
        {
            diffuseVector = lightsUniform[currentLight].diffuse * diffuseFloat * objectMaterialUniform.diffuse;
        }
    
        // calc specular
        vec3 fragmentToCameraVector = normalize(cameraUniform.Position - fragmentShaderIn.FragmentPosition);
        // Phong
        //vec3 R = reflect(-L, N);  
        //float s = pow(max(dot(R, V), 0.0), u_ObjectMaterial.Shininess);
        // Blinn-Phong
        vec3 BlinnPhongVector = normalize(lightDirectionVector + fragmentToCameraVector);
        float specularFloat = pow(max(dot(normalVector, BlinnPhongVector), 0.0), objectMaterialUniform.shininess);
        vec3 specularVector = lightsUniform[currentLight].specular * specularFloat * objectMaterialUniform.specular;

        finalColor += (attenuation*(ambient + diffuseVector + specularVector));
     }

    float brightness = dot(finalColor, vec3(0.4126, 0.9152, 0.0922));
    if(brightness > 2.0)
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

    FragmentColor = vec4(finalColor, 1.0);
}