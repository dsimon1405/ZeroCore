#pragma once

#include "ZC_GUI_ButtonMouse.h"
#include <ZC/GUI/Backend/Text/ZC_GUI_TextForButton.h>

struct ZC_GUI_ButtonMouseText : public ZC_GUI_ButtonMouse
{
    ZC_GUI_TextForButton textForButton;

    ZC_GUI_ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& _textForButton, const ColorsButton& _colorsButton = {});
    
    ZC_GUI_ButtonMouseText(float width, float height, ZC_GUI_ButtonFlags _buttonFlags, ZC_GUI_TextForButton&& _textForButton, const ZC_GUI_UV& uv,
        const ColorsButton& _colorsButton = {});

    ZC_GUI_ButtonMouseText(ZC_GUI_ButtonMouseText&& bmt);

    /*
    Update text in wstring. Data updates only if pixel width of new string less or equal current width. Text width don't change in any case.

    Params:
    - wstr - new text.
    - brootForceUpdate - if false -> at first trying to find same text in existing Immutable texts and use it, if can't find, creates new text from wstr.
        If _brrotForceMap is true -> just update data in existing space (it's usefull if created changing number, more profit to update data, then trying to find in existing texts...).

    Return:
    true on success, otherwise false.
    */
    bool UpdateText_BMT(const std::wstring& wstr, bool brootForceUpdate);

        //  Return current wstring.
    const std::wstring& GetWStr_BMT();

    void UpdateText_BMT(ZC_GUI_TextManager::Text* pText);
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
        //  must call heirs wich override VSet_pBL_Obj()
    void Set_pBL_BMT(const ZC_Vec2<float>& _bl);
    float VGetWidthComposite_Obj() override;
    void VConf_SetTextUV_Obj() override;
};
