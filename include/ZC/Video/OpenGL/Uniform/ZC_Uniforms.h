#pragma once

#include "ZC_Uniform.h"

#include <vector>

struct ZC_Uniforms
{
    std::vector<ZC_uptr<ZC_Uniform>> uniforms;

    ZC_Uniforms(std::vector<ZC_uptr<ZC_Uniform>>&& _uniforms);
    ZC_Uniforms(ZC_Uniforms&& u);

    bool operator == (ZC_Uniforms* pUniforms);

    void Set(ZC_UniformName name, const void* value);
    void Set(ZC_UniformName name, void* value);
    const void* Get(ZC_UniformName name) const;
    void Activate() const;
    ZC_Uniforms GetCopy() const;
};