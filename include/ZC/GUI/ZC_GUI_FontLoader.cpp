#include <ZC/GUI/ZC_GUI_FontLoader.h>

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/File/ZC_File.h>
#include <ZC/Tools/Math/ZC_Math.h>

#include <ft2build.h>
#include FT_FREETYPE_H

ZC_GUI_Font ZC_GUI_FontLoader::LoadFont(FontName font_name, ulong pix_height, ZC_GUI_FontElements elements)
{
    return LoadFont(GetPath(font_name).c_str(), pix_height, elements);
}

ZC_GUI_Font ZC_GUI_FontLoader::LoadFont(const char* path, ulong pix_height, ZC_GUI_FontElements elements)
{
    if (elements == 0) return {};  //  elements not requested

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) 
    {
        ZC_ErrorLogger::Err("Fail FT_Init_FreeType()!", __FILE__, __LINE__);
        FT_Done_FreeType(ft);
        return {};
    }
    FT_Face ft_face;
    if (FT_New_Face(ft, path, 0, &ft_face))
    {
        ZC_ErrorLogger::Err("Fail FT_New_Face()!", __FILE__, __LINE__);
        FT_Done_Face(ft_face);
        FT_Done_FreeType(ft);
        return {};
    }
    if (FT_Set_Pixel_Sizes(ft_face, 0, pix_height))
    {
        ZC_ErrorLogger::Err("Fail FT_Set_Pixel_Sizes()!", __FILE__, __LINE__);
        FT_Done_Face(ft_face);
        FT_Done_FreeType(ft);
        return {};
    }
    ZC_GUI_Font font = CreateFont(ft_face, GetCategories(elements));

    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft);
    return font;
}

std::string ZC_GUI_FontLoader::GetPath(FontName name)
{
    static const ZC_FSPath ZC_fontsPath = ZC_FSPath(ZC_ZCDirPath).append("fonts");
    switch (name)
    {
    case Arial: return ZC_FSPath(ZC_fontsPath).append("arial.ttf").string();
    case ChunkFivePrint: return ZC_FSPath(ZC_fontsPath).append("chunkFivePrint.otf").string();
    default: return "";
    }
}

ZC_GUI_Font ZC_GUI_FontLoader::CreateFont(void* ft_face, std::forward_list<ElementsRange>&& el_ranges)
{
    std::vector<ZC_GUI_Character> chrs;
    ulong chrs_count = 0;
    for (ElementsRange& el_r : el_ranges) chrs_count += el_r.GetCount();
    chrs.reserve(chrs_count);

    int max_top = 0;    //  highest top (pixel)
    int max_tail = 0;   //  lowest tail (pixel)
    int min_left_offset = ZC_INT_MAX;
    for (ElementsRange& el_r : el_ranges) el_r.FillCharactersParams(ft_face, chrs, max_top, max_tail, min_left_offset);

    int total_height = max_top + max_tail;    //  heigth for each symbol (pixel)
    size_t chrsIndex = 0;   //  index for iteretion in chrs
    for (ElementsRange& el_r : el_ranges) el_r.FillCharactersData(ft_face, chrs, chrsIndex, max_top, max_tail, total_height, min_left_offset);

    return { std::move(chrs) };
}

std::forward_list<typename ZC_GUI_FontLoader::ElementsRange> ZC_GUI_FontLoader::GetCategories(ZC_GUI_FontElements elements)
{
    std::forward_list<ElementsRange> ctgs;
    if (elements & ZC_GUI_FE_Symbols)
    {
        ctgs.emplace_front(ElementsRange{.ranges
            {
                { .start_index = 32, .end_index = 64 },
                { .start_index = 91, .end_index = 96 },
                { .start_index = 123, .end_index = 126 }
            }});
    }
    if (elements & ZC_GUI_FE_English)
    {
        ctgs.emplace_front(ElementsRange{.ranges
            {
                { .start_index = 65, .end_index = 90 },
                { .start_index = 97, .end_index = 122 }
            }});
    }
    if (elements & ZC_GUI_FE_Russian)
    {
        ctgs.emplace_front(ElementsRange{.ranges
            {
                { .start_index = 0x410, .end_index = 0x44F },   //  alphabet
                { .start_index = 0x401, .end_index = 0x401 },   //  Ё
                { .start_index = 0x451, .end_index = 0x451 }    //  ё
            }});
    }
    return ctgs;
}


//  ZC_GUI_FontLoader::ElementsRange

unsigned long ZC_GUI_FontLoader::ElementsRange::GetCount()
{
    unsigned long count = 0;
    for (Range& range : ranges) count += range.end_index - range.start_index + 1;     //  + 1 caurse index difference
    return count;
}

