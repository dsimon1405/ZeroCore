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
    - reserveWidth - may be reserve pixel width longer then in wstr (have effect only if _isImmutable = false). More info in method UpdateText().
    - textAlignment - if reservedWidth more then wstr pixel wisth, current wstr can have horizontal alignment in texture. In other case it's Left.
    */
    ZC_GUI_Text(const std::wstring& wstr, bool _isImmutable, int reserveWidth, ZC_GUI_TextAlignment textAlignment)
        : ZC_GUI_Obj(ZC_GUI_ObjData(0.f, 0.f, text_color, {}, ZC_GUI_Bindings::bind_tex_Text)),
        isImmutable(_isImmutable),
        pText(ZC_GUI_TextManager::GetText(wstr, isImmutable, reserveWidth, textAlignment)),
        actual_width(pText ? pText->width : 0)
    {
        if (!pText) return;
        VSetWidth_Obj(pText->width);
        this->SetHeight_Obj(pText->GetHeight());
    }

    ZC_GUI_Text(ZC_GUI_Text&& t)
        : ZC_GUI_Obj(dynamic_cast<ZC_GUI_Obj&&>(t)),
        isImmutable(t.isImmutable),
        pText(t.pText),
        actual_width(t.actual_width)
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
        if (!ZC_GUI_TextManager::UpdateText(pText, this->VGetWidth_Obj(), brootForceUpdate, wstr)) return false;     //  can't update text
        if (pText_temp != this->pText)  //  UpdateText() was not map existing texture, but set new text in pText. So need update uv
        {
            VSetWidth_Obj(pText->width);
            this->pObjData->uv = pText->uv;
            VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), offsetof(ZC_GUI_ObjData, borderIndex), pObjData);   //  offset to border will calculate size in ZC_GUI_ObjData from width to uv include
        }
        return true;
    }

    bool UpdateText(const std::list<ZC_GUI_ChData>& chDatas)
    {
        return ZC_GUI_TextManager::UpdateText(pText, chDatas);
    }
        //  calls from ZC_GUI_Branch::UpdateName()
    void UpdateText(ZC_GUI_TextManager::Text* _pText)
    {
        ZC_GUI_TextManager::EraseText(pText);
        pText = _pText;

        actual_width = pText->width;    //  in that case totaly changes actual_width 
        VSetWidth_Obj(pText->width);
        
        this->pObjData->uv = pText->uv;
        VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), offsetof(ZC_GUI_ObjData, borderIndex), pObjData);   //  offset to border will calculate size in ZC_GUI_ObjData from width to uv include
    }

    const std::wstring& GetWStr()
    {
        return pText->wstr;
    }

    float VGetWidth_Obj() override
    {
        return actual_width;
    }

protected:
    static inline unsigned int text_color = ZC_PackColorUCharToUInt(200, 200, 200);
    
    bool isImmutable;
    typename ZC_GUI_TextManager::Text* pText;
    float actual_width;     //  mutable textuew can get Texts with different texture width. Object's width must be updated with Text width (to have don't wraped texture size). So actual_width is object width actual width. this->pObjData->width can have less or equal actual_width from new Text.

    void VConf_SetTextUV_Obj() override
    {
        this->pObjData->uv = pText->uv;
    }

    void VSetWidth_Obj(float width) override
    {
        pObjData->width = width < actual_width ? width : actual_width;     //  can't be set width large then actual_width
    }
};




struct ZC_GUI_TextForButton : public ZC_GUI_Text
{
    struct Indent
    {
        enum Indent_X
        {
            Left,       //  indent X calculates from the left border
            Right,      //  indent X calculates from right border
            Center,     //  ignore ZC_GUI_MBTParams::indent_x
            OutOfButton //  out of button (from right side)
        };
        float indent_x = 0.f;       //  indent from buttons left/right border
        Indent_X indentFlag_X = Left;   //  indent specifier for indent_x

        Indent(float _indent_x, Indent_X _indentFlag_X)
            : indent_x(_indent_x < 0.f || _indentFlag_X == Center ? 0.f : _indent_x),
            indentFlag_X(_indentFlag_X)
        {}
    } const indent;

    ZC_GUI_TextForButton(const Indent& _indent, const std::wstring& wstr, bool _isImmutable, int reserveWidth, ZC_GUI_TextAlignment textAlignment)
        : ZC_GUI_Text(wstr, _isImmutable, reserveWidth, textAlignment),
        indent(_indent)
    {}
    
    float VGetWidthComposite_Obj() override
    {
        return indent.indent_x + this->VGetWidth_Obj();
    }

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override
    {
        switch (indent.indentFlag_X)
        {
        case Indent::Left:
        {
            *pBL = ZC_Vec2<float>(_bl[0] + indent.indent_x, _bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f));
        } break;
        case Indent::Right:
        {
            *pBL = ZC_Vec2<float>(_bl[0] + pObjHolder->VGetWidth_Obj() - indent.indent_x - this->VGetWidth_Obj(), _bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f));
        } break;
        case Indent::Center:
        {
            *pBL = ZC_Vec2<float>(_bl[0] + ((pObjHolder->VGetWidth_Obj() - this->VGetWidth_Obj()) / 2.f), _bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f));
        } break;
        case Indent::OutOfButton:
        {
            *pBL = ZC_Vec2<float>(_bl[0] + pObjHolder->VGetWidth_Obj() + indent.indent_x, _bl[1] + ((pObjHolder->GetHeight() - this->GetHeight()) / 2.f));
        } break;
        }
    }
};