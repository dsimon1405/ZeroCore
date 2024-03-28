#pragma once

#include "ZC_Font.h"
#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>
#include <ZC/Tools/Math/ZC_Vec3.h>

class ZC_TextData
{
public:
    virtual ~ZC_TextData() = default;

    //  Add to ZC_Renderer or removes from there.
    void NeedDraw(bool needDraw);
    //  Sets texts color.
    void SetColor(const ZC_Vec3<float>& color);
    //  Resets text.
    void SetText(const std::string& _text);
    //  Changes alignment.
    void SetAlignment(ZC_TextAlignment _alignment);
    //  If need to change text and alignment at the same time, use this function. Calling SetText() and SetAlignment() separately is less effective.
    void SetTextAndAlignment(const std::string& _text, ZC_TextAlignment _alignment);
    /*
    Changes level on which gonna be added text in ZC_Renderer. This functionality is designed to be added to user-added rendering layers, not those included with ZeroCore.
    Don't use it if you don't know what you're doing.
    */
    void SetRendererLevel(ZC_RendererLevel _rendererLevel);

protected:
    struct SharedData
    {
        std::string text;
        ZC_TextAlignment alignment;
        ZC_RendererSet rendererSet;
    };

    typedef typename ZC_Font::Origin FontOrigin;
    ZC_Font* pFont;
    FontOrigin fontOrigin;
    ZC_sptr<SharedData> spTextSharedData;
    ZC_RSController rsController;
    float textWidth,
        textHeight;
    ZC_RendererLevel rendererLevel;
    static inline float startHeightInScene = 2.f;

    ZC_TextData(typename ZC_ShProgs::ShPInitSet* pShPIS, FontOrigin _fontOrigin, const ZC_FontNameHeight& fontData, const std::string& _text,
            ZC_TextAlignment _alignment, ZC_RendererLevel _rendererLevel);

    ZC_TextData(const ZC_TextData& td);

private:
    virtual void SetNewTextSize() {};
    
    ZC_RendererSet MakeRendererSet(typename ZC_ShProgs::ShPInitSet* pShPIS, const std::string& _text, ZC_TextAlignment _alignment);
    ZC_DrawElements CalculateAndSetTextData(ZC_Buffer& rVBO, ZC_Buffer& rEBO, const std::string& text, ZC_TextAlignment alignment);
};