#include <ZC/GUI/Backend/ZC_GUI_ObjData.h>

#include <cmath>

    //  ZC_GUI_Border

bool ZC_GUI_Border::IsDrawing() const noexcept
{
    return bl[0] != tr[0];    //  if tl.x and tr.x are at the same point, the border is not visible
}

bool ZC_GUI_Border::CursorCollision(float x, float y)
{
    return bl[0] <= x && bl[1] <= y && x <= tr[0] && y <= tr[1];
}

    //  ZC_GUI_ObjData

ZC_GUI_ObjData::ZC_GUI_ObjData(float _width, float _height, unsigned int _color, const ZC_GUI_UV& _uv, unsigned int _tex_binding)
    : ZC_GUI_ObjData(_width, _height, 0, _color, _uv, 0, _tex_binding)
{}

ZC_GUI_ObjData::ZC_GUI_ObjData(float _width, float _height, float _depth, unsigned int _color, const ZC_GUI_UV& _uv, int _borderIndex, unsigned int _tex_binding)
    : width(std::round(_width)),
    height(std::round(_height)),
    depth(_depth),
    color(_color),
    uv(_uv),
    borderIndex(_borderIndex),
    tex_binding(_tex_binding)
{}