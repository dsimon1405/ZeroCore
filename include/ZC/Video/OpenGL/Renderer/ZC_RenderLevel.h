#pragma once

typedef int ZC_RenderLevel;
enum ZC_RenderLevels
{
    ZC_RL_Default = 1000,
};


enum ZC_DrawerLevels
{
    None = -1,
    Drawing = 100,    //  ZC_Drawer.cpp
    StencilBorder = 200,        //  ZC_DSStencilBorder.cpp
    TextScene = Drawing + 1000,     //  ZC_Drawer.cpp
    TextWindowIntoScene = TextScene + 1000,     //  ZC_Drawer.cpp
    OrthoBlend = TextWindowIntoScene + 1000,    //  ZC_Drawer.cpp
};

typedef int ZC_DrawerLevel;   //  Draw Levels for user
#define ZC_DL_None ZC_DrawerLevels::None
#define ZC_DL_Drawing ZC_DrawerLevels::Drawing
#define ZC_DL_StencilBorder ZC_DrawerLevels::StencilBorder