#pragma once

#include <ZC/GUI/Backend/Text/ZC_GUI_Font.h>

#include <forward_list>
#include <string>

enum ZC_GUI_FontElement
{
    ZC_GUI_FE__Symbols     = 1,
    ZC_GUI_FE__English     = 1 << 1,
    ZC_GUI_FE__Russian     = 1 << 2
};
typedef int ZC_GUI_FontElements;

struct ZC_GUI_FontLoader
{
    enum FontName
    {
        Arial,
        ChunkFivePrint
    };

    static ZC_GUI_Font LoadFont(FontName font_name, unsigned long pix_height, ZC_GUI_FontElements elements);
    static ZC_GUI_Font LoadFont(const char* path, unsigned long pix_height, ZC_GUI_FontElements elements);
    static std::string GetPath(FontName name);

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

    static ZC_GUI_Font CreateFont(void* ft_face, std::forward_list<ElementsRange>&& el_ranges);
    static std::forward_list<ElementsRange> GetCategories(ZC_GUI_FontElements elements);
};
