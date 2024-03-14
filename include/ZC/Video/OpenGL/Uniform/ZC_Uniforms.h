#pragma once

#include "ZC_Uniform.h"

#include <vector>

struct ZC_Uniforms
{
    std::vector<ZC_uptr<ZC_Uniform>> uniforms;

    ZC_Uniforms(std::vector<ZC_uptr<ZC_Uniform>>&& _uniforms);
    ZC_Uniforms(ZC_Uniforms&& u);

    bool operator == (ZC_Uniforms* pUniforms);

    void Set(typename ZC_Uniform::Name name, void* value);
    const void* Get(typename ZC_Uniform::Name name) const;
    void Activate();
    ZC_Uniforms GetCopy() const;
};