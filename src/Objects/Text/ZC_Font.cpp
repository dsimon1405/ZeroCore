#include <ZC/Objects/Text/ZC_Font.h>

#include <algorithm>

ZC_Font::ZC_Font(ZC_Texture&& _texture, std::vector<Character>&& _characters)
    : texture(std::move(_texture)),
    characters(std::move(_characters))
{}

std::vector<typename ZC_Font::Point> ZC_Font::FillCoords(const std::string& text, ZC_FontOrigin origin, ZC_TextAlignment alignment, float& rTextWidth, float& rTextHeight) const
{
    auto newLineCount = std::count(text.begin(), text.end(), '\n');
    std::vector<LineData> linesData;
    linesData.reserve(newLineCount + 1);   //  + 1 -> minimum 1 line

    std::vector<Point> coords;
    coords.reserve((text.length() - std::count(text.begin(), text.end(), ' ') - newLineCount) * 4); //  4 -> vertices count, except ' ' and '\n'

    FillCoordsAndLines(text, coords, linesData, rTextWidth, rTextHeight);

    switch (origin)
    {
    case ZC_FontOrigin::ZC_FO_center: CalcutateXYOriginCenter(linesData, coords, alignment, rTextWidth, rTextHeight); break;
    case ZC_FontOrigin::ZC_FO_bottomLeft: CalcutateXYOriginBottomLeft(linesData, coords, alignment, rTextWidth); break;
    case ZC_FontOrigin::ZC_FO_bottomCenter: CalcutateXYOriginBottomCenter(linesData, coords, alignment, rTextWidth); break;
    }

    return coords;
}

const ZC_Texture* ZC_Font::GetTexture() const noexcept
{
    return &texture;
}

void ZC_Font::FillCoordsAndLines(const std::string& text, std::vector<Point>& rCoords, std::vector<LineData>& rLinesData, float& rTotalWidth, float& rTotalHeight) const
{
    float startX = 0,
        startY = 0,
        lineHeightWithoughtBottomTail = 0,
        lineHeightBottomTail = 0;
    ulong coordsStartIndex = 0,
        coordsCount = 0;
    bool isFirstLine = true;
    for (auto c : text)
    {
        if (c == '\n')
        {
            AddLine(lineHeightBottomTail, lineHeightWithoughtBottomTail, rLinesData, coordsStartIndex, coordsCount, startX, rTotalWidth, rTotalHeight, isFirstLine);
            startX = 0;
            startY = 0;
            lineHeightWithoughtBottomTail = 0;
            lineHeightBottomTail = 0;
            coordsCount = 0;
            isFirstLine = false;
            continue;
        }

        const Character& ch = characters[static_cast<ulong>(c) - firstASCII];
        float x = startX + ch.bitmapLeft,
            y = startY + (ch.bitmapH - (ch.bitmapH - ch.bitmapTop)),
            w = ch.bitmapW,
            h = ch.bitmapH;

        startX += ch.advX;

        float mustBeUp = h - y;
        if (mustBeUp > lineHeightBottomTail) lineHeightBottomTail = mustBeUp;

        float symbolsHeightWithoughtDownTail = h - mustBeUp;
        if (lineHeightWithoughtBottomTail < symbolsHeightWithoughtDownTail) lineHeightWithoughtBottomTail = symbolsHeightWithoughtDownTail;

        if (!w || !h) continue;   //  whitespace

        ushort texCoord_l = ZC_PackTexCoordFloatToUShort(ch.texX_left);
        ushort texCoord_r = ZC_PackTexCoordFloatToUShort(ch.texX_right);
        ushort texCoord_t = ZC_PackTexCoordFloatToUShort(ch.texY_top);
        ushort texCoord_b = ZC_PackTexCoordFloatToUShort(ch.texY_bottom);

        rCoords.emplace_back(Point{ x, y, texCoord_l, texCoord_b });            //  bl  full texture of symbols is Y inversed!
        rCoords.emplace_back(Point{ x + w, y - h, texCoord_r, texCoord_t });    //  tr  full texture of symbols is Y inversed!
        rCoords.emplace_back(Point{ x, y - h, texCoord_l, texCoord_t });        //  tl  full texture of symbols is Y inversed!
        rCoords.emplace_back(Point{ x + w, y, texCoord_r, texCoord_b });        //  br  full texture of symbols is Y inversed!

        coordsCount += 4;
    }

    AddLine(lineHeightBottomTail, lineHeightWithoughtBottomTail, rLinesData, coordsStartIndex, coordsCount, startX, rTotalWidth, rTotalHeight, isFirstLine);
}

