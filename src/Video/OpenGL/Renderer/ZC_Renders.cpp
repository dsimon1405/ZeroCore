#include <ZC/Video/OpenGL/Renderer/ZC_Renders.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <cassert>

ZC_Render* ZC_Renders::CreateRender(ZC_RenderLevel renderLevel, typename ZC_Render::DrawState drawState, ZC_FBO&& fbo)
{
    auto prevIter = renders.before_begin();
    for (auto curIter = renders.begin(); curIter != renders.end(); )
    {
        if (*curIter < renderLevel)
        {
            prevIter = curIter;
            ++curIter;
            continue;
        }
        else
        {
            assert(*curIter != renderLevel);     //  double adding
            break;
        }
    }

    auto pRender = &*(renders.emplace_after(prevIter, ZC_Render(renderLevel, std::move(fbo))));
    if (drawState != ZC_Render::DS_None) pRender->SetDrawState(drawState);
    return pRender;
}

void ZC_Renders::EraseRender(ZC_RenderLevel renderLevel)
{
    ZC_ForwardListErase(renders, renderLevel);
}

ZC_Render* ZC_Renders::GetRender(ZC_RenderLevel renderLevel)
{
    auto pRender = ZC_Find(renders, renderLevel);
    assert(pRender);   //  ZC_Render doesn't exists
    return pRender;
}