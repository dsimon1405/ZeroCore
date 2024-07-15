#pragma once

#include <ZC/GUI/ZC_GUI_Obj.h>
#include <ZC/GUI/Text/ZC_GUI_TextManager.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>
#include <ZC/Tools/Math/ZC_Math.h>

//  Text is created once and until the end of the program (cannot be changed)
struct ZC_GUI_TextImmutable : public ZC_GUI_Obj
{
    ZC_GUI_TextImmutable(const std::wstring& wstr)
        : ZC_GUI_TextImmutable(wstr, true, 0)
    {}

    ~ZC_GUI_TextImmutable()
    {
        ZC_GUI_TextManager::EraseText(pText);
    }

    // bool VIsDrawing_Obj() const noexcept override
    // {
    //     return true;
    // }

    void VConf_SetTextUV() override
    {
        this->pObjData->uv = pText->uv;
    }

protected:
    static inline unsigned int text_color = ZC_PackColorUCharToUInt(200, 200, 200);
    
    typename ZC_GUI_TextManager::Text* pText;

    ZC_GUI_TextImmutable(const std::wstring& wstr, bool isImmutable, int reserveWidth)
        : ZC_GUI_Obj(ZC_GUI_ObjData{ .color = text_color, .tex_binding = ZC_GUI_Bindings::bind_tex_Text }),
        pText(ZC_GUI_TextManager::GetText(wstr, isImmutable, reserveWidth))
    {
        if (!pText) return;
        this->pObjData->width = pText->width;
        this->SetHeight_Obj(pText->GetHeight());
    }
};