#version 460
layout (location = 0) out vec4 FragmentColor;

uniform vec3 lineColor;

void main()
{
    FragmentColor = vec4(lineColor, 1.0);
}
