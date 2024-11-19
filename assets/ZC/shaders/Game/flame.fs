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
    vec3 color = texture(texColor, inF.tex_coords).xyz;
    float particle_alpha = (color.x + color.y + color.z) / 3.f;
    // FragColor = vec4(color, inF.life_time_alpha);
    float alpha = particle_alpha * inF.life_time_alpha;
    if (alpha == 0.f) discard;
    FragColor = vec4(color, alpha);
}