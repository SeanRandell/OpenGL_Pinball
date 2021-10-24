#version 460
//out vec4 FragmentColor;
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
    vec3 ambient;
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D reflectionMap;
    float shininess; 
    // float reflectiveness
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

//in vec3 FragmentPosition; 
//in vec3 Normal;  

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

uniform int numberOfLightsUniform;
uniform Light lightsUniform[MAX_NUMBER_OF_LIGHTS];
uniform Material objectMaterialUniform;
uniform Camera cameraUniform;
uniform samplerCube skyBoxUniform;

uniform sampler2D shadowMap;

uniform Light directionalLight;

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

void main() 
{
    //vec3 N = normalize(cross(dFdx(fragmentShaderIn.FragPos), dFdy(fragmentShaderIn.FragPos)));
    vec3 normalVector = normalize(transpose(inverse(mat3(modelMatrixUniform))) * fragmentShaderIn.Normal);
    
    vec3 finalColor = vec3(0.0, 0.0, 0.0);
    for (int currentLight = 0; currentLight < numberOfLightsUniform; currentLight++) {
        float attenuation = 1.0;

        // calc ambient
        vec3 ambient = lightsUniform[currentLight].ambient * texture(objectMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb * objectMaterialUniform.ambient;
    
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
        vec3 diffuseVector = lightsUniform[currentLight].diffuse * texture(objectMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb * diffuseFloat;
    
        // calc specular
        vec3 fragmentToCameraVector = normalize(cameraUniform.Position - fragmentShaderIn.FragmentPosition);
        // Phong
        //vec3 R = reflect(-L, N);  
        //float s = pow(max(dot(R, V), 0.0), u_ObjectMaterial.Shininess);
        // Blinn-Phong
        vec3 BlinnPhongVector = normalize(lightDirectionVector + fragmentToCameraVector);
        float specularFloat = pow(max(dot(normalVector, BlinnPhongVector), 0.0), objectMaterialUniform.shininess);
        vec3 specularVector = lightsUniform[currentLight].specular * texture(objectMaterialUniform.specularMap, fragmentShaderIn.TextureCoordinates).rgb * specularFloat;

        //reflection
        vec3 viewDirectionVector = normalize(fragmentShaderIn.FragmentPosition - cameraUniform.Position);
        vec3 reflectionVector = reflect(viewDirectionVector, normalize(fragmentShaderIn.Normal));

        vec3 reflection = vec3(texture(skyBoxUniform, reflectionVector).rgb * texture(objectMaterialUniform.reflectionMap, fragmentShaderIn.TextureCoordinates).rgb);

//        vec3 reflection = vec3(texture(skyboxUniform, reflectionVector).rgb * texture(objectMaterialUniform.reflectionMap, fragmentShaderIn.TextureCoordinates).rgb);
//        specularVector += reflection;

        finalColor += (ambient + attenuation*(diffuseVector + specularVector + reflection));
//        finalColor += (ambient + attenuation*(diffuseVector + specularVector));
    }
    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(finalColor, vec3(0.1126, 0.5152, 0.0522));
    //vec3(0.2126, 0.7152, 0.0722)
    if(brightness > 1.0)
    {
        BrightColor = vec4(finalColor, 1.0);
    }
    else
    {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    FragmentColor = vec4(finalColor, 1.0);
//    FragmentColor = vec4(0.0, 1.0, 0.0, 1.0); 
}

//uniform bool isLightingOnUniform;

// function prototypes
//vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
//vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);

//void main()
//{
//// if (!isLightingOn){
//    //    // ambient
//    //    vec3 ambient = light.ambient * material.ambient;
//    //  	
//    //    // diffuse 
//    //    vec3 norm = normalize(Normal);
//    //    vec3 lightDir = normalize(light.position - FragPos);
//    //    float diff = max(dot(norm, lightDir), 0.0);
//    //    vec3 diffuse = light.diffuse * (diff * material.diffuse);
//
//        // ambient
//        vec3 ambient = material.ambient;
//  	
//        // diffuse 
//        vec3 normal = normalize(Normal);
//        vec3 lightDirection = normalize(viewPosition - FragmentPosition);
//        float diffuseFloat = max(dot(normal, lightDirection), 0.0f);
//        vec3 diffuseVector = material.diffuse * (diffuseFloat * material.diffuse);
//
//        vec3 result = ambient + diffuseVector;
//        FragColor = vec4(result, 1.0);
//} else {
//
//        // properties
//        vec3 normal = normalize(Normal);
//        vec3 viewDirection = normalize(viewPosition - FragmentPosition);
//
//        // phase 1: directional lighting
//        vec3 result = vec3(0.0,0.0,0.0);
//        result += CalculateDirectionalLight(directionalLight, normal, viewDirection);
//        // phase 2: point lights
//        for(int i = 0; i < numberOfLights; i++)
//        {
//            result += CalculatePointLight(pointLights[i], normal, FragmentPosition, viewDirection); 
//        }
//
//    //    vec3 result = ambient + diffuse + specular;
//        FragColor = vec4(result, 1.0);
//    }
//} 
//
// calculates the color when using a directional light.
//vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
//{
//    vec3 lightDirection = normalize(-light.direction);
//    // diffuse shading
//    float diffuseFloat = max(dot(normal, lightDirection), 0.0);
//    // specular shading
//    vec3 reflectDirection = reflect(-lightDirection, normal);
////    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//    // blinn specular
//    float specularFloat = 0.0;
//    vec3 halfwayDirection = normalize(lightDirection + viewDirection);  
////    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
//    specularFloat = pow(max(dot(halfwayDirection, normal), 0.0),  material.shininess);
//
//    // combine results
//    vec3 ambient = light.ambient * material.diffuse;
//    vec3 diffuseVector = light.diffuse * diffuseFloat * material.diffuse;
//    vec3 specularVector = light.specular * specularFloat * material.specular;
//    return (ambient + diffuseVector + specularVector);
//}

// calculates the color when using a point light.
//vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection)
//{
//    vec3 lightDirection = normalize(light.position - fragmentPosition);
//    // diffuse shading
//    float diffuseFloat = max(dot(normal, lightDirection), 0.0);
//    // specular shading
////    vec3 reflectDir = reflect(-lightDir, normal);
////    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//    // blinn specular
//    float specularFloat = 0.0;
//    vec3 halfwayDirection = normalize(lightDirection + viewDirection);  
//    specularFloat = pow(max(dot(halfwayDirection, normal), 0.0), 32.0);
//
//    // attenuation
//    float distance = length(light.position - fragmentPosition);
//    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
//    
//    // combine results
//    vec3 ambient = light.ambient * material.diffuse;
//    vec3 diffuseVector = light.diffuse * diffuseFloat * material.diffuse;
//    vec3 specularVector = light.specular * specularFloat * material.specular;
////    ambient *= attenuation;
////    diffuse *= attenuation;
////    specular *= attenuation;
//    return (ambient + diffuseVector + specularVector);
//}