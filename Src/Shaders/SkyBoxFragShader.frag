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