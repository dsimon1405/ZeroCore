#version 460 core

    //  color, see enum G_PS_Source::Color::RGBUse
#define PS_Color_RGBU_Add 0
#define PS_Color_RGBU_Replace 1

layout (location = 0) in InF
{
    flat int color_rgb_use;
    vec3 color_rgb;
    float alpha;
    vec2 uv;
} inF;

uniform sampler2D texColor;
// layout (location = 0) uniform sampler2D texColor;

out vec4 FragColor;

void main()
{
    vec4 color = texture(texColor, inF.uv);
    float alpha = color.a * inF.alpha;
    if (alpha == 0.f) discard;
    FragColor = vec4(inF.color_rgb_use == PS_Color_RGBU_Add ? color.rgb + inF.color_rgb : inF.color_rgb, alpha);
}