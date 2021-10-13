#version 460
out vec4 FragmentColor;

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
    sampler2D diffuse;
    sampler2D specular;    
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

//in vec3 FragmentPosition; 
//in vec3 Normal;  

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

uniform int numberOfLightsUniform;
uniform Light lightsUniform[MAX_NUMBER_OF_LIGHTS];
uniform Material objectMaterialUniform;
uniform Camera cameraUniform;

void main() 
{
    //vec3 N = normalize(cross(dFdx(fragmentShaderIn.FragPos), dFdy(fragmentShaderIn.FragPos)));
    vec3 normalVector = normalize(transpose(inverse(mat3(modelMatrixUniform))) * fragmentShaderIn.Normal);
    
    vec3 finalColor = vec3(0.0, 0.0, 0.0);
    for (int currentLight = 0; currentLight < numberOfLightsUniform; currentLight++) {
        float attenuation = 1.0;

        // calc ambient
        vec3 ambient = lightsUniform[currentLight].ambient * texture(objectMaterialUniform.diffuse, fragmentShaderIn.TextureCoordinates).rgb;
    
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
        vec3 diffuseVector = lightsUniform[currentLight].diffuse * texture(objectMaterialUniform.diffuse, fragmentShaderIn.TextureCoordinates).rgb * diffuseFloat;
    
        // calc specular
        vec3 fragmentToCameraVector = normalize(cameraUniform.Position - fragmentShaderIn.FragmentPosition);
        // Phong
        //vec3 R = reflect(-L, N);  
        //float s = pow(max(dot(R, V), 0.0), u_ObjectMaterial.Shininess);
        // Blinn-Phong
        vec3 BlinnPhongVector = normalize(lightDirectionVector + fragmentToCameraVector);
        float specularFloat = pow(max(dot(normalVector, BlinnPhongVector), 0.0), objectMaterialUniform.shininess);
        vec3 specularVector = lightsUniform[currentLight].specular * texture(objectMaterialUniform.specular, fragmentShaderIn.TextureCoordinates).rgb * specularFloat;

        finalColor += (ambient + attenuation*(diffuseVector + specularVector));
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