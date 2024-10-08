#include <ZC/Objects/Text/ZC_Fonts.h>

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/File/ZC_File.h>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

void ZC_Fonts::Load(ZC_FontData* pFontData, ulong fontDataCount)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) ZC_ErrorLogger::Err("Fail FT_Init_FreeType()!", __FILE__, __LINE__);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (ulong namesIndex = 0; namesIndex < fontDataCount; ++namesIndex)
    {
        auto& nameData = pFontData[namesIndex];
        auto pFontData = ZC_Find(fonts, nameData);
        if (pFontData) continue;

        FT_Face face;
        if (FT_New_Face(ft, GetPath(nameData.name).c_str(), 0, &face))
        {
            ZC_ErrorLogger::Err("Fail FT_New_Face()!", __FILE__, __LINE__);
            continue;
        }

        if (FT_Set_Pixel_Sizes(face, 0, nameData.pixelsHeight))
        {
            ZC_ErrorLogger::Err("Fail FT_Set_Pixel_Sizes()!", __FILE__, __LINE__);
            continue;
        }

        fonts.emplace_front(nameData, MakeFont(face));
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    FT_Done_FreeType(ft);
}

ZC_Font* ZC_Fonts::GetFont(const ZC_FontData& fontData)
{
    auto pFontData = ZC_Find(fonts, fontData);
    if (!pFontData) ZC_ErrorLogger::Err("Error in GetFont() Font wasn't loaded!", __FILE__,__LINE__);
    return &(pFontData->font);
}

std::string ZC_Fonts::GetPath(ZC_FontName name)
{
    static const ZC_FSPath ZC_fontsPath = ZC_FSPath(ZC_ZCDirPath).append("fonts");
    switch (name)
    {
    case ZC_F_Arial: return ZC_FSPath(ZC_fontsPath).append("arial.ttf").string();
    case ZC_F_ChunkFivePrint: return ZC_FSPath(ZC_fontsPath).append("chunkFivePrint.otf").string();
    default: return "";
    }
}

ZC_Font ZC_Fonts::MakeFont(void* ft_face)
{
    FT_Face face = static_cast<FT_Face>(ft_face);
    uint texW = 0,
        texH = 0;
    std::vector<UnicodeRange> unicode_ranges = CalculateTextureSize(texW, texH, ft_face);

    ZC_Texture texture = ZC_Texture::TextureStorage2D(GL_R8, 0, texW, texH, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);

    float texX = 0,
        texY = 0;
    unsigned rowH = 0;
    FT_GlyphSlot glyph = face->glyph;

    size_t characters_count = 0;
    for (UnicodeRange& ur : unicode_ranges) characters_count += ur.end_index - ur.start_index + 1;

    typedef typename ZC_Font::Character FCharacter;
    std::vector<FCharacter> characters;
    characters.reserve(characters_count);

    for (UnicodeRange& ur : unicode_ranges)
    {
        for (ulong symbol = ur.start_index; symbol < ur.end_index + 1; ++symbol)
        {
            if (FT_Load_Char(face, symbol, FT_LOAD_RENDER)) continue;

            if (texX + glyph->bitmap.width + pixelPadding > texMaxW)
            {
                texY += rowH + pixelPadding;
                rowH = 0;
                texX = 0;
            }
            
            if (glyph->bitmap.width != 0 && glyph->bitmap.rows != 0)
                texture.GLTextureSubImage2D(static_cast<int>(texX), static_cast<int>(texY), glyph->bitmap.width, glyph->bitmap.rows,
                    GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
                
            float texX_left = texX / static_cast<float>(texW),
                texY_bottom = texY / static_cast<float>(texH);
            characters.emplace_back(FCharacter
                {
                    .symbol = static_cast<wchar_t>(symbol),
                    .advX = static_cast<float>(glyph->advance.x >> 6),
                    .advY = static_cast<float>(glyph->advance.y >> 6),
                    .bitmapW = static_cast<float>(glyph->bitmap.width),
                    .bitmapH = static_cast<float>(glyph->bitmap.rows),
                    .bitmapLeft = static_cast<float>(glyph->bitmap_left),
                    .bitmapTop = static_cast<float>(glyph->bitmap_top),
                    .texX_left = texX_left,
                    .texY_bottom = texY_bottom,
                    .texX_right = texX_left + (static_cast<float>(glyph->bitmap.width) / texW),
                    .texY_top = texY_bottom + (static_cast<float>(glyph->bitmap.rows) / texH)
                });

            texX += glyph->bitmap.width + pixelPadding;
            rowH = std::max<uint>(glyph->bitmap.rows, rowH);
        }
    }

    return { std::move(texture), std::move(characters) };
}

std::vector<ZC_Fonts::UnicodeRange> ZC_Fonts::CalculateTextureSize(uint& texW, uint& texH, void* ft_face)
{
    std::vector<UnicodeRange> unicode_ranges = GetunicodeRanges();

    FT_Face face = static_cast<FT_Face>(ft_face);
    unsigned rowW = 0,
        rowH = 0;
    FT_GlyphSlot glyph = face->glyph;

    for (UnicodeRange& ur : unicode_ranges)
    {
        for (auto symbol = ur.start_index; symbol < ur.end_index + 1; ++symbol)
        {
            if (FT_Load_Char(face, symbol, FT_LOAD_RENDER))
            {
                ZC_ErrorLogger::Err("Fail FT_Load_Char()!", __FILE__, __LINE__);
                continue;
            }

            if (rowW + glyph->bitmap.width + pixelPadding > texMaxW)
            {
                texW = std::max<uint>(rowW, texW);
                texH += rowH + pixelPadding;
                rowW = 0;
                rowH = 0;
            }

            rowW += glyph->bitmap.width + pixelPadding;
            rowH = std::max<uint>(rowH, glyph->bitmap.rows);
        }
    }

    texW = std::max<uint>(texW, rowW);
    texH += rowH + pixelPadding;

    return unicode_ranges;
}

std::vector<ZC_Fonts::UnicodeRange> ZC_Fonts::GetunicodeRanges()
{
    return
    {       //  symbols
        { .start_index = 32, .end_index = 64 },
        { .start_index = 91, .end_index = 96 },
        { .start_index = 123, .end_index = 126 },
            //  english
        { .start_index = 65, .end_index = 90 },
        { .start_index = 97, .end_index = 122 },
            //  russian
        { .start_index = 0x410, .end_index = 0x44F },   //  alphabet
        { .start_index = 0x401, .end_index = 0x401 },   //  Ё
        { .start_index = 0x451, .end_index = 0x451 }    //  ё
    };
}


//  FontData

bool ZC_Fonts::FontData::operator == (const ZC_FontData& _fontData) const noexcept
{
    return fontData.name == _fontData.name && fontData.pixelsHeight == _fontData.pixelsHeight;
}