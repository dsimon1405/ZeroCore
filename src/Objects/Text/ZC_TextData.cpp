#include <ZC/Objects/Text/ZC_TextData.h>

#include <ZC/Objects/Text/ZC_Fonts.h>

void ZC_TextData::NeedDraw(bool needDraw)
{
    needDraw ? rsController.SwitchToDrawLvl(frameBuffer, drawLevel) : rsController.SwitchToDrawLvl(frameBuffer, ZC_DL_None);
}

bool ZC_TextData::IsDrawing()
{
    return rsController.IsDrawing(frameBuffer);
}

void ZC_TextData::SetColorFloat(float red, float green, float blue)
{
    uint colorUint = ZC_PackColorFloatToUInt(red, green, blue);
    UpdateColor(colorUint);
}

void ZC_TextData::SetColorUChar(uchar red, uchar green, uchar blue)
{
    uint colorUint = ZC_PackColorUCharToUInt(red, green, blue);
    UpdateColor(colorUint);
}

void ZC_TextData::SetText(const std::string& _text)
{
    if (spTextSharedData->text == _text) return;
    spTextSharedData->text = _text;

    //  change data in ZC_DrawElements don't changing pointer in spTextSharedData->rendererSet.upGLDraw
    dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->rendererSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
        CalculateAndSetTextData(*(++(spTextSharedData->rendererSet.buffers.begin())), *(spTextSharedData->rendererSet.buffers.begin()),
        spTextSharedData->text, spTextSharedData->alignment);
    SetNewTextSize();
}

void ZC_TextData::SetAlignment(ZC_TextAlignment _alignment)
{
    if (spTextSharedData->alignment == _alignment) return;
    spTextSharedData->alignment = _alignment;

    //  change data in ZC_DrawElements don't changing pointer in spTextSharedData->rendererSet.upGLDraw
    dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->rendererSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
        CalculateAndSetTextData(*(++(spTextSharedData->rendererSet.buffers.begin())), *(spTextSharedData->rendererSet.buffers.begin()),
        spTextSharedData->text, spTextSharedData->alignment);
}

void ZC_TextData::SetTextAndAlignment(const std::string& _text, ZC_TextAlignment _alignment)
{
    bool recalculateText = false;
    if (spTextSharedData->text != _text)
    {
        spTextSharedData->text = _text;
        recalculateText = true;
    }
    bool recalculateAlignment = false;
    if (spTextSharedData->alignment != _alignment)
    {
        spTextSharedData->alignment = _alignment;
        recalculateAlignment = true;
    }
    if (recalculateText || recalculateAlignment)
        dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->rendererSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
            CalculateAndSetTextData(*(++(spTextSharedData->rendererSet.buffers.begin())), *(spTextSharedData->rendererSet.buffers.begin()),
            spTextSharedData->text, spTextSharedData->alignment);

    if (recalculateText) SetNewTextSize();
}

void ZC_TextData::SetDrawLevel(ZC_DrawLevel _drawLevel)
{
    if (drawLevel == _drawLevel) return;
    drawLevel = _drawLevel;
    if (rsController.IsDrawing(frameBuffer)) rsController.SwitchToDrawLvl(frameBuffer, drawLevel);
}

void ZC_TextData::SetFrameBuffer(ZC_FrameBuffer _frameBuffer)
{
    if (frameBuffer == _frameBuffer) return;
    rsController.SwitchToDrawLvl(frameBuffer, ZC_DL_None);  //  stop drawing on previous render level
    frameBuffer = _frameBuffer;
    
    //  adds new render level to ZC_RenderSet (following copy of ZC_RSControllers will create with that ZC_RenderLevel), and to current ZC_RSController
    spTextSharedData->rendererSet.AddRenderLevel(frameBuffer);
    rsController.AddRenderLevel(frameBuffer);
}

float ZC_TextData::GetWidth() const noexcept
{
    return textWidth;
}

float ZC_TextData::GetHeight() const noexcept
{
    return textHeight;
}

std::string ZC_TextData::GetText() const noexcept
{
    return spTextSharedData->text;
}

ZC_TextData::ZC_TextData(typename ZC_ShProgs::ShPInitSet* pShPIS, ZC_FontOrigin _fontOrigin, const ZC_FontData& fontData, const std::string& _text,
        ZC_TextAlignment _alignment, ZC_DrawLevel _rendererLevel, bool needDraw)
    : pFont(ZC_Fonts::GetFont(fontData)),
    fontOrigin(_fontOrigin),
    spTextSharedData(ZC_sptrMake<SharedData>(_text, _alignment, MakeRendererSet(pShPIS, _text, _alignment))),
    rsController(spTextSharedData->rendererSet.MakeZC_RSController()),
    drawLevel(_rendererLevel)
{
    rsController.pTexture = pFont->GetTexture();  //  the texture pointer is manually set because in the case of Text one texture can be used in multiple objects and is stored in Font::Font and not in ZC_RenderSet::texSets
    rsController.texturesCount = 1;

    SetColorUChar(0, 0, 0);
    if (needDraw) NeedDraw(true);
}

ZC_TextData::ZC_TextData(const ZC_TextData& td)
    : pFont(td.pFont),
    fontOrigin(td.fontOrigin),
    spTextSharedData(td.spTextSharedData),
    rsController(td.rsController.MakeCopy()),
    textWidth(td.textWidth),
    textHeight(td.textHeight),
    drawLevel(td.drawLevel)
{}

ZC_RenderSet ZC_TextData::MakeRendererSet(typename ZC_ShProgs::ShPInitSet* pShPIS, const std::string& _text, ZC_TextAlignment _alignment)
{
    ZC_Buffer vbo(GL_ARRAY_BUFFER);
    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, &ebo, 0, 0);

    auto upGLDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(CalculateAndSetTextData(vbo, ebo, _text, _alignment));

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));
    buffers.emplace_front(std::move(ebo));

    return ZC_RenderSet(pShPIS, std::move(vao), std::move(upGLDraw), std::move(buffers));
}

ZC_DrawElements ZC_TextData::CalculateAndSetTextData(ZC_Buffer& rVBO, ZC_Buffer& rEBO, const std::string& text, ZC_TextAlignment alignment)
{
    textWidth = 0;
    textHeight = 0;
    auto pointsCoords = pFont->FillCoords(text, fontOrigin, alignment, textWidth, textHeight);

    rVBO.BufferData(static_cast<long>(pointsCoords.size() * sizeof(typename ZC_Font::Point)), &(pointsCoords[0]), GL_STATIC_DRAW);

    ulong elementsCount = 0;
    GLenum elementsType = 0;
    ZC_DA<uchar> elements = ZC_Buffer::GetTriangleElements(elementsCount, elementsType, static_cast<ulong>(pointsCoords.size() / 4), 0);

    rEBO.BufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);
    //  creates on stack ZC_DrawElements cause in MakeRendererSet() will created in heap ZC_uptr<ZC GLDraw>, in other functions updates data created in MakeRendererSet()
    return { GL_TRIANGLES, static_cast<int>(elementsCount), elementsType, 0 };
}

void ZC_TextData::UpdateColor(uint color)
{
    if (color == spTextSharedData->color) return;
    spTextSharedData->color = color;

    ZC_RSPDUniformData unColor(ZC_UN_unColor, &color);
    rsController.SetData(ZC_RSPDC_uniforms, &unColor);
}