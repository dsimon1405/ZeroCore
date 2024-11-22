#version 460 core

    //  std 430 don't use mat and vec types!
// struct Particle
// {
//     float secs_to_start;

//     float pos_start[3];
//     float pos_cur[3];
    
//     float life_secs_total;
//     float life_secs_cur;

//     float dir_move_normalized[3];
// };
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
    float texture_alpha = (color.x + color.y + color.z) / 3.f;
    // FragColor = vec4(color, inF.life_time_alpha);
    float alpha = texture_alpha * inF.life_time_alpha;
    if (alpha == 0.f) discard;
    FragColor = vec4(color, alpha);
}