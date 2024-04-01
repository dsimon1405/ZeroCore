#include <ZC/Objects/Text/ZC_TextData.h>

#include <ZC/Objects/Text/ZC_Fonts.h>

void ZC_TextData::NeedDraw(bool needDraw)
{
    needDraw ? rsController.SwitchToLvl(rendererLevel) : rsController.SwitchToLvl(ZC_RL_None);
}

void ZC_TextData::SetColor(const ZC_Vec3<float>& color)
{
    uint colorUint = ZC_PackColorFloatToUInt(color[0], color[1], color[2]);
    ZC_RSPDUniformData unColor(ZC_UN_unColor, &colorUint);
    rsController.SetData(ZC_RSPDC_uniforms, &unColor);
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

void ZC_TextData::SetRendererLevel(ZC_RendererLevel _rendererLevel)
{
    if (rendererLevel == _rendererLevel) return;
    rendererLevel = _rendererLevel;
    if (rsController.IsDrawing()) rsController.SwitchToLvl(rendererLevel);
}

ZC_TextData::ZC_TextData(typename ZC_ShProgs::ShPInitSet* pShPIS, FontOrigin _fontOrigin, const ZC_FontNameHeight& fontData, const std::string& _text,
        ZC_TextAlignment _alignment, ZC_RendererLevel _rendererLevel)
    : pFont(ZC_Fonts::GetFont(fontData)),
    fontOrigin(_fontOrigin),
    spTextSharedData(ZC_sptrMake<SharedData>(_text, _alignment, MakeRendererSet(pShPIS, _text, _alignment))),
    rsController(spTextSharedData->rendererSet.MakeZC_RSController()),
    rendererLevel(_rendererLevel)
{
    rsController.pTexture = pFont->GetTexture();  //  the texture pointer is manually set because in the case of Text one texture can be used in multiple objects and is stored in Font::Font and not in ZC_RendererSet::texSets
    rsController.texturesCount = 1;

    NeedDraw(true);
}

ZC_TextData::ZC_TextData(const ZC_TextData& td)
    : pFont(td.pFont),
    fontOrigin(td.fontOrigin),
    spTextSharedData(td.spTextSharedData),
    rsController(td.rsController.MakeCopy()),
    textWidth(td.textWidth),
    textHeight(td.textHeight),
    rendererLevel(td.rendererLevel)
{
    NeedDraw(true);
}

ZC_RendererSet ZC_TextData::MakeRendererSet(typename ZC_ShProgs::ShPInitSet* pShPIS, const std::string& _text, ZC_TextAlignment _alignment)
{
    ZC_Buffer vbo(GL_ARRAY_BUFFER);
    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, &ebo, 0, 0);

    auto upGLDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(CalculateAndSetTextData(vbo, ebo, _text, _alignment));

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));
    buffers.emplace_front(std::move(ebo));

    return ZC_RendererSet(pShPIS, std::move(vao), std::move(upGLDraw), std::move(buffers));
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