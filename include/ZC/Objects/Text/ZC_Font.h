#pragma once

#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/Tools/ZC_string.h>

#include <vector>

enum ZC_TextAlignment
{
    ZC_TA_Left,
    ZC_TA_Right,
    ZC_TA_Center,
};

enum ZC_FontName
{
    Arial,
};

struct ZC_FontNameHeight
{
    ZC_FontName name;
    size_t pixelsHeight;
};

class ZC_Font
{
public:
    struct Character
    {
        float advX; 	// advance.x
        float advY;	    // advance.y

        float bitmapW;	    // bitmap.width;
        float bitmapH;  	// bitmap.height;

        float bitmapLeft;	// bitmap_left;
        float bitmapTop;	// bitmap_top;

        float texX_left;	    // x offset of glyph in texture coordinates
        float texY_bottom;	    // y offset of glyph in texture coordinates
        float texX_right;	    // x offset of glyph in texture coordinates
        float texY_top;	        // y offset of glyph in texture coordinates
    };

    struct Point
    {
        float x,
            y;
        ushort s,  // texCoords X
            t;  // texCoords Y
    };

    enum Origin
    {
        center,
        bottomLeft,
        bottomCenter
    };

    ZC_Font(ZC_Texture&& _texture, std::vector<Character>&& characters);

    std::vector<Point> FillCoords(const std::string& text, Origin origin, ZC_TextAlignment alignment, float& rTextWidth, float& rTextHeight) const;
    const ZC_Texture* GetTexture() const noexcept;

private:
    ZC_Texture texture;
    std::vector<Character> characters;
    static const size_t firstASCII = 32;
    float lineSpacing = characters[static_cast<size_t>(' ') - firstASCII].advX / 3.f;    //  (- 32 is first index of ASCII symbol in chrarcters); line spacing = (white space) / 3

    struct LineData
    {
        size_t coordsStartIndex,
            coordsCount;
        float mustBeUp_Y,
            width;
    };

    void FillCoordsAndLines(const std::string& text, std::vector<Point>& rCoords, std::vector<LineData>& rLinesData, float& rTotalWidth, float& rTotalHeight) const;
    void CalcutateXYOriginBottomLeft(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth) const;
    void CalcutateXYOriginCenter(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth, float textHeight) const;
    void CalcutateXYOriginBottomCenter(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth) const;
    void UpY(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords) const;
    void MakeCenterX(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth) const;
    void AddLine(float lineHeightDownTail, float lineHeightWithoughtButtomTail, std::vector<LineData>& rLinesData, size_t& rCoordsStartIndex,
        size_t coordsCount, float startX, float& rTotalWidth, float& rTotalHeight, bool isFirstLine) const;
};