#include <string>

class ShaderSources {
public:
    const char* BlockFragShader = R"(
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
        vec3 ambient = vec3(0.0, 0.0, 0.0);
        if(lightsUniform[currentLight].type == PointLight)
        {
            ambient = (lightsUniform[currentLight].ambient * 4) * texture(objectDefaultMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb;
        }
        else
        {
             ambient = lightsUniform[currentLight].ambient * texture(objectDefaultMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb;
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
            diffuseVector  = (lightsUniform[currentLight].diffuse * 4) * diffuseFloat * texture(objectDefaultMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb;
        }
        else
        {
            diffuseVector  = lightsUniform[currentLight].diffuse * diffuseFloat * texture(objectDefaultMaterialUniform.diffuseMap, fragmentShaderIn.TextureCoordinates).rgb;
        }
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
)";

    const char* BlockGeomShader = R"(
#version 460
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VertexData {
    vec3 FragmentPosition;
    vec2 TextureCoordinates;
} gs_in[];

out VertexData {
    vec3 FragmentPosition;
    vec3 Normal;
    vec2 TextureCoordinates;
} gs_out;

void main() {
    vec3 a = vec3(gs_in[1].FragmentPosition - gs_in[0].FragmentPosition);
    vec3 b = vec3(gs_in[2].FragmentPosition - gs_in[0].FragmentPosition);
    vec3 normal = normalize(cross(a, b));

    gl_Position = gl_in[0].gl_Position;
    gs_out.FragmentPosition = gs_in[0].FragmentPosition;
    gs_out.Normal = normal;
    gs_out.TextureCoordinates = gs_in[0].TextureCoordinates;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position;
    gs_out.FragmentPosition = gs_in[1].FragmentPosition;
    gs_out.Normal = normal;
    gs_out.TextureCoordinates = gs_in[1].TextureCoordinates;
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position;
    gs_out.FragmentPosition = gs_in[2].FragmentPosition;
    gs_out.Normal = normal;
    gs_out.TextureCoordinates = gs_in[2].TextureCoordinates;
    EmitVertex();
 
    EndPrimitive();
}

)";

    const char* BlockVertShader = R"(
#version 460
layout (location = 0) in vec3 aPosition;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTextureCoordinate;

out VertexData{
    vec3 FragmentPosition;
    vec2 TextureCoordinates;
} vertexShaderOut;

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

void main()
{
    vertexShaderOut.FragmentPosition = vec3(modelMatrixUniform * vec4(aPosition, 1.0f));
    vertexShaderOut.TextureCoordinates = aTextureCoordinate;
    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4(vertexShaderOut.FragmentPosition, 1.0f);
}    
)";

    const char* BloomFinalFragShader = R"(
#version 460
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sceneUniform;
uniform sampler2D bloomBlurUniform;
uniform bool bloom;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(sceneUniform, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlurUniform, TexCoords).rgb;
    if(bloom)
        hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correct
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}
)";

    const char* BloomFinalVertShader = R"(
#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
)";

    const char* BloomFragShader = R"(

)";

    const char* BlurFragShader = R"(
#version 460
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D imageUniform;

uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{             
     vec2 tex_offset = 1.0 / textureSize(imageUniform, 0); // gets size of single texel
     vec3 result = texture(imageUniform, TexCoords).rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(imageUniform, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(imageUniform, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture(imageUniform, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(imageUniform, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }
     FragColor = vec4(result, 1.0);
}
)";

    const char* BlurVertShader = R"(
#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
)";

    const char* CubeFragShader = R"(
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

in vec3 FragmentPosition; 
in vec3 Normal;  

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

uniform int numberOfLightsUniform;
uniform Light lightsUniform[MAX_NUMBER_OF_LIGHTS];
uniform Material objectMaterialUniform;
uniform Camera cameraUniform;
uniform bool debugUniform;

void main() 
{
    vec3 normalVector = normalize(transpose(inverse(mat3(modelMatrixUniform))) * fragmentShaderIn.Normal);
    
    vec3 finalColor = vec3(0.0, 0.0, 0.0);
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
    
    FragmentColor = vec4(finalColor, 1.0);
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

)";

    const char* CubeGeomShader = R"(
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

)";

    const char* CubeVertShader = R"(
#version 460
layout (location = 0) in vec3 aPosition;   // the position variable has attribute position 0

out VertexData{
    vec3 FragmentPosition;
} vertexShaderOut;

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

void main()
{
    vertexShaderOut.FragmentPosition = vec3(modelMatrixUniform * vec4(aPosition, 1.0));

    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4(vertexShaderOut.FragmentPosition, 1.0);
}
)";

    const char* DebugQuadDepthFragShader = R"(
#version 460
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{             
    float depthValue = texture(depthMap, TexCoords).r;
    // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}
)";

    const char* DebugQuadDepthVertShader = R"(
#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
)";

    const char* DepthFragShader = R"(
#version 460

void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
}
)";

    const char* DepthVertShader = R"(
#version 460
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrixUniform;
uniform mat4 modelMatrixUniform;

void main()
{
    gl_Position = lightSpaceMatrixUniform * modelMatrixUniform * vec4(aPos, 1.0);
}
)";

    const char* DirectionalFragShader = R"(
#version 460
layout (location = 0) out vec4 FragmentColor;

uniform vec3 lineColor;

void main()
{
    FragmentColor = vec4(lineColor, 1.0);
}

)";

    const char* DirectionalGeomShader = R"(
#version 460
layout (points) in;
layout (line_strip, max_vertices = 2) out;

const float MAGNITUDE = 1;

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 modelMatrixUniform;

