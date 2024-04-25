#pragma once

typedef int ZC_FrameBuffer;
enum ZC_FrameBuffers
{
    ZC_FB_Default = 1000,
};


enum ZC_DrawLevels
{
    None = -1,
    Drawing = 0,    //  ZC_RLDDrawing.h     ZC_RLDDrawing
    StencilBorder = 1,        //  ZC_RLDStencilBorder.h     ZC_RLDStencilBorder
    TextScene = Drawing + 1000,     //  ZC_RLDText.h    ZC_RLDTextScene
    TextWindowIntoScene = TextScene + 1000,     //  ZC_RLDText.h    ZC_RLDTextWindowIntoScene
    OrthoBlend = TextWindowIntoScene + 1000,    //  ZC_RLDText.h    ZC_RLDText
};

typedef int ZC_DrawLevel;   //  Draw Levels for user
#define ZC_DL_None ZC_DrawLevels::None
#define ZC_DL_Drawing ZC_DrawLevels::Drawing
#define ZC_DL_StencilBorder ZC_DrawLevels::StencilBorder