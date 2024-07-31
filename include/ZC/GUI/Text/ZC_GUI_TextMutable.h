// #pragma once

// #include <ZC/GUI/Text/ZC_GUI_Text.h>

// //  Text can be change
// struct ZC_GUI_TextMutable : public ZC_GUI_Text
// {
//     /*
//     Params:
//     - wstr - wstring of text.
//     - reserveWidth - width that must be reserved in pixels. To avoid creation of new object, may be reserved total width and use UpdateText() to change text.
//     */
//     ZC_GUI_TextMutable(const std::wstring& wstr, int reserveWidth = 0)
//         : ZC_GUI_Text(wstr, false, reserveWidth)
//     {}

//     /*
//     Update text in wstring. Data updates only if pixel width of new string less or equal current width. Text width don't change in any case.

//     Params:
//     - wstr - new text.
//     - brootForceUpdate - if false -> at first trying to find same text in existing Immutable texts and use it, if can't find, creates new text from wstr.
//         If _brrotForceMap is true -> just update data in existing space (it's usefull if created changing number, more profit to update data, then trying to find in existing texts...).

//     Return:
//     true on success, otherwise false
//     */ 
//     bool UpdateText(const std::wstring& wstr, bool brootForceUpdate)
//     {
//         auto pText_temp = this->pText;
//         if (!ZC_GUI_TextManager::UpdateText(pText, this->VGetWidth_Obj(), brootForceUpdate, wstr)) return false;     //  can't update text
//         if (pText_temp != this->pText)  //  UpdateText() was not map existing texture, but set new text in pText. So need update uv
//         {
//             this->pObjData->uv = pText->uv;
//             VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, uv), sizeof(ZC_GUI_ObjData::uv), &(pObjData->uv));
//         }
//         return true;
//     }

//     bool UpdateText(const std::list<ZC_GUI_ChData>& chDatas)
//     {
//         return ZC_GUI_TextManager::UpdateText(pText, chDatas);
//     }
// };