#version 460 core

out vec4 FragColor;

in vec2 TexCoords;
in float vColor_coef;
// in vec4 vColor;

uniform sampler2D texColor;

void main()
{    
    // FragColor = vec4(1,1,1,1);
    // FragColor = vColor;
    FragColor = vec4(texture(texColor, TexCoords).xyz * vColor_coef, 1);
    // FragColor = texture(texture_diffuse1, TexCoords);
}