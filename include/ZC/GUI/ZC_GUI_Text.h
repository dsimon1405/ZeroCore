#pragma once

#include <ZC/GUI/ZC_GUI_Obj.h>
#include <ZC/GUI/ZC_GUI_TextManager.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>
#include <ZC/Tools/Math/ZC_Math.h>

struct ZC_GUI_Text : public ZC_GUI_Obj
{
    static inline unsigned int text_color = ZC_PackColorUCharToUInt(200, 200, 200);
    typename ZC_GUI_TextManager::Text* pText;

    ZC_GUI_Text(const std::wstring& wstr)
        : ZC_GUI_Obj(ZC_GUI_ObjData{ .color = text_color, .tex_binding = ZC_GUI_Bindings::bind_tex_Text }),
        pText(ZC_GUI_TextManager::GetText(wstr))
    {
        if (!pText) return;
        this->pObjData->width = pText->width;
        this->pObjData->height = pText->GetHeight();
    }

    void VConf_SetTextUV() override
    {
        this->pObjData->uv = pText->uv;
    }

    bool VIsDrawing_Obj() const noexcept override
    {
        return true;
    }
};