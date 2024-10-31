#version 460 core

layout (location = 0) in InF
{
    vec2 uv;
};

layout (location = 1) uniform sampler2D tex_Text;

out vec4 FragColor;

void main()
{
    float alpha = texture(tex_Text, uv).r;
    if (alpha < 0.2) discard;   //  optimal variant for Ortho and perspective Blending

    FragColor = vec4(1,1,1, alpha);
}