#version 460 core
//  in
struct ZC_GUI_Border
{
    vec2 bl;     //  coord of frame border, to avoid drawing objects out of window
    vec2 tr;
};
layout (std430, binding = 0) readonly buffer InBorder { ZC_GUI_Border borders[]; } inBorder;

layout (location = 0) in InF
{
    flat int borderIndex;       //  if -1, don't need border check
    flat uint tex_binding;     //  0 is tex_Icon, 1 is tex_Text(alpha)
    vec2 uv;
    flat uint color;
} inF;

layout (location = 0, binding = 0) uniform sampler2D tex_Icon;
layout (location = 1, binding = 1) uniform sampler2D tex_Text;
    
//  out
layout (location = 0) out vec4 FragColor;

void main()
{
    if (inF.borderIndex != -1)   //  gl_FragCoord.x = window's width, gl_FragCoord.y = window's height, in pixels
    {
        ZC_GUI_Border border = inBorder.borders[inF.borderIndex];
        if (border.bl.x > gl_FragCoord.x || border.bl.y > gl_FragCoord.y || border.tr.x < gl_FragCoord.x || border.tr.y < gl_FragCoord.y) discard; 
    }

    if (inF.tex_binding == 0) FragColor = texture(tex_Icon, inF.uv);
    else FragColor = vec4(0, 0, 0, texture(tex_Text, inF.uv).r);

    if (FragColor.a == 0) discard;

    if (inF.color != 0)
    {
        FragColor.r += (inF.color >> 20) / 255.f;
        FragColor.g += (inF.color >> 10 & uint(1023)) / 255.f;
        FragColor.b += (inF.color & uint(1023)) / 255.f;
    }
}








//      its text shader
// #version 460 core

// in vec2 vTexCoords;

// uniform sampler2D texColor;
// uniform uint unColor;
// uniform float unAlpha = 1;  //  uses only in ZC_TextWindow

// out vec4 FragColor;

// void main()
// {
//     if (unAlpha == 0) discard;  //  check users alpha from uniform (sets only in ZC_TextWindow)

//     float alpha = texture(texColor, vTexCoords).r;
//     if (alpha < 0.2) discard;   //  optimal variant for Ortho and perspective Blending

//     alpha *= unAlpha;   //  reduce font's alpha to users alpha (only for ZC_TextWindow)

//     float r = (unColor >> 20) / 255.f;
//     float g = (unColor >> 10 & uint(1023)) / 255.f;
//     float b = (unColor & uint(1023)) / 255.f;

//     FragColor = vec4(r, g, b, alpha);
// }