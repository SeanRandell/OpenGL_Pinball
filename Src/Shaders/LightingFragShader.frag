#version 460 core
//out vec4 FragmentColor;
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