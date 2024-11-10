#version 460 core

in vec2 vTexCoords;

uniform sampler2D texColor;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(texColor, vTexCoords);
    // float offset = 100.f;
    float offset = 0.5f;
    FragColor = vec4(texColor.xyz + vec3(offset, offset, offset), 0.3);
}