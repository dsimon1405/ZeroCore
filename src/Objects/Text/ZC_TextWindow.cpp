#include <ZC/Objects/Text/ZC_TextWindow.h>

ZC_TextWindow::ZC_TextWindow(ZC_FontNameHeight fontData, const std::string& _text, ZC_TextAlignment _alignment,
        float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags)
    : ZC_WindowOrthoIndent(false, 0, 0, windowIndentX, windowIndentY, indentFlags),
    ZC_TextData(ZC_ShProgs::Get(ShPN_ZC_TextWindow), FontOrigin::bottomLeft, fontData, _text, _alignment, ZC_RendererLevels::TextWindow)
{
    SetNewTextSize();

    ZC_RSPDUniformData unPosition(ZC_UN_unPositionWindow, this->currentIndents);
    rsController.SetData(ZC_RSPDC_uniforms, &unPosition);
}

void ZC_TextWindow::SetIndentData(float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags)
{
    this->SetNewIndentParams(windowIndentX, windowIndentY, indentFlags);
}

ZC_TextWindow ZC_TextWindow::MakeCopy(float windowIndentX, float windowIndentY, ZC_WindowOrthoIndentFlags indentFlags) const
{
    ZC_TextWindow copy = { *this };
    copy.SetIndentData(windowIndentX, windowIndentY, indentFlags);
    return copy;
} 

ZC_TextWindow::ZC_TextWindow(const ZC_TextWindow& tw)
    : ZC_WindowOrthoIndent(dynamic_cast<const ZC_WindowOrthoIndent&>(tw)),
    ZC_TextData(dynamic_cast<const ZC_TextData&>(tw))
{
    rsController.SetUniformsData(ZC_UN_unPositionWindow, this->currentIndents);
}

void ZC_TextWindow::SetNewTextSize()
{
    this->SetNewSize(textWidth, textHeight);    //  calculates text indents into the window and store text size in ZC_WindowOrthoIndent
}