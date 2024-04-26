#pragma once

#include "ZC_Font.h"
#include <ZC/Video/OpenGL/Renderer/ZC_DrawerSet.h>
#include <ZC/Tools/Math/ZC_Vec3.h>

class ZC_TextData
{
public:
    virtual ~ZC_TextData() = default;

    //  Add to ZC_Renderer or removes from there.
    void NeedDraw(bool needDraw);
    bool IsDrawing();
    //  Sets text color, each color range [0.f - 1.f].
    void SetColorFloat(float red, float green, float blue);
    //  Sets text color, each color range [0 - 255].
    void SetColorUChar(uchar red, uchar green, uchar blue);
    //  Sets text color packed in (unsigned int 32) with packing shema uint[32] indices -> [0-1] nothing, [2-11] red, [12-21] green, [22-31] blue.
    //  For packing may be used ZC_PackColorFloatToUInt(...) or ZC_PackColorUCharToUInt(...) from <ZC/Tools/Math/ZC_Math.h>.
    void SetColorUInt(uint packedColor);
    //  Resets text.
    void SetText(const std::string& _text);
    //  Changes alignment.
    void SetAlignment(ZC_TextAlignment _alignment);
    //  If need to change text and alignment at the same time, use this function. Calling SetText() and SetAlignment() separately is less effective.
    void SetTextAndAlignment(const std::string& _text, ZC_TextAlignment _alignment);
    //  Changes the level at which text will be added to the ZC_Render. If the text is not currently at the ZC_DL_None level, it will switch to a new level.
    void SetDrawLevel(ZC_DrawerLevel _drawLevel);
    //  Adds new freameBuffer for drawing on it's drawing levels. Needs call NeedDraw(true) for start drawing on new render level.
    void SetFrameBuffer(ZC_RenderLevel _renderBuffer);
    float GetWidth() const noexcept;
    float GetHeight() const noexcept;
    std::string GetText() const noexcept;

protected:
    struct SharedData
    {
        std::string text;
        ZC_TextAlignment alignment;
        ZC_DrawerSet drawerSet;
        uint color = 1;    //  packed color
    };

    ZC_Font* pFont;
    ZC_FontOrigin fontOrigin;
    ZC_sptr<SharedData> spTextSharedData;
    ZC_DSController dsController;
    float textWidth,
        textHeight;
    ZC_RenderLevel renderLevel = ZC_RL_Default;
    ZC_DrawerLevel drawerLevel;
    static inline float startHeightInScene = 2.f;

    ZC_TextData(typename ZC_ShProgs::ShPInitSet* pShPIS, ZC_FontOrigin _fontOrigin, const ZC_FontData& fontData, const std::string& _text,
            ZC_TextAlignment _alignment, ZC_DrawerLevel _rendererLevel, bool needDraw);

    ZC_TextData(const ZC_TextData& td);

private:
    virtual void SetNewTextSize() {};
    
    ZC_DrawerSet CreateDrawerSet(typename ZC_ShProgs::ShPInitSet* pShPIS, const std::string& _text, ZC_TextAlignment _alignment);
    ZC_DrawElements CalculateAndSetTextData(ZC_Buffer& rVBO, ZC_Buffer& rEBO, const std::string& text, ZC_TextAlignment alignment);
    void UpdateColor(uint color);
};