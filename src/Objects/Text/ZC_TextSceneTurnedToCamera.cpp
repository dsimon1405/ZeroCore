#include <ZC/Objects/Text/ZC_TextSceneTurnedToCamera.h>
    
ZC_TextSceneTurnedToCamera::ZC_TextSceneTurnedToCamera(ZC_FontData fontData, const std::wstring& _text, ZC_TextAlignment _alignment,
        const ZC_Vec3<float>& _position, bool needDraw , float _scale)
    : ZC_TextData(ZC_ShProgs::Get(ShPN_ZC_TextScene), ZC_FO_center, fontData, _text, _alignment, ZC_DrawerLevels::TextScene, needDraw),
    ZC_TornToCamera(_position, _scale == 0.f ? startHeightInScene / fontData.pixelsHeight : _scale)   //  if user don't sets scale, make text height = startHeightInScene
{
    this->dsController.SetUniformsData(ZC_UN_unModel, &(this->model));
}

ZC_TextSceneTurnedToCamera ZC_TextSceneTurnedToCamera::MakeCopy() const
{
    return { *this };
}

ZC_TextSceneTurnedToCamera::ZC_TextSceneTurnedToCamera(const ZC_TextSceneTurnedToCamera& tsttc)
    : ZC_TextData(dynamic_cast<const ZC_TextData&>(tsttc)),
    ZC_TornToCamera(dynamic_cast<const ZC_TornToCamera&>(tsttc))
{
    this->dsController.SetUniformsData(ZC_UN_unModel, &(this->model));
}