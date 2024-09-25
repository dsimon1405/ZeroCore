#version 460 core

out vec4 FragColor;

in vec2 TexCoords;
in float vColor_coef;
// in vec4 vColor;

uniform sampler2D texColor;

void main()
{    
    vec3 color = texture(texColor, TexCoords).xyz;
    if (color.x > 0.2f && color.y > 0.2f && color.z > 0.2f) color = vec3(1, 0, 0);
    FragColor = vec4(color * vColor_coef, 1);
    // FragColor = vec4(texture(texColor, TexCoords).xyz, 1);
}