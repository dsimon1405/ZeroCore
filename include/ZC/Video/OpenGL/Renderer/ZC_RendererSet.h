#pragma once

#include <ZC/Tools/ZC_uptr.h>
#include <ZC/Tools/ZC_sptr.h>
#include <ZC/Video/OpenGL/Uniform/ZC_Uniforms.h>
#include <ZC_Config.h>

struct ZC_RendererSetAndDrawingSet;

class ZC_RendererSet
{
public:
    enum Level
    {
        //  firt must be 0!
        //  all must be increment by 1
        //  this all use in ZC_RSTexs::lvlCounter as indeces

        Drawing = 0,
        Stencil,        //  must be after scene objects!
        TextWindow,
#ifdef ZC_IMGUI
        ImGui,          //  before None
#endif
        None,           //  must be last use in ZC_RSTexs as arrays size!!!
    };

    virtual ~ZC_RendererSet() = default;

    bool operator == (ZC_RendererSet* pRS) const noexcept;

    virtual void Draw(Level lvl) = 0;

    /*
    Return unique pointer to ZC_RSDrawingSet.

    texSetName - name of the texture set (ZC_RSTexs::TexSet), can be nullptr if used for heir ZC_RSNonTex.
    stencilScale - scale for drawing the stencil border of the object. Should be greater than 1.0f. Can be anything if ZC_RendererSet::Level::Stencil will not be used.
    stencilColor - color of stencil border packed in unsigned int. Packing -> unsigned char[32] -> [0 - 7]{no metter}, [7 - 15]{red}, [15 - 23]{green}, [23 - 31]{blue}
    */
    virtual ZC_uptr<ZC_RendererSetAndDrawingSet> Make_uptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor);
    virtual ZC_sptr<ZC_RendererSetAndDrawingSet> Make_sptrRendererSetDrawingSet(const char* texSetName, float stencilScale, unsigned int stencilColor);

    struct DrawingSet
    {
        ZC_Uniforms uniforms;
        Level lvl = Level::None;
        void* pTexSet = nullptr; //  pointer on TexSet use in ZC_RSTexture::Add(), ZC_RSTexture::Erase()
        float stencilScale = 0.f;
        unsigned int stencilColor = 0;

        //  Return copy with Level::None.
        DrawingSet GetCopy() const
        {
            return { uniforms.GetCopy(), Level::None, pTexSet, stencilScale, stencilColor };
        }
    };
    virtual void Add(DrawingSet* upDS) {}
    virtual void Erase(DrawingSet* upDS) {}
    virtual ZC_ShProg* GetShProg() const noexcept { return nullptr; }

protected:
    ZC_RendererSet() = default;

    void AddToRenderer(Level lvl);
    void EraseFromRenderer(Level lvl);
};