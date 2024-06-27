#include <ZC/GUI/ZC_GUI_ObjData.h>

bool ZC_GUI_Border::IsDrawing() const noexcept
{
    return bl[0] != tr[0];    //  if tl.x and tr.x are at the same point, the border is not visible
}

bool ZC_GUI_Border::CursorCollision(float x, float y)
{
    return bl[0] <= x && bl[1] <= y && x <= tr[0] && y <= tr[1];
}