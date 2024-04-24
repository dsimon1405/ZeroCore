#include <ZC/Video/OpenGL/Renderer/ZC_Renders.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>

#include <cassert>

ZC_Render* ZC_Renders::CreateRender(ZC_FrameBuffer frameBuffer, typename ZC_Render::DrawState drawState, ZC_FBO&& fbo)
{
    auto prevIter = renders.before_begin();
    for (auto curIter = renders.begin(); curIter != renders.end(); )
    {
        if (*curIter < frameBuffer)
        {
            prevIter = curIter;
            ++curIter;
            continue;
        }
        else
        {
            assert(*curIter != frameBuffer);     //  double adding
            break;
        }
    }

    auto pRender = &*(renders.emplace_after(prevIter, ZC_Render(frameBuffer, std::move(fbo))));
    if (drawState != ZC_Render::DS_None) pRender->SetDrawState(drawState);
    return pRender;
}

void ZC_Renders::EraseRender(ZC_FrameBuffer frameBuffer)
{
    ZC_ForwardListErase(renders, frameBuffer);
}

ZC_Render* ZC_Renders::GetRender(ZC_FrameBuffer frameBuffer)
{
    auto pRender = ZC_Find(renders, frameBuffer);
    assert(pRender);   //  ZC_Render doesn't exists
    return pRender;
}