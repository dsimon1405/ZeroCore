#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouse.h>
#include <ZC/GUI/Text/ZC_GUI_Text.h>

#include <ZC/GUI/ZC_GUI_IconUV.h>

struct ZC_GUI_ButtonMouseText : public ZC_GUI_ButtonMouse
{
    ZC_GUI_ButtonMouseText(float width, float height, ZC_GUI_MB__Flags _mb_flags, ZC_GUI_Text&& text)
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData{ .width = width, .height = height, .uv = ZC_GUI_IconUV::button, .tex_binding = ZC_GUI_Bindings::bind_tex_Icons }),
        ZC_GUI_ButtonMouse(width, height, _mb_flags),
        textImmut(std::move(text))
    {
        if (width < textImmut.GetWidth()) this->pObjData->width = textImmut.GetWidth();
        if (height < textImmut.GetHeight()) this->SetHeight_Obj(textImmut.GetHeight());

        this->VAddObj_Obj(&textImmut, nullptr);
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
    bool UpdateText_BMT(const std::wstring& wstr, bool brootForceUpdate)
    {
        return textImmut.UpdateText(wstr, brootForceUpdate);
    }

    //  Return current wstring.
    const std::wstring& GetText_BMT()
    {
        return textImmut.GetText();
    }

protected:
    void Conf_Set_bl_BMT(const ZC_Vec2<float>& _bl)
    {
        *(this->pBL) = _bl;

        ZC_Vec2<float> bl_texImmut(_bl[0], _bl[1] + (this->GetHeight() - textImmut.GetHeight()) / 2.f);     //  make text in center by Y
        textImmut.VSet_pBL_Obj(bl_texImmut);
    }

private:
    ZC_GUI_Text textImmut;

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
    {
        Conf_Set_bl_BMT(_bl);
    }
};
