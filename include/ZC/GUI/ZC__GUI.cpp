#include "ZC__GUI.h"

#include <ZC/GUI/Backend/Text/ZC_GUI_FontLoader.h>
#include <ZC/GUI/Backend/System/ZC_GUI.h>

std::vector<ZC_GUI_Font*> ZC__GUI::LoadFonts(const std::vector<ZC_GUI_FontLoadData>& font_load_data, const std::vector<ZC_GUI_FontLoadDataUser>& font_load_data_user)
{
    return ZC_GUI_FontLoader::LoadFonts(font_load_data, font_load_data_user);
}

const ZC_GUI_Font* ZC__GUI::GetFont(const ZC_GUI_FontSrc& font_src)
{
    return ZC_GUI_FontLoader::GetFont(font_src);
}

void ZC__GUI::SetPathToCustomIconTexture(std::string&& path)
{
    ZC_GUI_DrawManager::SetPathToCustomIconTexture(std::move(path));
}

// void ZC__GUI::SetState(bool active)
// {
//     ZC_GUI::SetState(active);
// }

// bool ZC__GUI::GetState()
// {
//     return ZC_GUI::GetState();
// }