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
        //  colors
    float red;
    float green;
    float blue;
    float alpha;     //  alpha sets only in color pack uint32 -> 8x8x8x8 in ZC_GUI_ColorManipulator
} inF;

layout (location = 0, binding = 0) uniform sampler2D tex_Icon;
layout (location = 1, binding = 1) uniform sampler2D tex_Text;
    
//  out
layout (location = 0) out vec4 FragColor;

void AddColorsToTexture();

void main()
{
    if (inF.borderIndex != -1)   //  gl_FragCoord.x = window's width, gl_FragCoord.y = window's height, in pixels
    {
        ZC_GUI_Border border = inBorder.borders[inF.borderIndex];
        if (border.bl.x > gl_FragCoord.x || border.bl.y > gl_FragCoord.y || border.tr.x < gl_FragCoord.x || border.tr.y < gl_FragCoord.y) discard; 
    }

    switch (inF.tex_binding)    //  classification in ZC_GUI_Bindings.h
    {
    case 0:     //  icons texture
    {
        FragColor = texture(tex_Icon, inF.uv);
        AddColorsToTexture();
    } break;
    case 1:     //  text texture
    {
        FragColor = vec4(0, 0, 0, texture(tex_Text, inF.uv).r);
        AddColorsToTexture();
    } break;
    case 10: FragColor = vec4(inF.red, inF.green, inF.blue, inF.alpha); break;    //  ZC_GUI_ColorManipulator (result_triangle), haven't texture, only rgba
    case 11: FragColor = vec4(inF.red, inF.green, inF.blue, 1.f); break;    //  ZC_GUI_ColorManipulator (saturation_triangle), haven't texture, only rgb
    case 12: FragColor = FragColor = texture(tex_Icon, inF.uv); break;      //  ZC_GUI_ColorManipulator (alpha_triangle), have texture
    default: break;
    }
}

void AddColorsToTexture()
{
    if (FragColor.a == 0) discard;
        //  adds colors to texture color
    FragColor.r += inF.red;
    FragColor.g += inF.green;
    FragColor.b += inF.blue;
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