#pragma once

#include "ZC_Texture.h"
#include <ZC/Tools/Container/ZC_DA.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>

#include <vector>

struct ZC_TexSets
{
    enum TextureName
    {
        texColor,
    };

    ZC_DA<TextureName> texSets;

    /*
    Struct for adding textures in std::vector<ZC_Texture> in right order
    
    Using:
    NextName() - return name of the next texture for adding.
    Add(...) - add texture that name return NextName().

    ZC_TexSets texSets;     - taking from ShPInitSet;

    VectorOfTexturesCreator votc = texSets.GetCreator();
    TextureName textureName = votc.NextName();
    ZC_Texture texture = Load texture that must have name textureName...
    votc.Add(std::move(texture));

    std::vector<ZC_Texture> textures = votc.GetVector();

    Than use vector to set in ZC_RSTexs::TexSet with name of set.
    */
    struct VectorOfTexturesCreator
    {
        ZC_DA<TextureName>& texSets;
        unsigned char nextIndex = 0;

        VectorOfTexturesCreator(ZC_DA<TextureName>& _texSets, std::vector<ZC_Texture>&& _texs);

        TextureName* NextName();
        void Add(ZC_Texture&& tex);
        std::vector<ZC_Texture> GetVector();

    private:
        std::vector<ZC_Texture> texs;
    };

    void Uniformli(ZC_ShProg& shProg);
    VectorOfTexturesCreator GetCreator();
};