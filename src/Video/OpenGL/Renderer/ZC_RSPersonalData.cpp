#include <ZC/Video/OpenGL/Renderer/ZC_RSPersonalData.h>

//  ZC_RSPersonalData

ZC_RSPersonalData::ZC_RSPersonalData(ZC_RSPDCategory _category)
    : category(_category)
{}

bool ZC_RSPersonalData::operator == (ZC_RSPDCategory _category) const noexcept
{
    return category == _category;
}


//  ZC_RSPDUniformData

ZC_RSPDUniformData::ZC_RSPDUniformData(ZC_UniformName _name, void* _pData)
    : name(_name),
    pData(_pData)
{}

//  ZC_RSPDUniforms

ZC_uptr<ZC_RSPersonalData> ZC_RSPDUniforms::Make(ZC_Uniforms&& _uniforms)
{
    return { new ZC_RSPDUniforms(std::move(_uniforms)) };
}

const void* ZC_RSPDUniforms::GetPointerOnData() const
{
    return &uniforms;
}

void ZC_RSPDUniforms::SetData(ZC_RSPDStoredData* pData)
{
    auto pUniformsData = dynamic_cast<ZC_RSPDUniformData*>(pData);
    uniforms.Set(pUniformsData->name, pUniformsData->pData);
}

ZC_uptr<ZC_RSPersonalData> ZC_RSPDUniforms::MakeCopy() const
{
    return Make(uniforms.GetCopy());
}

ZC_RSPDUniforms::ZC_RSPDUniforms(ZC_Uniforms&& _uniforms)
    : ZC_RSPersonalData(ZC_RSPDC_uniforms),
    uniforms(std::move(_uniforms))
{}


//  ZC_RSPDStencilBorderData

ZC_RSPDStencilBorderData::ZC_RSPDStencilBorderData(float _scaleX, float _scaleY, float _scaleZ, uint _color)
    : scaleX(_scaleX),
    scaleY(_scaleY),
    scaleZ(_scaleZ),
    color(_color)
{}

//  ZC_RSPDStencilBorder

ZC_uptr<ZC_RSPersonalData> ZC_RSPDStencilBorder::Make(const ZC_RSPDStencilBorderData& _stencilBorderData)
{
    return { new ZC_RSPDStencilBorder(_stencilBorderData) };
}

const void* ZC_RSPDStencilBorder::GetPointerOnData() const
{
    return &stencilBorderData;
}

void ZC_RSPDStencilBorder::SetData(ZC_RSPDStoredData* pData)
{
    stencilBorderData = *(dynamic_cast<ZC_RSPDStencilBorderData*>(pData));
}

ZC_uptr<ZC_RSPersonalData> ZC_RSPDStencilBorder::MakeCopy() const
{
    return Make(stencilBorderData);
}

ZC_RSPDStencilBorder::ZC_RSPDStencilBorder(const ZC_RSPDStencilBorderData& _stencilBorderData)
    : ZC_RSPersonalData(ZC_RSPDC_stencilBorder),
    stencilBorderData(_stencilBorderData)
{}