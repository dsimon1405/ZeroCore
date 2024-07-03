#pragma once

#include <ZC/Tools/Math/ZC_Vec2.h>

struct ZC_GUI_UV
{
    ZC_Vec2<float> bl;
    ZC_Vec2<float> tr;
};

struct ZC_GUI_Border
{
    ZC_Vec2<float> bl;     //  coord of frame border, to avoid drawing objects out of window
    ZC_Vec2<float> tr;

    bool IsDrawing() const noexcept;
    bool CursorCollision(float x, float y);
};

struct ZC_GUI_ObjData //  gui.vs -> struct Data
{
    float width = 0.f;
    float height = 0.f;
    float depth = 0.f;
    unsigned int color = 0; //  must be after depth! (look ZC_GUI_Window::SetFocuseDepthAndColor()) color will be added to texture color, default 0, must be pack with ZC_PackColorUCharToUInt() or ZC_PackColorFloatToUInt() from ZC_Math.h
    ZC_GUI_UV uv;
    int borderIndex = 0;
    unsigned int tex_binding = 0;
};