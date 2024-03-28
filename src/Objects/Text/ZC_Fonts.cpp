#include <ZC/Objects/Text/ZC_Fonts.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/File/ZC_File.h>

#include <ft2build.h>
#include FT_FREETYPE_H

void ZC_Fonts::Load(ZC_FontNameHeight* pNames, size_t namesCount)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) ZC_ErrorLogger::Err("Fail FT_Init_FreeType()!", __FILE__, __LINE__);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (size_t namesIndex = 0; namesIndex < namesCount; ++namesIndex)
    {
        auto pFontData = ZC_Find(fonts, pNames[namesIndex]);
        if (pFontData) continue;

        FT_Face face;
        if (FT_New_Face(ft, GetPath(pNames[namesIndex].name).c_str(), 0, &face))
        {
            ZC_ErrorLogger::Err("Fail FT_New_Face()!", __FILE__, __LINE__);
            continue;
        }

        if (FT_Set_Pixel_Sizes(face, 0, pNames[namesIndex].pixelsHeight))
        {
            ZC_ErrorLogger::Err("Fail FT_Set_Pixel_Sizes()!", __FILE__, __LINE__);
            continue;
        }

        fonts.emplace_front(pNames[namesIndex], MakeFont(face));
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

ZC_Font* ZC_Fonts::GetFont(const ZC_FontNameHeight& name)
{
    auto pFontData = ZC_Find(fonts, name);
    if (!pFontData) ZC_ErrorLogger::Err("Error in GetFont() Font wasn't loaded!", __FILE__,__LINE__);
    return &(pFontData->font);
}

std::string ZC_Fonts::GetPath(ZC_FontName name)
{
    static const ZC_FSPath ZC_fontsPath = ZC_FSPath(ZC_ZCDirPath).append("fonts");
    switch (name)
    {
    case Arial: return ZC_FSPath(ZC_fontsPath).append("arial.ttf").string();
    default: return "";
    }
}

ZC_Font ZC_Fonts::MakeFont(void* ft_face)
{
    FT_Face face = static_cast<FT_Face>(ft_face);
    uint texW = 0,
        texH = 0;
    CalculateTextureSize(texW, texH, ft_face);

    ZC_Texture texture(GL_TEXTURE_2D, GL_RED, texW, texH, nullptr, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
    texture.Bind();

    float texX = 0,
        texY = 0;
    unsigned rowH = 0;
    FT_GlyphSlot glyph = face->glyph;

    typedef typename ZC_Font::Character FCharacter;
    std::vector<FCharacter> characters;

    for (auto symbol = firstASCII; symbol < lastASCII; ++symbol)
    {
        if (FT_Load_Char(face, symbol, FT_LOAD_RENDER)) continue;

        if (texX + glyph->bitmap.width + pixelPadding > texMaxW)
        {
            texY += rowH + pixelPadding;
            rowH = 0;
            texX = 0;
        }
        
        if (glyph->bitmap.width != 0 && glyph->bitmap.rows != 0)
            glTexSubImage2D(GL_TEXTURE_2D, 0, texX, texY, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
            
        float texX_left = texX / static_cast<float>(texW),
            texY_bottom = texY / static_cast<float>(texH);
        characters.emplace_back(FCharacter
            {
                static_cast<float>(glyph->advance.x >> 6),
                static_cast<float>(glyph->advance.y >> 6),
                static_cast<float>(glyph->bitmap.width),
                static_cast<float>(glyph->bitmap.rows),
                static_cast<float>(glyph->bitmap_left),
                static_cast<float>(glyph->bitmap_top),
                texX_left,
                texY_bottom,
                texX_left + (static_cast<float>(glyph->bitmap.width) / texW),
                texY_bottom + (static_cast<float>(glyph->bitmap.rows) / texH)
            });

        texX += glyph->bitmap.width + pixelPadding;
        rowH = std::max(glyph->bitmap.rows, rowH);
    }
    texture.Unbind();

    return { std::move(texture), std::move(characters) };
}

void ZC_Fonts::CalculateTextureSize(uint& texW, uint& texH, void* ft_face)
{
    FT_Face face = static_cast<FT_Face>(ft_face);
    unsigned rowW = 0,
        rowH = 0;
    FT_GlyphSlot glyph = face->glyph;

    for (auto symbol = firstASCII; symbol < lastASCII; ++symbol)
    {
        if (FT_Load_Char(face, symbol, FT_LOAD_RENDER))
        {
            ZC_ErrorLogger::Err("Fail FT_Load_Char()!", __FILE__, __LINE__);
            continue;
        }

        if (rowW + glyph->bitmap.width + pixelPadding > texMaxW)
        {
            texW = std::max(rowW, texW);
            texH += rowH + pixelPadding;
            rowW = 0;
            rowH = 0;
        }

        rowW += glyph->bitmap.width + pixelPadding;
        rowH = std::max(rowH, glyph->bitmap.rows);
    }

    texW = std::max(texW, rowW);
    texH += rowH + pixelPadding;
}