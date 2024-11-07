#version 460 core

in vec2 vTexCoords;

uniform sampler2D texColor;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(texColor, vTexCoords);
    FragColor = vec4(texColor.xyz + vec3(0.5f, 0.5f, 0.5f), 0.3);
}