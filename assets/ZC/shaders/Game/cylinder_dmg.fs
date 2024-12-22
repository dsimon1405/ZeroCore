#version 460 core
    //  in
layout (location = 0) in InF
{
    float frag_z;
} inF;

uniform float unAlpha;

    //  out
out vec4 FragColor;

void main()
{
    float frag_z = abs(inF.frag_z);
    const float z_max = 25.f;
    float alpha = (1.f - (frag_z / z_max)) * unAlpha;
    
    if (alpha == 0.f) discard;

    FragColor = vec4(1.f, 0.f, 0.f, alpha);
}