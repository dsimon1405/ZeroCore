#pragma once

#include <vector>
#include <string>
#include <list>

//  class for storing charecters
struct ZC_GUI_Font
{
    struct Character
    {
        wchar_t character;  //  character in unicode
        int left_offset; //  in a text string, the offset after the previous symbol
        int width;       //  fact width of element (pixels)
        int height;      //  fact height of element (pixels) (not height of data)
        int start_row;    //  when created in ZC_GUI_FontLoader::ElementsRange::FillCharactersParams is the distance from the origin to the top (in pixels), later in ZC_GUI_FontLoader::ElementsRange::FillCharactersData here sets line number from which symbol's data begins in data (to avoid spaces (top/bottom) lines when copying)
        std::vector<unsigned char> data;    //  alpha channel data (in 2d array interpretation: width(columns) = Character::width, height(rows) = total_height from ZC_GUI_FontLoader::CreateFont
        
        bool operator == (wchar_t ch) const noexcept;
    };

    ZC_GUI_Font() = default;
    ZC_GUI_Font(std::vector<Character>&& _characters);

    const Character* GetCharacter(wchar_t ch);
    int GetHeight();
    int GetLongestCharacterLength() const noexcept;

    /*
    Fills concrete quad part part of 2d array, with some add data. Both data and add are in 1d arrays, but interprete as 2d arrays.

    Params:
    - data - destination array (interpreted as a 2d array).
    - rData_index - start index in desination (data) (for eteration uses only first line and it's indexes).
    - data_width - length of the row in desination (data) (length of the 1d array in 2d array).
    - add - source array (interpreted as a 2d array).
    - add_width - length of the row in source (add) (length of the 1d array in 2d array).
    - add_height - count of not empty rows in source (add) (count of 1d arrays in 2d array).
    - add_startRow - index of the first not empty row in source (add).
    */
    void AddSymbolData(std::vector<unsigned char>& data, int& rData_index, int data_width, const Character* pCh);
    
private:
    std::vector<Character> characters;
    int height = 0;
};
typedef typename ZC_GUI_Font::Character ZC_GUI_Character;

    
// unsigned long А = 0x410;
// unsigned char А1 = 'А';
// unsigned long Б = 0x411;
// unsigned char Б1 = 'Б';
// unsigned long В = 0x412;
// unsigned char В1 = 'В';
// unsigned long Г = 0x413;
// unsigned char Г1 = 'Г';
// unsigned long Д = 0x414;
// unsigned char Д1 = 'Д';
// unsigned long Е = 0x415;
// unsigned char Е1 = 'Е';
// unsigned long Ж = 0x416;
// unsigned char Ж1 = 'Ж';
// unsigned long З = 0x417;
// unsigned char З1 = 'З';
// unsigned long И = 0x418;
// unsigned char И1 = 'И';
// unsigned long Й = 0x419;
// unsigned char Й1 = 'Й';
// unsigned long К = 0x41A;
// unsigned char К1 = 'К';
// unsigned long Л = 0x41B;
// unsigned char Л1 = 'Л';
// unsigned long М = 0x41C;
// unsigned char М1 = 'М';
// unsigned long Н = 0x41D;
// unsigned char Н1 = 'Н';
// unsigned long О = 0x41E;
// unsigned char О1 = 'О';
// unsigned long П = 0x41F;
// unsigned char П1 = 'П';
// unsigned long Р = 0x420;
// unsigned char Р1 = 'Р';
// unsigned long С = 0x421;
// unsigned char С1 = 'С';
// unsigned long Т = 0x422;
// unsigned char Т1 = 'Т';
// unsigned long У = 0x423;
// unsigned char У1 = 'У';
// unsigned long Ф = 0x424;
// unsigned char Ф1 = 'Ф';
// unsigned long Х = 0x425;
// unsigned char Х1 = 'Х';
// unsigned long Ц = 0x426;
// unsigned char Ц1 = 'Ц';
// unsigned long Ч = 0x427;
// unsigned char Ч1 = 'Ч';
// unsigned long Ш = 0x428;
// unsigned char Ш1 = 'Ш';
// unsigned long Щ = 0x429;
// unsigned char Щ1 = 'Щ';
// unsigned long Ъ = 0x42A;
// unsigned char Ъ1 = 'Ъ';
// unsigned long Ы = 0x42B;
// unsigned char Ы1 = 'Ы';
// unsigned long Ь = 0x42C;
// unsigned char Ь1 = 'Ь';
// unsigned long Э = 0x42D;
// unsigned char Э1 = 'Э';
// unsigned long Ю = 0x42E;
// unsigned char Ю1 = 'Ю';
// unsigned long Я = 0x42F;
// unsigned char Я1 = 'Я';
// unsigned long а = 0x430;
// unsigned char а1 = 'а';
// unsigned long б = 0x431;
// unsigned char б1 = 'б';
// unsigned long в = 0x432;
// unsigned char в1 = 'в';
// unsigned long г = 0x433;
// unsigned char г1 = 'г';
// unsigned long д = 0x434;
// unsigned char д1 = 'д';
// unsigned long е = 0x435;
// unsigned char е1 = 'е';
// unsigned long ж = 0x436;
// unsigned char ж1 = 'ж';
// unsigned long з = 0x437;
// unsigned char з1 = 'з';
// unsigned long и = 0x438;
// unsigned char и1 = 'и';
// unsigned long й = 0x439;
// unsigned char й1 = 'й';
// unsigned long к = 0x43A;
// unsigned char к1 = 'к';
// unsigned long л = 0x43B;
// unsigned char л1 = 'л';
// unsigned long м = 0x43C;
// unsigned char м1 = 'м';
// unsigned long н = 0x43D;
// unsigned char н1 = 'н';
// unsigned long о = 0x43E;
// unsigned char о1 = 'о';
// unsigned long п = 0x43F;
// unsigned char п1 = 'п';
// unsigned long р = 0x440;
// unsigned char р1 = 'р';
// unsigned long с = 0x441;
// unsigned char с1 = 'с';
// unsigned long т = 0x442;
// unsigned char т1 = 'т';
// unsigned long у = 0x443;
// unsigned char у1 = 'у';
// unsigned long ф = 0x444;
// unsigned char ф1 = 'ф';
// unsigned long х = 0x445;
// unsigned char х1 = 'х';
// unsigned long ц = 0x446;
// unsigned char ц1 = 'ц';
// unsigned long ч = 0x447;
// unsigned char ч1 = 'ч';
// unsigned long ш = 0x448;
// unsigned char ш1 = 'ш';
// unsigned long щ = 0x449;
// unsigned char щ1 = 'щ';
// unsigned long ъ = 0x44A;
// unsigned char ъ1 = 'ъ';
// unsigned long ы = 0x44B;
// unsigned char ы1 = 'ы';
// unsigned long ь = 0x44C;
// unsigned char ь1 = 'ь';
// unsigned long э = 0x44D;
// unsigned char э1 = 'э';
// unsigned long ю = 0x44E;
// unsigned char ю1 = 'ю';
// unsigned long я = 0x44F;
// unsigned char я1 = 'я';


// unsigned long Ё = 0x401;
// unsigned char Ё1 = 'Ё';
// unsigned long ё = 0x451;
// unsigned char ё1 = 'ё';
