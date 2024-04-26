#pragma once

#include <ZC/Video/OpenGL/Uniform/ZC_Uniforms.h>
#include <ZC/Tools/Math/ZC_Math.h>

enum ZC_RSPDCategory
{
    ZC_RSPDC_uniforms,
    ZC_RSPDC_stencilBorder,
};

struct ZC_RSPDStoredData
{
    virtual ~ZC_RSPDStoredData() = default;
};

struct ZC_RSPersonalData
{
    ZC_RSPDCategory category;

    virtual ~ZC_RSPersonalData() = default;
    
    virtual const void* GetPointerOnData() const = 0;
    virtual void SetData(ZC_RSPDStoredData* pData) = 0;
    virtual ZC_uptr<ZC_RSPersonalData> MakeCopy() const = 0;

protected:
    ZC_RSPersonalData(ZC_RSPDCategory _category);
};

struct ZC_RSPDUniformData : public ZC_RSPDStoredData
{
    ZC_UniformName name;
    void* pData;

    ZC_RSPDUniformData(ZC_UniformName _name, void* _data);
};

struct ZC_RSPDUniforms : public ZC_RSPersonalData
{
    static ZC_uptr<ZC_RSPersonalData> Make(ZC_Uniforms&& _uniforms);
    const void* GetPointerOnData() const override;
    void SetData(ZC_RSPDStoredData* pData) override;
    ZC_uptr<ZC_RSPersonalData> MakeCopy() const override;

private:
    ZC_Uniforms uniforms;

    ZC_RSPDUniforms(ZC_Uniforms&& _uniforms);
};


/*
scale - scale for drawing the stencil border of the object. Should be greater than 1.0f. Can be anything if ZC_DrawerSet::Level::Stencil will not be used.
color - color of stencil border packed in unsigned int. Packing -> unsigned char[32] -> [0 - 7]{no metter}, [7 - 15]{red}, [15 - 23]{green}, [23 - 31]{blue}
*/
struct ZC_RSPDStencilBorderData : public ZC_RSPDStoredData
{
    float scale = 0;
    uint color = 0;

    ZC_RSPDStencilBorderData(float _scale, uint _color);
};

struct ZC_RSPDStencilBorder : public ZC_RSPersonalData
{
    static ZC_uptr<ZC_RSPersonalData> Make(const ZC_RSPDStencilBorderData& _stencilBorderData);
    const void* GetPointerOnData() const override;
    void SetData(ZC_RSPDStoredData* pData) override;
    ZC_uptr<ZC_RSPersonalData> MakeCopy() const override;

private:
    ZC_RSPDStencilBorderData stencilBorderData;

    ZC_RSPDStencilBorder(const ZC_RSPDStencilBorderData& _stencilBorderData);
};