void ZC_GUI_FontLoader::ElementsRange::FillCharactersParams(void* ft_face, std::vector<ZC_GUI_Character>& rChrs, int& rMax_top, int& rMax_tail, int& rMin_left_offset)
{
    FT_Face face = static_cast<FT_Face>(ft_face);
    auto lambFillChars = [face, &rChrs, &rMax_top, &rMax_tail, &rMin_left_offset](ulong start_index, ulong end)
    {
        //  end = end_index + 1
        for (ulong symbol = start_index; symbol < end; ++symbol)   //  cycle for getting max_top and max_tail
        {
            if (FT_Load_Char(face, symbol, FT_LOAD_RENDER))
            {
                ZC_ErrorLogger::Err("Fail FT_Load_Char()!", __FILE__, __LINE__);
                continue;
            }
            ZC_GUI_Character& curChar = rChrs.emplace_back(ZC_GUI_Character
            {
                .symbol = static_cast<wchar_t>(symbol),
                .left_offset = face->glyph->bitmap_left >= 0 ? face->glyph->bitmap_left : - face->glyph->bitmap_left,
                .width =  static_cast<int>(symbol == ulong(' ') ? face->glyph->advance.x >> 6 : face->glyph->bitmap.width),     //  white space haven't bitmap, so take its distance from origin to next symbol 
                .height = static_cast<int>(face->glyph->bitmap.rows),
                .start_row = face->glyph->bitmap_top      //  set top (distance from origin to top) (pixels)
            });
            if (curChar.left_offset != 0 && curChar.left_offset < rMin_left_offset) rMin_left_offset = curChar.left_offset; //  don't want have left_offset = 0, so those who have 0, will get minimum leeft_offset in FillCharactersData()
            if (curChar.start_row > 0 && curChar.start_row > rMax_top) rMax_top = curChar.start_row;   //  set highest top
            if (curChar.height > curChar.start_row)
            {
                int tail = curChar.start_row > 0 ? curChar.height - curChar.start_row     //  start upper origin
                    : - curChar.start_row + curChar.height;   //  start under origin
                if (tail > rMax_tail) rMax_tail = tail;
            }
        }
    };

    for (Range& range : ranges) lambFillChars(range.start_index, range.end_index + 1);  //  + 1 for more easy looping in for(;;)
}

void ZC_GUI_FontLoader::ElementsRange::FillCharactersData(void* ft_face, std::vector<ZC_GUI_Character>& rChrs, size_t& rChrsIndex,
    int& max_top, int max_tail, int total_height, int min_left_offset)
{
    FT_Face face = static_cast<FT_Face>(ft_face);

    auto lambFillData = [face, &rChrs, &rChrsIndex, max_top, max_tail, total_height, min_left_offset](ulong startIndex, ulong end)
    {
        //  end = last_index + 1
        for (ulong symbol = startIndex; symbol < end; ++symbol)   //  cycle for filling chararacters data
        {
            if (symbol == ulong(' '))   //  don't fill data for space
            {
                ++rChrsIndex;
                continue;
            }
            if (FT_Load_Char(face, symbol, FT_LOAD_RENDER)) continue;
            ZC_GUI_Character& ch = rChrs[rChrsIndex++];
            if (ch.left_offset == 0) ch.left_offset = min_left_offset;  //  avoid zero left offset
            ch.data = std::vector<unsigned char>(total_height * ch.width);     //  on gpu it's be 2d array, on cpu is 1d array (that alpha channel (not color))
            //  freetype stores data origin in tl corner, in openGL textures origin in bl corner, so the rows need to be take in the reverse order (the data in the rows must keep the sequence!)
            int ch_startRevRow = ch.start_row > 0 ? total_height - (max_top - ch.start_row) - 1     //  start row (takes from the end) to fill in the ch data 
                : max_tail + ch.start_row;  //  don't need -1 
            int ch_RowIndex = ch_startRevRow * ch.width;     //  start inidex (pos) of ch_startRevRow in 1d array
            for (int ft_index = 0, height = 0; height < ch.height; ch_RowIndex -= ch.width, ++height)  //  take rows in reverse order (from freetype takes data in right order), while not reach ch's height
                for (int i = 0; i < ch.width; ++i)   //  fill columns in row (order same as in freetype)
                    ch.data[ch_RowIndex + i] = face->glyph->bitmap.buffer[ft_index++];
            //  in freetype start at bl corner (top of the symbol), but we need now start from the bottom of the symbol (openGL start draw from bl) (to avoid in future copying empty lines from top and bottom)
            ch.start_row = ch.start_row > 0 ? total_height - ((max_top - ch.start_row) + ch.height)   //  (+) calculate from the top
                : max_tail + ch.start_row - 1;    //  (-) calculate from the tail
        }
    };
    
    for (Range& range : ranges) lambFillData(range.start_index, range.end_index + 1);  //  + 1 for more easy looping in for(;;)
}