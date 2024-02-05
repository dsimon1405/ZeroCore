#include <ZC/Video/OpenGL/Renderer/ZC_RendererSet.h>

#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

ZC_RendererSet::ZC_RendererSet(unsigned char _rendererLevel)
    : rendererLevel(_rendererLevel)
{}

ZC_RendererSet::ZC_RendererSet(ZC_RendererSet&& rs)
    : rendererLevel(rs.rendererLevel),
    uniformsCount(rs.uniformsCount)
{}

ZC_RendererSet::~ZC_RendererSet()
{
    ZC_Renderer::Erase(this);
}

bool ZC_RendererSet::operator == (ZC_RendererSet* pRS) const noexcept
{
    return this == pRS;
}

unsigned char ZC_RendererSet::GetLevel() const noexcept
{
    return rendererLevel;
}

void ZC_RendererSet::AddToRenderer()
{
    if (uniformsCount++ == 0) ZC_Renderer::Add(this);
}

void ZC_RendererSet::EraseFromRenderer()
{
    if (--uniformsCount == 0) ZC_Renderer::Erase(this);
}


#include <ZC/Tools/Container/ZC_ContFunc.h>

ZC_RS::ZC_RS(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_Draw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers, unsigned char _rendererLevel)
    : ZC_RendererSet(_rendererLevel),
    pShP(&pShPInitSet->shProg),
    pBaseUniforms(&pShPInitSet->uniforms),
    vao(std::move(_vao)),
    upDraw(std::move(_upDraw)),
    buffers(std::move(_buffers))
{}

ZC_RS::ZC_RS(ZC_RS&& rs)
    : ZC_RendererSet(dynamic_cast<ZC_RendererSet&&>(rs)),
    pShP(rs.pShP),
    pBaseUniforms(std::move(rs.pBaseUniforms)),
    vao(std::move(rs.vao)),
    upDraw(std::move(rs.upDraw)),
    buffers(std::move(rs.buffers))
{}

void ZC_RS::SetStencilShaderProgData(typename ZC_ShProgs::ShPInitSet* pShPInitSet) noexcept
{
    pShPStencil = &pShPInitSet->shProg;
    pActiveUniformsStencil = &pShPInitSet->uniforms;
}





ZC_RSNonTex::ZC_RSNonTex(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_Draw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers, unsigned char _rendererLevel)
    : ZC_RS(pShPInitSet, std::move(_vao), std::move(_upDraw), std::move(_buffers), _rendererLevel)
{}

ZC_RSNonTex::ZC_RSNonTex(ZC_RSNonTex&& rs)
    : ZC_RS(dynamic_cast<ZC_RS&&>(rs)),
    activeUniforms(std::move(rs.activeUniforms))
{}

#include <Video/OpenGL/ZC_OpenGL.h>
void ZC_RSNonTex::Draw()
{
    vao.BindVertexArray();
    pShP->Use();
    for (auto& uniforms : activeUniforms)
    {
        uniforms.Activate();
        upDraw->Draw();
    }
    if (!stencilUniforms.empty())
    {
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        for (auto suIter = stencilUniforms.begin(); ; )
        {
            glStencilFunc(GL_ALWAYS, 1, 255);
            glStencilMask(255);
            suIter->uniforms.Activate();
            upDraw->Draw();

            auto model = *static_cast<ZC_Mat4<float>*>(suIter->uniforms.Get(ZC_Uniform::Name::model));
            model.Scale({1.1f, 1.1f, 1.1f});
            pActiveUniformsStencil->Set(ZC_Uniform::Name::model, &model);
            pActiveUniformsStencil->Set(ZC_Uniform::Name::color, &(suIter->color));

            glDisable(GL_DEPTH_TEST);
            pShPStencil->Use();
            glStencilFunc(GL_NOTEQUAL, 1, 255);
            glStencilMask(0);
            pActiveUniformsStencil->Activate();
            upDraw->Draw();
            glEnable(GL_DEPTH_TEST);
            
            if (++suIter != stencilUniforms.end()) pShP->Use();
            else break;
        }
        glDisable(GL_STENCIL_TEST);
    }
}

ZC_Uniforms* ZC_RSNonTex::Add()
{
    AddToRenderer();
    return &(activeUniforms.emplace_front(pBaseUniforms->GetCopy()));
}

void ZC_RSNonTex::Erase(ZC_Uniforms* uniforms)
{
    ZC_ForwardListErase(activeUniforms, uniforms);
    EraseFromRenderer();
}

void ZC_RSNonTex::AddStencil(ZC_Uniforms* pUniforms, const ZC_Vec3<float>& color)
{
    auto previous = activeUniforms.before_begin();
    for (auto auIter = activeUniforms.begin(); auIter != activeUniforms.end(); ++auIter)
    {
        if (*auIter == pUniforms)
        {
            stencilUniforms.emplace_front(std::move(*auIter), color);
            activeUniforms.erase_after(previous);
            return;
        }
        previous = auIter;
    }
    ZC_ErrorLogger::Err("Can't find ZC_Uniform to add to the stencil test!", __FILE__,__LINE__);
}

