#pragma once

#include <ZC/GUI/Backend/ZC_GUI_Obj.h>
#include <ZC/GUI/Backend/System/ZC_GUI_TextManager.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Colors.h>

//  Text is created once and until the end of the program (cannot be changed)
struct ZC_GUI_Text : public ZC_GUI_Obj
{
    /*
    Createt text string.

    Params:
    - wstr - string in unicode.
    - _isImmutable - if true can't be used method UpdateText().
    - reserveWidth - may be reserve pixel width longer then in wstr (have effect only if _isImmutable = false). More info in method UpdateText().
    - textAlignment - if reservedWidth more then wstr pixel wisth, current wstr can have horizontal alignment in texture. In other case it's Left.
    - color - text's color.
    */
    ZC_GUI_Text(const std::wstring& wstr, bool _isImmutable, int reserveWidth, ZC_GUI_TextAlignment textAlignment, unsigned int color = ZC_GUI_Colors::window_text);

    ZC_GUI_Text(ZC_GUI_Text&& t);

    ~ZC_GUI_Text();

    /*
    Update text in wstring. Data updates only if pixel width of new string less or equal current width. Text width don't change in any case.

    Params:
    - wstr - new text.
    - brootForceUpdate - if false -> at first trying to find same text in existing Immutable texts and use it, if can't find, creates new text from wstr.
        If _brrotForceMap is true -> just update data in existing space (it's usefull if created changing number, more profit to update data, then trying to find in existing texts...).

    Return:
    true on success, otherwise false
    */
    bool UpdateText(const std::wstring& wstr, bool brootForceUpdate);
    bool UpdateText(const std::list<ZC_GUI_ChData>& chDatas);
        //  calls from ZC_GUI_BranchSimple::UpdateName()
    void UpdateText(ZC_GUI_TextManager::Text* _pText);
    const std::wstring& GetWStr();
    float VGetWidth_Obj() override;
    float VGetWidthComposite_Obj() override;
    void VConf_SetTextUV_Obj() override;
    void VSetWidth_Obj(float width) override;
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;

protected:
    bool isImmutable;
    typename ZC_GUI_TextManager::Text* pText;
    float actual_width;     //  mutable texture can get Texts with different texture width. Object's width must be updated with Text width (to have don't wraped texture size). So actual_width is object's full width. this->pObjData->width can have less or equal actual_width from new Text.

};