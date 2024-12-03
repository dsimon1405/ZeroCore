#pragma once

#include <ZC/GUI/Backend/Text/ZC_GUI_Font.h>
#include "ZC_GUI_FontElement.h"

#include <forward_list>
#include <string>

class ZC_GUI_FontLoader
{
public:
        //  load fonts from ZC_GUI_FontName or users, or both
    static std::vector<ZC_GUI_Font*> LoadFonts(const std::vector<ZC_GUI_FontLoadData>& font_load_data, const std::vector<ZC_GUI_FontLoadDataUser>& font_load_data_user);

    static const ZC_GUI_Font* GetFont(const ZC_GUI_FontSrc& font_src);

private:
        //  range of the symbols in unicode
    struct ElementsRange
    {
        struct UnicodeRange
        {
            unsigned long start_index;      //  first index (unicode)
            unsigned long end_index;        //  last index (unicode)
        };
        std::vector<UnicodeRange> ranges;

        unsigned long GetCount();
        void FillCharactersParams(void* ft_face, std::vector<ZC_GUI_Character>& rChrs, int& rMax_top, int& rMax_tail, int& rMin_left_offset);
        void FillCharactersData(void* ft_face, std::vector<ZC_GUI_Character>& rChrs, size_t& rChrsIndex, int& max_top, int max_tail, int total_height, int min_left_offset);
    };

    static inline std::list<ZC_GUI_Font> fonts;

    static std::string GetPath(ZC_GUI_FontLoadData::ZC_GUI_FontName name);
    static ZC_GUI_Font CreateFont(i_zc font_src, void* ft_face, std::forward_list<ElementsRange>&& el_ranges);
    static std::forward_list<ElementsRange> GetCategories(ZC_GUI_FontElements elements);
};
