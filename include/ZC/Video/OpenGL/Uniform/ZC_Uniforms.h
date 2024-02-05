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
    void* Get(typename ZC_Uniform::Name name);
    void Activate();
    ZC_Uniforms GetCopy();
};