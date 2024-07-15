#include <ZC/GUI/Text/ZC_GUI_TextInputWindow.h>

#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/GUI/ZC_GUI.h>
#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>
#include <ZC/GUI/ZC_GUI_IconUV.h>

ZC_GUI_TextInputWindow::ZC_GUI_TextInputWindow(float fontHeight)
    : ZC_GUI_WinImmutable(ZC_WOIData{ .width = 0.f, .height = 0.f, .indentX = 0.f, .indentY = 0.f, .indentFlags = ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel }, 0),
    highlight(fontHeight),
    textMut(L"", ZC_GUI_TextManager::pTM->font.GetLongestCharacterLength() * 200),     //  reserve place for 200 longest symbols (may be more if symbols are smaller)
    caret(fontHeight)
{
    this->pObjData->color = ZC_PackColorUCharToUInt(20, 20, 20);
    this->pObjData->depth = 0.f;    //  allways on first plane
    this->SetHeight_Obj(fontHeight);
    
    AddRow(Row({}, { &highlight, &textMut, &caret }));  //  must be correct order: highlight, textMut, caret

    pTIW = this;
}

bool ZC_GUI_TextInputWindow::VIsInputWindow_W() const noexcept
{
    return true;
}