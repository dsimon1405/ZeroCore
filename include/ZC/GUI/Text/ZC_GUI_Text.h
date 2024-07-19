#pragma once

#include <ZC/GUI/ZC_GUI_Obj.h>
#include <ZC/GUI/Text/ZC_GUI_TextManager.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>
#include <ZC/Tools/Math/ZC_Math.h>

//  Text is created once and until the end of the program (cannot be changed)
struct ZC_GUI_Text : public ZC_GUI_Obj
{
    /*
    Createt text string.

    Params:
    - wstr - string in unicode.
    - _isImmutable - if true can't be used method UpdateText().
    - reserveWidth - may be reserve pixel width longer then in wstr now (have effect only if _isImmutable = false). More info in method UpdateText().
    */
    ZC_GUI_Text(const std::wstring& wstr, bool _isImmutable, int reserveWidth)
        : ZC_GUI_Obj(ZC_GUI_ObjData{ .color = text_color, .tex_binding = ZC_GUI_Bindings::bind_tex_Text }),
        isImmutable(_isImmutable),
        pText(ZC_GUI_TextManager::GetText(wstr, isImmutable, reserveWidth))
    {
        if (!pText) return;
        this->pObjData->width = pText->width;
        this->SetHeight_Obj(pText->GetHeight());
    }

    ZC_GUI_Text(ZC_GUI_Text&& t)
        : ZC_GUI_Obj(dynamic_cast<ZC_GUI_Obj&&>(t)),
        isImmutable(t.isImmutable),
        pText(t.pText)
    {
        t.pText = nullptr;
    }

    ~ZC_GUI_Text()
    {
        if (pText) ZC_GUI_TextManager::EraseText(pText);
    }

    /*
    Update text in wstring. Data updates only if pixel width of new string less or equal current width. Text width don't change in any case.

    Params:
    - wstr - new text.
    - brootForceUpdate - if false -> at first trying to find same text in existing Immutable texts and use it, if can't find, creates new text from wstr.
        If _brrotForceMap is true -> just update data in existing space (it's usefull if created changing number, more profit to update data, then trying to find in existing texts...).

    Return:
    true on success, otherwise false
    */
    bool UpdateText(const std::wstring& wstr, bool brootForceUpdate)
    {
        if (isImmutable) return false;
        auto pText_temp = this->pText;
        if (!ZC_GUI_TextManager::UpdateText(pText, this->GetWidth(), brootForceUpdate, wstr)) return false;     //  can't update text
        if (pText_temp != this->pText)  //  UpdateText() was not map existing texture, but set new text in pText. So need update uv
        {
            this->pObjData->uv = pText->uv;
            VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, uv), sizeof(ZC_GUI_ObjData::uv), &(pObjData->uv));
        }
        return true;
    }

    bool UpdateText(const std::list<ZC_GUI_ChData>& chDatas)
    {
        return ZC_GUI_TextManager::UpdateText(pText, chDatas);
    }

    const std::wstring& GetText()
    {
        return pText->wstr;
    }

protected:
    static inline unsigned int text_color = ZC_PackColorUCharToUInt(200, 200, 200);
    
    bool isImmutable;
    typename ZC_GUI_TextManager::Text* pText;

    void VConf_SetTextUV_Obj() override
    {
        this->pObjData->uv = pText->uv;
    }
};