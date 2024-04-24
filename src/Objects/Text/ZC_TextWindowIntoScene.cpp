#include <ZC/Objects/Text/ZC_TextWindowIntoScene.h>
    
ZC_TextWindowIntoScene::ZC_TextWindowIntoScene(ZC_FontData fontData, ZC_FontOrigin fontOrigin, const std::string &_text, ZC_TextAlignment _alignment,
        const ZC_Vec3<float>& position, bool needDraw)
    : ZC_TextData(ZC_ShProgs::Get(ShPN_ZC_TextWindowIntoScene), fontOrigin, fontData, _text, _alignment, ZC_DrawLevels::TextWindowIntoScene, needDraw),
    positionIntoScene(position)
{
    rsController.SetUniformsData(ZC_UN_unPositionScene, positionIntoScene.Begin());
}

void ZC_TextWindowIntoScene::SetPosition(const ZC_Vec3<float>& position) noexcept
{
    positionIntoScene = position;
}

ZC_Vec3<float> ZC_TextWindowIntoScene::GetPosition() const noexcept
{
    return positionIntoScene;
}

ZC_TextWindowIntoScene ZC_TextWindowIntoScene::MakeCopy() const
{
    return { *this };
}

ZC_TextWindowIntoScene::ZC_TextWindowIntoScene(const ZC_TextWindowIntoScene& twis)
    : ZC_TextData(dynamic_cast<const ZC_TextData&>(twis)),
    positionIntoScene(twis.positionIntoScene)
{
    rsController.SetUniformsData(ZC_UN_unPositionScene, positionIntoScene.Begin());
}