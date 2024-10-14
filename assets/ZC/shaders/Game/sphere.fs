#version 460 core

out vec4 FragColor;

layout (std140, binding = 1) uniform Light
{
    vec4 pos;
    vec4 color;
} light;

uniform sampler2D texColor;

uniform uint unColor = 0;
uniform float unAlpha = 1.f;

    //  in
layout (location = 0) in InF
{
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords;
    float color_coef;   //  coef on dmg color
} inF;

void main()
{
    // FragColor = vec4(texture(texColor, inF.tex_coords).xyz ,1);
    vec3 color = texture(texColor, inF.tex_coords).xyz;
    if (unColor != 0 && color.x > 0.1f && color.y > 0.1f && color.z > 0.1f)
    {

        color.r += (unColor >> 20) / 255.f;
        color.g += (unColor >> 10 & uint(1023)) / 255.f;
        color.b += (unColor & uint(1023)) / 255.f;
        FragColor = vec4(color * inF.color_coef, unAlpha);
    }
    else
    {
        const float constant_c = 1.0;
        const float linear_c = 0.014;
        const float quadratic_c = 0.00007;

        vec3 light_pos = vec3(light.pos);
        vec3 light_color = vec3(light.color) * 3.0;
        
        // attenuation
        float distance = length(light_pos - inF.frag_pos);
        float attenuation = 1.0 / (constant_c + linear_c * distance + quadratic_c * (distance * distance));
        
        // ambient
        vec3 ambient = 0.2 * texture(texColor, inF.tex_coords).rgb * light_color * attenuation;
        
        // diffuse 
        vec3 normal = normalize(inF.normal);
        vec3 lightDir = normalize(light_pos - inF.frag_pos);    //  fragment look on light
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = 0.5 * diff * texture(texColor, inF.tex_coords).rgb * light_color * attenuation;

        FragColor = vec4(ambient + diffuse, unAlpha);
    }
}