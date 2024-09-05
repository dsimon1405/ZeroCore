#include <ZC/Objects/Text/ZC_TextData.h>

#include <ZC/Objects/Text/ZC_Fonts.h>

void ZC_TextData::NeedDraw(bool needDraw)
{
    needDraw ? dsController.SwitchToDrawLvl(renderLevel, drawerLevel) : dsController.SwitchToDrawLvl(renderLevel, ZC_DL_None);
}

bool ZC_TextData::IsDrawing()
{
    return dsController.IsDrawing(renderLevel);
}

void ZC_TextData::SetColorFloat(float red, float green, float blue)
{
    uint colorUint = ZC_PackColorFloatToUInt_RGB(red, green, blue);
    UpdateColor(colorUint);
}

void ZC_TextData::SetColorUChar(uchar red, uchar green, uchar blue)
{
    uint colorUint = ZC_PackColorUCharToUInt_RGB(red, green, blue);
    UpdateColor(colorUint);
}

void ZC_TextData::SetColorUInt(uint packedColor)
{
    UpdateColor(packedColor);
}

void ZC_TextData::SetText(const std::string& _text)
{
    if (spTextSharedData->text == _text) return;
    spTextSharedData->text = _text;

    //  change data in ZC_DrawElements don't changing pointer in spTextSharedData->drawerSet.upGLDraw
    dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->drawerSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
        CalculateAndSetTextData(*(++(spTextSharedData->drawerSet.buffers.begin())), *(spTextSharedData->drawerSet.buffers.begin()),
        spTextSharedData->text, spTextSharedData->alignment);
    SetNewTextSize();
}

void ZC_TextData::SetAlignment(ZC_TextAlignment _alignment)
{
    if (spTextSharedData->alignment == _alignment) return;
    spTextSharedData->alignment = _alignment;

    //  change data in ZC_DrawElements don't changing pointer in spTextSharedData->drawerSet.upGLDraw
    dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->drawerSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
        CalculateAndSetTextData(*(++(spTextSharedData->drawerSet.buffers.begin())), *(spTextSharedData->drawerSet.buffers.begin()),
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
        dynamic_cast<ZC_DrawElements&>(*(spTextSharedData->drawerSet.upGLDraw)) =     //  vbo - second in buffers (firsts added); ebo - first in buffers (seconds added)
            CalculateAndSetTextData(*(++(spTextSharedData->drawerSet.buffers.begin())), *(spTextSharedData->drawerSet.buffers.begin()),
            spTextSharedData->text, spTextSharedData->alignment);

    if (recalculateText) SetNewTextSize();
}

void ZC_TextData::SetDrawerLevel(ZC_DrawerLevel _drawLevel)
{
    if (drawerLevel == _drawLevel) return;
    drawerLevel = _drawLevel;
    if (dsController.IsDrawing(renderLevel)) dsController.SwitchToDrawLvl(renderLevel, drawerLevel);
}

void ZC_TextData::SetFrameBuffer(ZC_RenderLevel _frameBuffer)
{
    if (renderLevel == _frameBuffer) return;
    dsController.SwitchToDrawLvl(renderLevel, ZC_DL_None);  //  stop drawing on previous render level
    renderLevel = _frameBuffer;
    
    //  adds new render level to ZC_DrawerSet (following copy of ZC_RSControllers will create with that ZC_RenderLevel), and to current ZC_DSController
    spTextSharedData->drawerSet.AddRenderLevel(renderLevel);
    dsController.AddRender(renderLevel);
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
        ZC_TextAlignment _alignment, ZC_DrawerLevel _rendererLevel, bool needDraw)
    : pFont(ZC_Fonts::GetFont(fontData)),
    fontOrigin(_fontOrigin),
    spTextSharedData(ZC_sptrMake<SharedData>(_text, _alignment, CreateDrawerSet(pShPIS, _text, _alignment))),
    dsController(spTextSharedData->drawerSet.MakeZC_DSController()),
    drawerLevel(_rendererLevel)
{
    dsController.SetTexturesHolder(ZC_TexturesHolder{ pFont->GetTexture(), 1 });

    SetColorUInt(0);    //  white color default
    if (needDraw) NeedDraw(true);
}

ZC_TextData::ZC_TextData(const ZC_TextData& td)
    : pFont(td.pFont),
    fontOrigin(td.fontOrigin),
    spTextSharedData(td.spTextSharedData),
    dsController(td.dsController.MakeCopy()),
    textWidth(td.textWidth),
    textHeight(td.textHeight),
    drawerLevel(td.drawerLevel)
{}

ZC_DrawerSet ZC_TextData::CreateDrawerSet(typename ZC_ShProgs::ShPInitSet* pShPIS, const std::string& _text, ZC_TextAlignment _alignment)
{
    ZC_Buffer vbo(GL_ARRAY_BUFFER);
    ZC_Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, &ebo, 0, 0);

    auto upGLDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawElements>(CalculateAndSetTextData(vbo, ebo, _text, _alignment));

    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));
    buffers.emplace_front(std::move(ebo));

    return ZC_DrawerSet(pShPIS, std::move(vao), std::move(upGLDraw), std::move(buffers));
}

ZC_DrawElements ZC_TextData::CalculateAndSetTextData(ZC_Buffer& rVBO, ZC_Buffer& rEBO, const std::string& text, ZC_TextAlignment alignment)
{
    textWidth = 0;
    textHeight = 0;
    auto pointsCoords = pFont->FillCoords(text, fontOrigin, alignment, textWidth, textHeight);

    rVBO.GLNamedBufferData(static_cast<long>(pointsCoords.size() * sizeof(typename ZC_Font::Point)), &(pointsCoords[0]), GL_STATIC_DRAW);

    ulong elementsCount = 0;
    GLenum elementsType = 0;
    ZC_DA<uchar> elements = ZC_Buffer::GetTriangleElements(elementsCount, elementsType, static_cast<ulong>(pointsCoords.size() / 4), 0);

    rEBO.GLNamedBufferData(elements.size, elements.Begin(), GL_STATIC_DRAW);
    //  creates on stack ZC_DrawElements cause in CreateDrawerSet() will created in heap ZC_uptr<ZC GLDraw>, in other functions updates data created in CreateDrawerSet()
    return { GL_TRIANGLES, static_cast<int>(elementsCount), elementsType, 0 };
}

void ZC_TextData::UpdateColor(uint color)
{
    if (color == spTextSharedData->color) return;
    spTextSharedData->color = color;

    ZC_RSPDUniformData unColor(ZC_UN_unColor, &color);
    dsController.SetData(ZC_RSPDC_uniforms, &unColor);
}