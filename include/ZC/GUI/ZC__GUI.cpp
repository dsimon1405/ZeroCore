#include "ZC__GUI.h"

#include <ZC/GUI/Backend/System/ZC_GUI.h>

void ZC__GUI::SetFontPath(const std::string& path)
{
    ZC_GUI_TextManager::font_path = path;
}

void ZC__GUI::SetFontHeight(uint pix_height)
{
    ZC_GUI_TextManager::font_height = pix_height;
}

void ZC__GUI::SetFontLoadingElements(ZC_GUI_FontElements font_elements)
{
    ZC_GUI_TextManager::font_elements = font_elements;
}

int ZC__GUI::GetFontHeight()
{
    return ZC_GUI_TextManager::GetFontHeight();
}

int ZC__GUI::CalculateWstrWidth(const std::wstring& wstr)
{
    return ZC_GUI_TextManager::CalculateWstrWidth(wstr);
}

void ZC__GUI::SetState(bool active)
{
    ZC_GUI::SetState(active);
}
