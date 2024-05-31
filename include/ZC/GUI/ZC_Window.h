#pragma once

#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/Tools/ZC_WindowOrthoIndent.h>
#include <ZC/Video/OpenGL/Shader/ZC_Pipeline.h>

struct Struct
{
    ZC_Pipeline* pipeline;
    
};

struct ZC_Window : public ZC_WindowOrthoIndent
{
    ZC_Window(float _width, float _height, float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags)
        : ZC_WindowOrthoIndent(false, _width, _height, _indentX, _indentY, _indentFlags)
    {}

    static inline ZC_Texture texBackground;
};