#include <ZC/Video/OpenGL/Uniform/ZC_Uniforms.h>

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_Uniforms::ZC_Uniforms(std::vector<ZC_uptr<ZC_Uniform>>&& _uniforms)
    : uniforms(std::move(_uniforms))
{}

ZC_Uniforms::ZC_Uniforms(ZC_Uniforms&& u)
    : uniforms(std::move(u.uniforms))
{}

bool ZC_Uniforms::operator == (ZC_Uniforms* pUniforms)
{
    return this == pUniforms;
}

void ZC_Uniforms::Set(typename ZC_Uniform::Name name, void* value)
{
    for (auto& uniform : uniforms)
    {
        if (uniform->name == name)
        {
            uniform->Set(value);
            return;
        }
    }
    ZC_ErrorLogger::Err("Can't find Uniform " + std::to_string(name) + ", to set value!", __FILE__, __LINE__);
}

void* ZC_Uniforms::Get(typename ZC_Uniform::Name name)
{
    for (auto& uniform : uniforms)
        if (uniform->name == name) return uniform->Get();
    ZC_ErrorLogger::Err("Can't find Uniform " + std::to_string(name) + ", to return value!", __FILE__, __LINE__);
    return nullptr;
}

void ZC_Uniforms::Activate()
{
    for (auto& uniform : uniforms) uniform->Activate();
}

ZC_Uniforms ZC_Uniforms::GetCopy()
{
    std::vector<ZC_uptr<ZC_Uniform>> copy;
    copy.reserve(uniforms.size());
    for (auto& uniform : uniforms) copy.emplace_back(uniform->GetCopy());
    return ZC_Uniforms(std::move(copy));
}