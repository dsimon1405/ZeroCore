#include <ZC/Video/OpenGL/Shader/ZC_ShaderManager.h>

void ZC_ShaderManager::Clear()
{
    if (!(pipelines.empty())) pipelines.clear();
    shaderPrograms.Clear();
}

bool ZC_ShaderManager::LoadShaderPrograms(const std::vector<ZC_ShaderInput>& vertex, const std::vector<ZC_ShaderInput>& tessControl,
    const std::vector<ZC_ShaderInput>& tessEvaluation, const std::vector<ZC_ShaderInput>& geometry, const std::vector<ZC_ShaderInput>& fragment)
{
    bool result = true;
    for (auto& input : vertex) if (!(shaderPrograms.AddShaderProgram(GL_VERTEX_SHADER, input))) result = false;
    for (auto& input : tessControl) if (!(shaderPrograms.AddShaderProgram(GL_TESS_CONTROL_SHADER, input))) result = false;
    for (auto& input : tessEvaluation) if (!(shaderPrograms.AddShaderProgram(GL_TESS_EVALUATION_SHADER, input))) result = false;
    for (auto& input : geometry) if (!(shaderPrograms.AddShaderProgram(GL_GEOMETRY_SHADER, input))) result = false;
    for (auto& input : fragment) if (!(shaderPrograms.AddShaderProgram(GL_FRAGMENT_SHADER, input))) result = false;
    return result;
}

bool ZC_ShaderManager::LoadShaderProgram(const ZC_ShaderInput& shaderInput, GLenum type)
{
    return shaderPrograms.AddShaderProgram(type, shaderInput);
}

ZC_Pipeline* ZC_ShaderManager::GetPipeLine(int custID_V, int custID_TC, int custID_TE, int custID_G, int custID_F)
{
    for (auto& pipeline : pipelines)
        if (pipeline.custID_V == custID_V && pipeline.custID_TC == custID_TC && pipeline.custID_TE == custID_TE
            && pipeline.custID_G == custID_G && pipeline.custID_F == custID_F) return &pipeline;
    
    return &(pipelines.emplace_front(ZC_Pipeline(
        custID_V != ZC_Shader_None ? shaderPrograms.GetShader(GL_VERTEX_SHADER, custID_V) : nullptr,
        custID_TC != ZC_Shader_None ? shaderPrograms.GetShader(GL_TESS_CONTROL_SHADER, custID_TC) : nullptr,
        custID_TE != ZC_Shader_None ? shaderPrograms.GetShader(GL_TESS_EVALUATION_SHADER, custID_TE) : nullptr,
        custID_G != ZC_Shader_None ? shaderPrograms.GetShader(GL_GEOMETRY_SHADER, custID_G) : nullptr,
        custID_F != ZC_Shader_None ? shaderPrograms.GetShader(GL_FRAGMENT_SHADER, custID_F) : nullptr)));
}