void ZC_Font::CalcutateXYOriginBottomLeft(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth) const
{
    UpY(rLinesData, rCoords);

    if (alignment == ZC_TA_Left) return;    //  Alignment_Left is default

    for (auto& lineData : rLinesData)    //  X alignment
    {
        if (lineData.width == textWidth) continue;
        float offsetX = alignment == ZC_TA_Right ? textWidth - lineData.width : (textWidth - lineData.width) / 2.f;  //  otherwise Alignment_Center
        for (ulong i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
            rCoords[i].x += offsetX;                
    }
}

void ZC_Font::CalcutateXYOriginCenter(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords,
    ZC_TextAlignment alignment, float textWidth, float textHeight) const
{
    float halfTextHeight = textHeight / 2.f;
    for (auto& lineData : rLinesData)    // Y alignment (down tails and next lines height)
    {
        for (ulong i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
            rCoords[i].y += lineData.mustBeUp_Y - halfTextHeight;    //  - halfTextHeight (make texts Y origin center)
    }
    
    MakeCenterX(rLinesData, rCoords, alignment, textWidth);
}

void ZC_Font::CalcutateXYOriginBottomCenter(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth) const
{
    UpY(rLinesData, rCoords);
    MakeCenterX(rLinesData, rCoords, alignment, textWidth);
}

void ZC_Font::UpY(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords) const
{
    for (auto& lineData : rLinesData)    // Y alignment (down tails and next lines height)
    {
        if (lineData.mustBeUp_Y == 0) continue;
        for (ulong i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
            rCoords[i].y += lineData.mustBeUp_Y;
    }
}

void ZC_Font::MakeCenterX(std::vector<LineData>& rLinesData, std::vector<Point>& rCoords, ZC_TextAlignment alignment, float textWidth) const
{
    float halfTextWidth = textWidth / 2.f;
    for (auto& lineData : rLinesData)    //  X alignment
    {
        float offsetX = -halfTextWidth;     //  make texts X origin center
        if (alignment != ZC_TA_Left && lineData.width != textWidth)
            offsetX += alignment == ZC_TA_Right ? textWidth - lineData.width : (textWidth - lineData.width) / 2.f;  //  otherwise Alignment_Center
        for (ulong i = lineData.coordsStartIndex; i < lineData.coordsStartIndex + lineData.coordsCount; ++i)
            rCoords[i].x += offsetX;                
    }
}

void ZC_Font::AddLine(float lineHeightDownTail, float lineHeightWithoughtButtomTail, std::vector<LineData>& rLinesData, ulong& rCoordsStartIndex,
    ulong coordsCount, float startX, float& rTotalWidth, float& rTotalHeight, bool isFirstLine) const
{
    float previousLinesMustBeUp_Y = lineHeightDownTail + lineHeightWithoughtButtomTail + (isFirstLine ? 0 : lineSpacing);   //  if not first line, add line spacing
    for (auto& lineData : rLinesData) lineData.mustBeUp_Y += previousLinesMustBeUp_Y;
    
    rLinesData.emplace_back(LineData{ rCoordsStartIndex, coordsCount, lineHeightDownTail, startX });    //  last param(line width) same as startX
    if (startX > rTotalWidth) rTotalWidth = startX;
    rCoordsStartIndex += coordsCount;

    rTotalHeight += previousLinesMustBeUp_Y;
}