#include <ZC/Objects/Text/ZC_TextSceneTurnedToCamera.h>
    
ZC_TextSceneTurnedToCamera::ZC_TextSceneTurnedToCamera(ZC_FontNameHeight fontData, const std::string& _text, ZC_TextAlignment _alignment,
        const ZC_Vec3<float>& _position, float _scale)
    : ZC_TextData(ZC_ShProgs::Get(ShPN_ZC_TextScene), FontOrigin::center, fontData, _text, _alignment, ZC_RendererLevels::TextScene),
    ZC_TurnToCamera(_position, _scale == 0.f ? startHeightInScene / fontData.pixelsHeight : _scale)   //  if user don't sets scale, make text height = startHeightInScene
{
    this->rsController.SetUniformsData(ZC_UN_unModel, &(this->model));
}

ZC_TextSceneTurnedToCamera ZC_TextSceneTurnedToCamera::MakeCopy() const
{
    return { *this };
}

ZC_TextSceneTurnedToCamera::ZC_TextSceneTurnedToCamera(const ZC_TextSceneTurnedToCamera& tsttc)
    : ZC_TextData(dynamic_cast<const ZC_TextData&>(tsttc)),
    ZC_TurnToCamera(dynamic_cast<const ZC_TurnToCamera&>(tsttc))
{
    this->rsController.SetUniformsData(ZC_UN_unModel, &(this->model));
}