void ZC_RSNonTex::EraseStencil(ZC_Uniforms* pUniforms)
{
    auto previous = stencilUniforms.before_begin();
    for (auto suIter = stencilUniforms.begin(); suIter != stencilUniforms.end(); ++suIter)
    {
        if (suIter->uniforms == pUniforms)
        {
            activeUniforms.emplace_front(std::move(suIter->uniforms));
            stencilUniforms.erase_after(previous);
            return;
        }
        previous = suIter;
    }
    ZC_ErrorLogger::Err("Can't find ZC_Uniform to replace from stensil to active!", __FILE__,__LINE__);
}



// ZC_RSNTStencil::ZC_RSNTStencil(typename ZC_ShProgs::ShPInitSet* pShPIS, typename ZC_ShProgs::ShPInitSet* pShPISStencil,
//         ZC_VAO&& _vao, ZC_uptr<ZC_Draw>&& _upDraw, std::forward_list<ZC_Buffer>&& _buffers, unsigned char _rendererLevel)
//     : ZC_RSNonTex(pShPIS, std::move(_vao), std::move(_upDraw), std::move(_buffers), _rendererLevel),
//     pShPStencil(&pShPISStencil->shProg),
//     baseUniformsStencil(&pShPISStencil->uniforms)
// {}

// ZC_RSNTStencil::ZC_RSNTStencil(ZC_RSNTStencil&& rs)
//     : ZC_RSNonTex(dynamic_cast<ZC_RSNonTex&&>(rs)),
//     pShPStencil(rs.pShPStencil),
//     activeUniformsStencil(std::move(rs.activeUniformsStencil))
// {}

// void ZC_RSNTStencil::Draw()
// {
//     // vao.BindVertexArray();
//     // pShP->Use();
//     // for (auto& uniforms : activeUniforms)
//     // {
//     //     uniforms.Activate();
//     //     upDraw->Draw();
//     // }
// }

// typename ZC_RSNTStencil::Uniforms ZC_RSNTStencil::Add()
// {
//     AddToRenderer();
//     return { &(activeUniforms.emplace_front(baseUniforms->GetCopy())), &(activeUniformsStencil.emplace_front(baseUniformsStencil->GetCopy())) };
// }

// void ZC_RSNTStencil::Erase(typename ZC_RSNTStencil::Uniforms& uniforms)
// {
//     ForwardListErase(activeUniforms, uniforms);
//     EraseFromRenderer();
// }

// void ZC_RSNTStencil::SetStencilMode(bool _stencilMode)
// {
//     stencilMode = _stencilMode;
// }




ZC_RSTexs::ZC_RSTexs(typename ZC_ShProgs::ShPInitSet* pShPInitSet, ZC_VAO&& _vao, ZC_uptr<ZC_Draw>&& _upDraw,
        std::forward_list<ZC_Buffer>&& _buffers, std::vector<TexSet>&& _texSets, unsigned char _rendererLevel)
    : ZC_RS(pShPInitSet, std::move(_vao), std::move(_upDraw), std::move(_buffers), _rendererLevel),
    texSets(std::move(_texSets))
{
    if (texSets.empty())
    {
        if (pShPInitSet->pTexSets) ZC_ErrorLogger::Err("There's no TexSet!");
        texSets.reserve(1);
        texSets.emplace_back(TexSet("", {}));
    }
}

ZC_RSTexs::ZC_RSTexs(ZC_RSTexs&& rs)
    : ZC_RS(dynamic_cast<ZC_RS&&>(rs)),
    texSets(std::move(rs.texSets))
{}

void ZC_RSTexs::Draw()
{
    vao.BindVertexArray();
    pShP->Use();
    for (auto& texSet : texSets) texSet.Draw(upDraw);
}

ZC_Uniforms* ZC_RSTexs::Add(const char* texSetName)
{
    auto pTexSet = std::find(texSets.begin(), texSets.end(), texSetName);
    if (pTexSet == texSets.end()) ZC_ErrorLogger::Err("There's no TexSet with name: " + (texSetName ? std::string(texSetName) : ""));
    AddToRenderer();
    return &(pTexSet->activeUniforms.emplace_front(pBaseUniforms->GetCopy()));
}

void ZC_RSTexs::Erase(ZC_Uniforms* uniforms, const char* texSetName)
{
    auto pTexSet = std::find(texSets.begin(), texSets.end(), texSetName);
    if (pTexSet == texSets.end()) ZC_ErrorLogger::Err("There's no TexSet with name: " + (texSetName ? std::string(texSetName) : ""));
    ZC_ForwardListErase(pTexSet->activeUniforms, uniforms);
    EraseFromRenderer();
}


//  TexSet start

ZC_RSTexs::TexSet::TexSet(std::string&& _name, std::vector<ZC_Texture>&& _textures)
    : name(std::move(name)),
    textures(std::move(_textures))
{}

ZC_RSTexs::TexSet::TexSet(TexSet&& ts)
    : name(std::move(ts.name)),
    textures(std::move(ts.textures)),
    activeUniforms(std::move(ts.activeUniforms))
{}

bool ZC_RSTexs::TexSet::operator == (const char* _name) const
{
    return name == _name;
}

void ZC_RSTexs::TexSet::Draw(ZC_uptr<ZC_Draw>& upDraw)
{
    for (size_t i = 0; i < textures.size(); ++i) textures[i].ActiveTexture(i);
    for (auto& uniforms : activeUniforms)
    {
        uniforms.Activate();
        upDraw->Draw();
    }
}