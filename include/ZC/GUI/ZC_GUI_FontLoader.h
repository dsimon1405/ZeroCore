#pragma once

#include <ZC/GUI/ZC_GUI_Font.h>

#include <forward_list>
#include <string>

enum ZC_GUI_FontElement
{
    ZC_GUI_FE_Symbols     = 1,
    ZC_GUI_FE_English     = 1 << 1,
    ZC_GUI_FE_Russian     = 1 << 2
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
    struct ElementsRange
    {
        struct Range
        {
            unsigned long start_index;      //  first index (unicode)
            unsigned long end_index;        //  last index (unicode)
        };
        std::vector<Range> ranges;

        unsigned long GetCount();
        void FillCharactersParams(void* ft_face, std::vector<ZC_GUI_Character>& rChrs, int& rMax_top, int& rMax_tail);
        void FillCharactersData(void* ft_face, std::vector<ZC_GUI_Character>& rChrs, size_t& rChrsIndex, int& max_top, int max_tail, int total_height);
    };

    static ZC_GUI_Font CreateFont(void* ft_face, std::forward_list<ElementsRange>&& el_ranges);
    static std::forward_list<ElementsRange> GetCategories(ZC_GUI_FontElements elements);
};