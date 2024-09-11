#include <ZC/GUI/Backend/Text/ZC_GUI_Font.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

ZC_GUI_Font::ZC_GUI_Font(std::vector<Character>&& _characters)
    : characters(std::move(_characters)),
    height(characters.empty() ? 0 : characters.back().data.size() / characters.back().width)   //  width individual for character but all data have same height, so get it
{}

const typename ZC_GUI_Font::Character* ZC_GUI_Font::GetCharacter(wchar_t ch)
{
    return ZC_Find(characters, ch);
}

int ZC_GUI_Font::GetHeight()
{
    return height;
}

int ZC_GUI_Font::GetLongestCharacterLength() const noexcept
{
    static int longestLength = 0;
    if (longestLength == 0)
    {
        for (auto& ch : characters)
        {
            int length = ch.width + ch.left_offset;
            if (length > longestLength) longestLength = length;
        }
    }
    return longestLength;
}

void ZC_GUI_Font::AddSymbolData(std::vector<unsigned char>& data, int& rData_index, int data_width, const Character* pCh)
{
    int data_i = rData_index + (pCh->start_row * data_width);     //  start index in data vector, miss white space rows in add vector
    int add_i = pCh->start_row * pCh->width;   //  start index in add vector, miss space rows
    int height = pCh->height;
    for (; height > 0; data_i += data_width, --height)     //  data_i += data_width - get same position in the next row. Looping while have height of add (in 2d array)
        for (int add_line_width = 0; add_line_width < pCh->width; ++add_line_width)   //  fill rows (same start pos for each row in 2d array)
            data[data_i + add_line_width] = pCh->data[add_i++];

    rData_index += pCh->width;
}



//  ZC_GUI_Font::Character
        
bool ZC_GUI_Font::Character::operator == (wchar_t ch) const noexcept
{
    return character == ch;
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