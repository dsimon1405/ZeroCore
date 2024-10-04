#include <ZC/Objects/Text/ZC_TextWindow.h>

ZC_TextWindow::ZC_TextWindow(ZC_FontData fontData, const std::string& _text, ZC_TextAlignment _alignment,
        float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags, bool needDraw)
    : ZC_WindowOrthoIndent(false, 0, 0, windowIndentX, windowIndentY, indentFlags),
    ZC_TextData(ZC_ShProgs::Get(ShPN_ZC_TextWindow), ZC_FO_bottomLeft, fontData, _text, _alignment, ZC_DrawerLevels::OrthoBlend, needDraw)
{
    SetNewTextSize();

    dsController.SetUniformsData(ZC_UN_unPositionWindow, &this->currentIndents);
    SetAlpha(1.f);
}

ZC_TextWindow ZC_TextWindow::MakeCopy(float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags) const
{
    ZC_TextWindow copy = { *this };
    copy.SetNewIndentParams(windowIndentX, windowIndentY, indentFlags);
    return copy;
}

void ZC_TextWindow::SetAlpha(float alpha)
{
    alpha = alpha < 0.f ? 0.f
        : alpha > 1.f ? 1.f
        : alpha;
    dsController.SetUniformsData(ZC_UN_unAlpha, &alpha);
}

float ZC_TextWindow::GetAlpha() const noexcept
{
    const float* alpha = static_cast<const float*>(dsController.GetDataFromUniforms(ZC_UN_unAlpha));
    return *alpha;
}

ZC_TextWindow::ZC_TextWindow(const ZC_TextWindow& tw)
    : ZC_WindowOrthoIndent(dynamic_cast<const ZC_WindowOrthoIndent&>(tw)),
    ZC_TextData(dynamic_cast<const ZC_TextData&>(tw))
{
    dsController.SetUniformsData(ZC_UN_unPositionWindow, &this->currentIndents);
}

void ZC_TextWindow::SetNewTextSize()
{
    this->SetNewSize(textWidth, textHeight);    //  calculates text indents into the window and store text size in ZC_WindowOrthoIndent
}