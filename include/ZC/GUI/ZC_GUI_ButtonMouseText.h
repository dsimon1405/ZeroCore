#pragma once

#include <ZC/GUI/ZC_GUI_ButtonMouse.h>
#include <ZC/GUI/Text/ZC_GUI_Text.h>

#include <ZC/GUI/ZC_GUI_IconUV.h>

struct ZC_GUI_ButtonMouseText : public ZC_GUI_ButtonMouse
{
    ZC_GUI_ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text)
        : ZC_GUI_ButtonMouseText(width, height, _buttonFlags, std::move(text), ZC_GUI_IconUV::button)
    {}
    
    ZC_GUI_ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& text, const ZC_GUI_UV& uv)
        : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, uv, ZC_GUI_Bindings::bind_tex_Icons), _buttonFlags),
        ZC_GUI_ButtonMouse(width, height, _buttonFlags),
        textForButton(std::move(text))
    {
        this->VAddObj_Obj(&textForButton, nullptr);
           //  width buttons with can't be less then text width (if text inside the button)
        float textForButton_width = this->objs.front()->VGetWidthComposite_Obj();
        if (textForButton.indent.indentFlag_X != ZC_GUI_TextForButton::Indent::OutOfButton && width < textForButton_width) this->pObjData->width = textForButton_width;
        if (height < textForButton.GetHeight()) this->SetHeight_Obj(textForButton.GetHeight());
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
        return textForButton.UpdateText(wstr, brootForceUpdate);
    }

        //  Return current wstring.
    const std::wstring& GetWStr_BMT()
    {
        return textForButton.GetWStr();
    }

protected:
    void UpdateText_BMT(ZC_GUI_TextManager::Text* pText)
    {
        textForButton.UpdateText(pText);
    }

private:
    ZC_GUI_TextForButton textForButton;

    float VGetWidthComposite_Obj() override
    {
        return textForButton.indent.indentFlag_X == ZC_GUI_TextForButton::Indent::OutOfButton ? this->VGetWidth_Obj() + this->objs.front()->VGetWidthComposite_Obj() : this->VGetWidth_Obj();
    }

    void VConf_SetTextUV_Obj() override
    {
        this->objs.front()->VConf_SetTextUV_Obj();
    }
};
