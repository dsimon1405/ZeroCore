#pragma once

#include "ZC_Texture.h"
#include <ZC/Tools/Container/ZC_DynamicArray.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>

#include <vector>

struct ZC_TexSets
{
    enum Name
    {
        color,
    };

    struct VecCreator
    {
        ZC_DA<Name>& texSets;
        unsigned char nextIndex = 0;

        VecCreator(ZC_DA<Name>& _texSets, std::vector<ZC_Texture>&& _texs)
            : texSets(_texSets),
            texs(std::move(texs))
        {}

        Name* NextName()
        {
            return nextIndex < texSets.size ? &texSets[nextIndex++] : nullptr;
        }

        void Add(ZC_Texture&& tex)
        {
            texs.emplace_back(std::move(tex));
        }

        std::vector<ZC_Texture> GetVecTexture()
        {
            if (texSets.size != texs.size()) ZC_ErrorLogger::Err("Not all textures added!", __FILE__,__LINE__);
            return std::move(texs);
        }

    private:
        std::vector<ZC_Texture> texs;
    };

    ZC_DA<Name> texSets;

    void UniformI(ZC_ShProg& shProg)
    {
        for (size_t texSetsI = 0; texSetsI < texSets.size; ++texSetsI)
        {
            switch (texSets[texSetsI])
            {
            case Name::color: 
            // glUniform1i(shProg.GetUniformLocation("color"), static_cast<GLint>(texSetsI)); 
            break;
            }
        }
    }

    VecCreator GetCreator()
    {
        std::vector<ZC_Texture> texs;
        texs.reserve(texSets.size);
        return {texSets, std::move(texs)};
    }
};