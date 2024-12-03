#include <ZC/GUI/Backend/Text/ZC_GUI_Font.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

ZC_GUI_Font::ZC_GUI_Font(int font_id, std::vector<Character>&& _characters)
    : characters(std::move(_characters)),
    font_src{ .id = font_id, .pixels_height = (characters.empty() ? 0 : i_zc(characters.back().data.size()) / characters.back().width) },   //  width individual for character but all data have same height, so get it
    longest_character_pixels_width(GetLongestCharacterLength()),
    longest_number_pixels_width(GetLongestNumberCharacterWidth())
{}

bool ZC_GUI_Font::operator == (const ZC_GUI_FontSrc& f) const noexcept
{
    return font_src == f;
}

const typename ZC_GUI_Font::Character* ZC_GUI_Font::GetCharacter(wchar_t ch) const
{
    return ZC_Find(characters, ch);
}

int ZC_GUI_Font::GetHeight() const noexcept
{
    return font_src.pixels_height;
}

int ZC_GUI_Font::GetLongestCharacterLength()
{
    int longestLength = 0;
    for (auto& ch : characters)
    {
        int length = ch.width + ch.left_offset;
        if (length > longestLength) longestLength = length;
    }
    return longestLength;
}

int ZC_GUI_Font::GetLongestNumberCharacterWidth()
{
    int longestLength = 0;
    bool found = false;
    for (const Character& ch : characters)
    {
        if (ch.character == L'0') found = true;
        if (found)
        {
            float length = ch.left_offset + ch.width;
            if (length > longestLength) longestLength = length;
            
            if (ch.character == L'9') break;
        }
    }
    return longestLength;
}

int ZC_GUI_Font::CalculateWstrWidth(const std::wstring& wstr) const
{
    int wstr_width = 0;
    for (const wchar_t& wch : wstr)
    {
        const typename ZC_GUI_Font::Character* pCh = GetCharacter(wch);
        if (!pCh) continue;     //  symbol wasn't loaded
        if (&wch != wstr.data()) wstr_width += pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
        wstr_width += pCh->width;
    }
    return wstr_width;
}


    //  ZC_GUI_Font::Character
        
bool ZC_GUI_Font::Character::operator == (wchar_t ch) const noexcept
{
    return character == ch;
}


    //  ZC_GUI_FontSrc

bool ZC_GUI_FontSrc::operator == (const ZC_GUI_FontSrc& f) const noexcept
{
    return f.id == id && f.pixels_height == pixels_height;
}

    
// #include <iostream>
    // if (FT_Load_Char(face, 0x451, FT_LOAD_RENDER))
    // {
    //     int a = 3;
    // }
    // for (size_t i = 0; i < glyph->bitmap.width * glyph->bitmap.rows; i++)
    // {
    //     glyph->bitmap.buffer[i] == 0 ? std::cout<<' ' : std::cout<<glyph->bitmap.buffer[i];
    //     if (i % glyph->bitmap.width == 0) std::cout<<std::endl;
    // }