#version 460

subroutine vec3 blockType();
subroutine uniform blockType block;

layout (location = 0) out vec4 FragmentColor;
layout (location = 1) out vec4 BrightColor;


in VertexData {
    vec3 FragmentPosition;
    vec3 Normal;
    vec2 TextureCoordinates;
} fragmentShaderIn;

struct Camera {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
};

struct Material {

    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D reflectionMap;
    float shininess; 
    // float reflectiveness
}; 

struct DefaultMaterial
{
    sampler2D diffuseMap;
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
uniform DefaultMaterial objectDefaultMaterialUniform;
uniform Camera cameraUniform;
uniform samplerCube skyBoxUniform;

uniform bool debugUniform;

subroutine(blockType)
vec3 OneTexture()
{
    vec3 finalColor = vec3(0.0, 0.0, 0.0);
    
    vec3 normalVector = normalize(transpose(inverse(mat3(modelMatrixUniform))) * fragmentShaderIn.Normal);
    
    for (int currentLight = 0; currentLight < numberOfLightsUniform; currentLight++) 
    {
        if(lightsUniform[currentLight].isLightOn == false)
        {
            continue;
        }

        float attenuation = 1.0;

        // calc ambient
        vec3 ambient = lightsUniform[currentLight].ambient * texture(objectDefaultMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb;


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

        vec3 diffuseVector  = lightsUniform[currentLight].diffuse * diffuseFloat * texture(objectDefaultMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb;

        // calc specular
        vec3 fragmentToCameraVector = normalize(cameraUniform.Position - fragmentShaderIn.FragmentPosition);
        // Phong
        //vec3 R = reflect(-L, N);  
        //float s = pow(max(dot(R, V), 0.0), u_ObjectMaterial.Shininess);
        // Blinn-Phong
        vec3 BlinnPhongVector = normalize(lightDirectionVector + fragmentToCameraVector);
        float specularFloat = pow(max(dot(normalVector, BlinnPhongVector), 0.0), objectMaterialUniform.shininess);

        vec3 specularVector =  lightsUniform[currentLight].specular * specularFloat * objectDefaultMaterialUniform.specular;

        finalColor += (attenuation*(ambient + diffuseVector + specularVector));
    }

    return finalColor;
}

subroutine(blockType)
vec3 ThreeTextures()
{
    vec3 finalColor = vec3(0.0, 0.0, 0.0);

    vec3 normalVector = normalize(transpose(inverse(mat3(modelMatrixUniform))) * fragmentShaderIn.Normal);
    
    for (int currentLight = 0; currentLight < numberOfLightsUniform; currentLight++) 
    {
        if(lightsUniform[currentLight].isLightOn == false)
        {
            continue;
        }

        float attenuation = 1.0;

        // calc ambient
        vec3 ambient = lightsUniform[currentLight].ambient * texture(objectMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb;


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

        vec3 diffuseVector = lightsUniform[currentLight].diffuse * diffuseFloat * texture(objectMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb;


        // calc specular
        vec3 fragmentToCameraVector = normalize(cameraUniform.Position - fragmentShaderIn.FragmentPosition);
        // Phong
        //vec3 R = reflect(-L, N);  
        //float s = pow(max(dot(R, V), 0.0), u_ObjectMaterial.Shininess);
        // Blinn-Phong
        vec3 BlinnPhongVector = normalize(lightDirectionVector + fragmentToCameraVector);
        float specularFloat = pow(max(dot(normalVector, BlinnPhongVector), 0.0), objectMaterialUniform.shininess);

        vec3  specularVector = lightsUniform[currentLight].specular * specularFloat * texture(objectMaterialUniform.specularMap, fragmentShaderIn.TextureCoordinates).rgb;

        //reflection
        vec3 viewDirectionVector = normalize(fragmentShaderIn.FragmentPosition - cameraUniform.Position);
        vec3 reflectionVector = reflect(viewDirectionVector, normalize(fragmentShaderIn.Normal));

        vec3 reflection = vec3(texture(skyBoxUniform, reflectionVector).rgb * texture(objectMaterialUniform.reflectionMap, fragmentShaderIn.TextureCoordinates).rgb);

        specularVector*= 2;
        specularVector += (reflection * 3) ;

        finalColor += (attenuation*(ambient + diffuseVector + specularVector));
//        finalColor += (ambient + attenuation*(diffuseVector + specularVector));
    }

    return finalColor;
}

void main() 
{
    vec3 finalColor = vec3(0.0, 0.0, 0.0);
    finalColor = block();

    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(finalColor, vec3(0.4126, 0.9152, 0.0922));
    //vec3(0.2126, 0.7152, 0.0722)
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

    FragmentColor = vec4(finalColor, 1.0);
}

//uniform sampler2D shadowMap;
//
//uniform Light directionalLight;
/*
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fragmentShaderIn.Normal);
    vec3 lightDir = normalize(directionalLight.position - fragmentShaderIn.FragmentPosition);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

*/