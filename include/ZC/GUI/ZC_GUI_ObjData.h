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
    float width;
    float height;
    float depth;
    ZC_GUI_UV uv;
    int borderIndex = 0;
    unsigned int textureIndex = 0;
};