#version 460 core

in vec2 vTexCoords;

uniform sampler2D texColor;
uniform uint unColor;

out vec4 FragColor;

void main()
{
    float alpha = texture(texColor, vTexCoords).r;
    if (alpha < 0.2) discard;   //  optimal variant for Ortho and perspective Blending

    float r = (unColor >> 20) / 255.f;
    float g = (unColor >> 10 & uint(1023)) / 255.f;
    float b = (unColor & uint(1023)) / 255.f;
    
    FragColor = vec4(vec3(r, g, b), alpha);
}