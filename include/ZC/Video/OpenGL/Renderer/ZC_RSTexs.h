#pragma once

#include "ZC_RS.h"
#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/Tools/ZC_string.h>

#include <vector>

struct ZC_RSTexs : public ZC_RS
{
    struct TexSet
    {
        std::string setName;
        std::vector<ZC_Texture> textures;
        LevelController levelController;

        TexSet(std::string&& _setName, std::vector<ZC_Texture>&& _textures);

        TexSet(TexSet&& ts);

        bool operator == (const char* _name) const;
        bool operator == (void* pTexSet) const;
    };

    std::vector<TexSet> texSets;
    unsigned int lvlCounter[Level::None];   //  counting how much LevelDrawing on each ZC_Renderer::Level in each TexSet (total amount for adding and erasing from ZC_Renderer)

    ZC_RSTexs(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_GLDraw>&& _upDraw,
            std::forward_list<ZC_Buffer>&& _buffers, std::vector<TexSet>&& _texSets);

    ZC_RSTexs(ZC_RSTexs&& rs);

    void Draw(Level lvl) override;

    /*
    Return unique pointer to ZC_RendererSetAndDrawingSet.

    texSetName - name of the texture set (ZC_RSTexs::TexSet), can be nullptr if used for heir ZC_RSNonTex.
    stencilScale - scale for drawing the stencil border of the object. Should be greater than 1.0f. Can be anything if ZC_RendererSet::Level::Stencil will not be used.
    stencilColor - color of stencil border packed in unsigned int. Packing -> unsigned char[32] -> [0 - 7]{no metter}, [7 - 15]{red}, [15 - 23]{green}, [23 - 31]{blue}
    */
    ZC_uptr<ZC_RendererSetAndDrawingSet> Make_uptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor) override;

    /*
    Return shared pointer to ZC_RendererSetAndDrawingSet.

    texSetName - name of the texture set (ZC_RSTexs::TexSet), can be nullptr if used for heir ZC_RSNonTex.
    stencilScale - scale for drawing the stencil border of the object. Should be greater than 1.0f. Can be anything if ZC_RendererSet::Level::Stencil will not be used.
    stencilColor - color of stencil border packed in unsigned int. Packing -> unsigned char[32] -> [0 - 7]{no metter}, [7 - 15]{red}, [15 - 23]{green}, [23 - 31]{blue}
    */
    ZC_sptr<ZC_RendererSetAndDrawingSet> Make_sptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor) override;
    void Add(DrawingSet* pDS) override;
    void Erase(DrawingSet* pDS) override;
};