uniform vec3 endVector;

void main()
{
    vec3 endDirectionVector = normalize(endVector);
    mat3 endDirectionMatrix = mat3(transpose(inverse(viewMatrixUniform * modelMatrixUniform)));
    endDirectionVector = normalize(vec3(vec4(endDirectionMatrix * endDirectionVector, 0.0)));

    gl_Position = projectionMatrixUniform * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = projectionMatrixUniform * (gl_in[0].gl_Position + vec4(endDirectionVector, 0.0f) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}
)";

    const char* DirectionalVertShader = R"(
#version 460 core
layout (location = 0) in vec3 aPosition;

out VertexData{
    vec3 FragmentPosition;
} vertexShaderOut;

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 modelMatrixUniform;

void main()
{
    vertexShaderOut.FragmentPosition = vec3(modelMatrixUniform * vec4(aPosition, 1.0));
    gl_Position = viewMatrixUniform * modelMatrixUniform * vec4(aPosition, 1.0); 
}
)";

    const char* LightingFragShader = R"(
#version 460 core
layout (location = 0) out vec4 FragmentColor;
layout (location = 1) out vec4 BrightColor;

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

uniform bool fixedColor;

void main()
{
    FragmentColor = vec4(lightUniform.diffuse, 1.0); // set alle 4 vector values to 1.0
    if(fixedColor){
        FragmentColor = vec4(1.0, 0.0 , 0.0, 1.0);
    }
    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(FragmentColor.rgb, vec3(0.3126, 0.8152, 0.0822));
    if(brightness > 1.0)
        BrightColor = vec4(FragmentColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
)";

    const char* LightingGeomShader = R"(

)";

    const char* LightingVertexShader = R"(
#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

void main()
{
    gl_Position = projectionMatrixUniform * viewMatrixUniform * modelMatrixUniform * vec4(aPos, 1.0);
} 
)";

    const char* NormalFragShader = R"(
#version 460
out vec4 FragmentColor;

void main()
{
    FragmentColor = vec4(1.0, 1.0, 0.0, 1.0);
}

)";

    const char* NormalGeomShader = R"(
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
)";

    const char* NormalVertShader = R"(
#version 460 core
layout (location = 0) in vec3 aPosition;

out VertexData{
    vec3 FragmentPosition;
} vertexShaderOut;

uniform mat4 viewMatrixUniform;
uniform mat4 modelMatrixUniform;
uniform mat4 projectionMatrixUniform;

void main()
{
    vertexShaderOut.FragmentPosition = vec3(modelMatrixUniform * vec4(aPosition, 1.0));
    gl_Position = viewMatrixUniform * modelMatrixUniform * vec4(aPosition, 1.0); 
}
)";

    const char* ParticleFragShader = R"(
#version 460
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D sprite;

void main()
{
    color = (texture(sprite, TexCoords) * ParticleColor);
}
)";

    const char* ParticleVertShader = R"(
#version 460
layout (location = 0) in vec3 aPosition;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTextureCoordinate;

struct Camera {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
};

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform vec2 offset;
uniform vec4 color;

uniform Camera cameraUniform;

void main()
{
    float scale = 0.2f;
    TexCoords = aTextureCoordinate;
    ParticleColor = color;
    vec3 vertexPositionWolrdSpace = vec3(offset,0.0) + cameraUniform.Right * (aPosition.x * scale) +
    cameraUniform.Up * (aPosition.y * scale);
    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4(vertexPositionWolrdSpace, 1.0);
}
)";

    const char* SkyBoxFragShader = R"(
#version 460
layout (location = 0) out vec4 FragmentColor;
layout (location = 1) out vec4 BrightColor;

in vec3 TextureCoordinates;

uniform samplerCube skyBoxUniform;

void main()
{    
    vec3 finalColor = texture(skyBoxUniform, TextureCoordinates).rgb;
    float brightness = dot(finalColor, vec3(0.4126, 0.9152, 0.0922));
    if(brightness > 3.0)
    {
        BrightColor = vec4(finalColor, 1.0);
    }
    else
    {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    FragmentColor = vec4(finalColor, 1.0);
}
)";

    const char* SkyBoxVertShader = R"(
#version 460
layout (location = 0) in vec3 aPosition;

out vec3 TextureCoordinates;

uniform mat4 projectionMatrixUniform;
uniform mat4 viewMatrixUniform;

void main()
{
    TextureCoordinates = aPosition;
    vec4 position = projectionMatrixUniform * viewMatrixUniform * vec4(aPosition, 1.0);
    gl_Position = position.xyww;
}  
)";

    const char* SphereFragShader = R"(
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
)";

    const char* SphereGeomShader = R"(
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

)";

    const char* SphereVertShader = R"(
// GLSL version
#version 460
// mesh attributes
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

// per instance attribute
//layout (location = 2) in mat4 aInstanceModeMatrix;

out VertexData{
    vec3 FragmentPosition;
    vec3 Normal;
} vertexShaderOut;

// uniforms
uniform mat4 modelMatrixUniform;
uniform mat4 viewMatrixUniform;
uniform mat4 projectionMatrixUniform;

#define MAX_SPHERE_COUNT 300
uniform vec3 offsets[300];

void main()
{
    vec3 offset = offsets[gl_InstanceID];
    vertexShaderOut.FragmentPosition = aPosition + offset;
    vertexShaderOut.Normal = mat3(transpose(inverse(modelMatrixUniform))) * aNormal;
    gl_Position = projectionMatrixUniform * viewMatrixUniform * vec4(vertexShaderOut.FragmentPosition, 1.0);
}
)";

};
