#version 460 core

    //  in
uniform sampler2D texColor;

layout (location = 0) in InF
{
    vec2 tex_coords;
    flat bool normal_aligned_to_cam;
} inF;

    //  out
out vec4 FragColor;

void main()
{
    if (inF.normal_aligned_to_cam) discard;

    vec4 texColor = texture(texColor, inF.tex_coords);
    // float light_plus = 100.f;
    float light_plus = 0.5f;
    FragColor = vec4(texColor.xyz + vec3(light_plus, light_plus, light_plus), 0.3);
}