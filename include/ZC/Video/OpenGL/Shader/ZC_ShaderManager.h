#pragma once

#include "ZC_ShaderPrograms.h"
#include "ZC_Pipeline.h"

#include <vector>


class ZC_ShaderManager
{
public:
    ZC_ShaderManager() = delete;

    static void Clear();
    static bool LoadShaderPrograms(const std::vector<ZC_ShaderInput>& vertex, const std::vector<ZC_ShaderInput>& tessControl,
        const std::vector<ZC_ShaderInput>& tessEvaluation, const std::vector<ZC_ShaderInput>& geometry, const std::vector<ZC_ShaderInput>& fragment);
    static bool LoadShaderProgram(const ZC_ShaderInput& shaderInput, GLenum type);

    //  Params is customID of loaded shader programs. If no shader in pipeline set ZC_Shader_None.
    static ZC_Pipeline* GetPipeLine(int custID_V, int custID_TC, int custID_TE, int custID_G, int custID_F);

private:
    static inline ZC_ShaderPrograms shaderPrograms;
    static inline std::forward_list<ZC_Pipeline> pipelines;
};