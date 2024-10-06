#version 460 core

out vec4 FragColor;

uniform sampler2D texColor;

uniform uint unColor = 0;
uniform float unAlpha = 1.f;

    //  out
layout (location = 0) in InF
{
    vec2 tex_coords;
    float color_coef;
} inF;

void main()
{    
    vec3 color = texture(texColor, inF.tex_coords).xyz;
    if (unColor != 0 && color.x > 0.1f && color.y > 0.1f && color.z > 0.1f)
    {

        color.r += (unColor >> 20) / 255.f;
        color.g += (unColor >> 10 & uint(1023)) / 255.f;
        color.b += (unColor & uint(1023)) / 255.f;
    }
    FragColor = vec4(color * inF.color_coef, unAlpha);
    // FragColor = vec4(texture(texColor, TexCoords).xyz, 1);
}