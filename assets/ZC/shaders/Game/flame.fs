#version 460 core

layout (location = 0) in InF
{
    float life_time_alpha;
    vec2 tex_coords;
} inF;

uniform sampler2D texColor;
// layout (location = 0) uniform sampler2D texColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(texture(texColor, inF.tex_coords).xyz, inF.life_time_alpha);
}