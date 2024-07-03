#version 460 core

layout (location = 0) in InF
{
    vec2 uv;
};

layout (location = 0, binding = 1) uniform sampler2D texColor;

out vec4 FragColor;

void main()
{
    float alpha = texture(texColor, uv).r;
    if (alpha < 0.2) discard;   //  optimal variant for Ortho and perspective Blending

    FragColor = vec4(1,1,1, alpha);
}