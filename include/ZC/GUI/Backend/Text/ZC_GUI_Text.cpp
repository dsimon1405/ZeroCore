#include "ZC_GUI_Text.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/Tools/Math/ZC_Math.h>

#include <cmath>

ZC_GUI_Text::ZC_GUI_Text(const std::wstring& wstr, bool _isImmutable, int reserveWidth, ZC_GUI_TextAlignment textAlignment, unsigned int color)
    : ZC_GUI_Obj(ZC_GUI_ObjData(0.f, 0.f, color, {}, ZC_GUI_Bindings::bind_tex_Text)),
    isImmutable(_isImmutable),
    pText(ZC_GUI_TextManager::GetText(wstr, isImmutable, reserveWidth, textAlignment)),
    actual_width(pText ? pText->width : 0)
{
    if (!pText) return;
    VSetWidth_Obj(pText->width);
    this->SetHeight_Obj(pText->GetHeight());
}

ZC_GUI_Text::ZC_GUI_Text(ZC_GUI_Text&& t)
    : ZC_GUI_Obj(static_cast<ZC_GUI_Obj&&>(t)),
    isImmutable(t.isImmutable),
    pText(t.pText),
    actual_width(t.actual_width)
{
    t.pText = nullptr;
}

ZC_GUI_Text::~ZC_GUI_Text()
{
    if (pText) ZC_GUI_TextManager::EraseText(pText);
}

bool ZC_GUI_Text::UpdateText(const std::wstring& wstr, bool brootForceUpdate)
{
    if (isImmutable) return false;
    auto pText_temp = this->pText;
    if (!ZC_GUI_TextManager::UpdateText(pText, this->VGetWidth_Obj(), brootForceUpdate, wstr)) return false;     //  can't update text
    if (pText_temp != this->pText)  //  UpdateText() was not map existing texture, but set new text in pText. So need update uv
    {
        this->VSetWidth_Obj(pText->width);
        this->pObjData->uv = pText->uv;
        this->VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), offsetof(ZC_GUI_ObjData, borderIndex), pObjData);   //  offset to border will calculate size in ZC_GUI_ObjData from width to uv include
    }
    return true;
}

bool ZC_GUI_Text::UpdateText(const std::list<ZC_GUI_ChData>& chDatas)
{
    return ZC_GUI_TextManager::UpdateText(pText, chDatas);
}

void ZC_GUI_Text::UpdateText(ZC_GUI_TextManager::Text* _pText)
{
    ZC_GUI_TextManager::EraseText(pText);
    pText = _pText;

    actual_width = pText->width;    //  in that case totaly changes actual_width 
    VSetWidth_Obj(pText->width);
    
    this->pObjData->uv = pText->uv;
    VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), offsetof(ZC_GUI_ObjData, borderIndex), pObjData);   //  offset to border will calculate size in ZC_GUI_ObjData from width to uv include
}

const std::wstring& ZC_GUI_Text::GetWStr()
{
    return pText->wstr;
}

float ZC_GUI_Text::VGetWidth_Obj()
{
    return actual_width;
}

float ZC_GUI_Text::VGetWidthComposite_Obj()
{
    return actual_width;
}

void ZC_GUI_Text::VConf_SetTextUV_Obj()
{
    this->pObjData->uv = pText->uv;
}

void ZC_GUI_Text::VSetWidth_Obj(float width)
{
    width = std::round(width);
    pObjData->width = width < actual_width ? width : actual_width;     //  can't be set width large then actual_width
}

void ZC_GUI_Text::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    *pBL = ZC_Vec2<float>(std::round(_bl[0]), std::round(_bl[1]));
}

float ZC_GUI_Text::VGetTextWidth_T()
{
    return this->VGetWidth_Obj();
}