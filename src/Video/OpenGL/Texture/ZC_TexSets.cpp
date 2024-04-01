#include <ZC/Video/OpenGL/Texture/ZC_TexSets.h>

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Video/OpenGL/ZC_OpenGL.h>

void ZC_TexSets::Uniformli(ZC_ShProg& shProg)
{
    for (ulong texSetsI = 0; texSetsI < texSets.size; ++texSetsI)
    {
        switch (texSets[texSetsI])
        {
        case TextureName::texColor: glUniform1i(shProg.GetUniformLocation("texColor"), static_cast<GLint>(texSetsI)); break;
        }
    }
}

typename ZC_TexSets::VectorOfTexturesCreator ZC_TexSets::GetCreator()
{
    std::vector<ZC_Texture> texs;
    texs.reserve(texSets.size);
    return {texSets, std::move(texs)};
}


//  VecCreator

ZC_TexSets::VectorOfTexturesCreator::VectorOfTexturesCreator(ZC_DA<TextureName>& _texSets, std::vector<ZC_Texture>&& _texs)
    : texSets(_texSets),
    texs(std::move(_texs))
{}

ZC_TexSets::TextureName* ZC_TexSets::VectorOfTexturesCreator::NextName()
{
    return nextIndex < texSets.size ? &texSets[nextIndex++] : nullptr;
}

void ZC_TexSets::VectorOfTexturesCreator::Add(ZC_Texture&& tex)
{
    texs.emplace_back(std::move(tex));
}

std::vector<ZC_Texture> ZC_TexSets::VectorOfTexturesCreator::GetVector()
{
    if (texSets.size != texs.size()) ZC_ErrorLogger::Err("Not all textures added!", __FILE__,__LINE__);
    return std::move(texs);
}