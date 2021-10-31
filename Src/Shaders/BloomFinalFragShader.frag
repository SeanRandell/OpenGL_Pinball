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