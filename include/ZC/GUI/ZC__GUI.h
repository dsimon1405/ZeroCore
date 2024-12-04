#pragma once

#include <ZC/GUI/Backend/Text/ZC_GUI_FontElement.h>
#include <ZC/GUI/Backend/Text/ZC_GUI_Font.h>

#include <string>

namespace ZC__GUI
{
    /*
    Loads fonts that could be used in the gui. In both of structures ZC_GUI_FontLoadData, ZC_GUI_FontLoadDataUser pay attention to pixels_height,
    system could change it after loading font on the factical height of the loaded font. And to use ZC__GUI::GetFont(ZC_GUI_FontSrc{}) need use factical height.

    Params:
    - font_load_data - fonts fron the library see struct ZC_GUI_FontLoadData. If don't need use {} (vector's empty list-intialization).
    - font_load_data_user - users fonts, see struct ZC_GUI_FontLoadDataUser. If don't need use {} (vector's empty list-intialization).

    Retrun:
    Vector of loaded fonts at first goes fonts from font_load_data, then from font_load_data_user. If load of some font is fail on it's plays will be nullptr. Pay atention that loaded
    ZC_GUI_Font::font_src::pixels_height seted to factical and if need to get that font with ZC__GUI::GetFont(ZC_GUI_FontSrc{}), must be used factical height!
    */
    std::vector<ZC_GUI_Font*> LoadFonts(const std::vector<ZC_GUI_FontLoadData>& font_load_data, const std::vector<ZC_GUI_FontLoadDataUser>& font_load_data_user);
    
        //  Returrns font if it was loaded, otherwise nullptr. Pay attention that font_src::pixels_height must be factical! See ZC__GUI::LoadFonts for more info.
    const ZC_GUI_Font* GetFont(const ZC_GUI_FontSrc& font_src);

        //  Set path to user icon texture. Must be called before ZC_SWindow creation. Texture coords must be defined user.
    void SetPathToCustomIconTexture(std::string&& path);

    //     //  GUI ADDED TO RENDER, TO STOP RENDERING NEED TO ADD SOME MECHANIZM IN RENDER SYSTEM, MAY BE LATER...
    // //     //  Set gui state. If active true gui is drawing and processing events, otherwise not.
    // // void SetState(bool active);
    // //     //  Return gui state. If true active (srawing and process events), otherwise not.
    // // bool GetState();
}