#pragma once

#include "ZC_Font.h"

#include <forward_list>

/*
Class for loading and getting loaded fonts.
For fonts using into the scene better use height in pixel (of loading texture) not less then 150-200 pixels (ZC_FontNameHeight::pixelHeight)
*/
class ZC_Fonts
{
public:
    /*
    Load fonts.

    Params:
    - pNames - is a pointer to the beginning of the font array to load.
    - namesCount - size of pNames array.
    */
    static void Load(ZC_FontData* pNames, ulong namesCount);
    static ZC_Font* GetFont(const ZC_FontData& name);

private:
    struct FontData
    {
        ZC_FontData fontData;
        ZC_Font font;

        bool operator == (const ZC_FontData& fontData) const noexcept;
    };

    static inline std::forward_list<FontData> fonts;
    static const uint texMaxW = 1024,
        pixelPadding = 2;
    static const ulong firstASCII = 32,
        lastASCII = 127;

    static std::string GetPath(ZC_FontName name);
    static ZC_Font MakeFont(void* ft_face);
    static void CalculateTextureSize(uint& texW, uint& texH, void* ft_face);
};