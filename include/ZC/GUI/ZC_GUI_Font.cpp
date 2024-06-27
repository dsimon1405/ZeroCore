#include <ZC/GUI/ZC_GUI_Font.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

ZC_GUI_Font::ZC_GUI_Font(std::vector<Character>&& _characters)
    : characters(std::move(_characters))
{}

const typename ZC_GUI_Font::Character* ZC_GUI_Font::FindCharacter(wchar_t ch)
{
    return ZC_Find(characters, ch);
}

int ZC_GUI_Font::GetHeight()
{
    return characters.empty() ? 0 : characters.back().data.size() / characters.back().width;  //  width individual for character but all data have same height, so get it
}

std::vector<unsigned char> ZC_GUI_Font::FillWStrData(const std::wstring& wstr, int& rWidth, int& rStart_row, int& rHeight)
{
    std::vector<const Character*> chs;
    chs.reserve(wstr.size());
    int wstr_width = 0;
    for (const wchar_t& wch : wstr)
    {
        const Character* pCh = chs.emplace_back(FindCharacter(wch));
        if (!pCh) continue;     //  symbol wasn't loaded
        wstr_width += pCh->width;
        if (&wch != wstr.data()) wstr_width += pCh->left_offset;   // if this is not the first wch of the wstr, adds left_offset
    }
    int start_offset = 0;
    if (wstr_width < rWidth) //  user want longer width then fact width of the wstr
    {
        start_offset = (rWidth - wstr_width) / 2;   //  wstr will be in the center of the texture, so if wstr_width != width, need to make an offset
        wstr_width = rWidth;
    }
    else rWidth = wstr_width;

    std::vector<unsigned char> wstr_data(wstr_width * GetHeight());    //   interpret as 2d array
    int wstr_data_index = start_offset;
    for (const Character*& pCh : chs)
    {
        if (&pCh != chs.data()) wstr_data_index += pCh->left_offset;   //  if not first element of wstr, adds left_offset
        if (rStart_row > pCh->start_row) rStart_row = pCh->start_row;   //  take the lowest starting row
        if (rHeight < pCh->start_row + pCh->height) rHeight = pCh->start_row + pCh->height;  //  take the greatest height
        FillQuad2D(wstr_data, wstr_data_index, wstr_width, pCh->data, pCh->width, pCh->height, pCh->start_row);
    }
    return wstr_data;
}

void ZC_GUI_Font::FillQuad2D(std::vector<unsigned char>& data, int& rData_index, int data_width,
    const std::vector<unsigned char>& add, int add_width, int add_height, int add_startRow)
{
    int data_i = rData_index + (add_startRow * data_width);     //  start index in data vector, miss space rows in add vector
    int add_i = add_startRow * add_width;   //  start index in add vector, miss space rows
    for (; add_height > 0; data_i += data_width, --add_height)     //  data_i += data_width - get same position in the next row. Looping while have height of add (in 2d array)
        for (int add_line_width = 0; add_line_width < add_width; ++add_line_width)   //  fill rows (same start pos for each row in 2d array)
            data[data_i + add_line_width] = add[add_i++];

    rData_index += add_width;
}



//  ZC_GUI_Font::Character
        
bool ZC_GUI_Font::Character::operator == (wchar_t ch) const noexcept
{
    return symbol == ch;
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