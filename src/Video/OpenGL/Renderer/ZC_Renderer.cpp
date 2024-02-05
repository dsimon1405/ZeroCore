#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>

void ZC_Renderer::DrawAll()
{
    ZC_UBOs::Update();
    for (auto& rendSetsP : rendSets)
    {
        if (rendSetsP.first.upRMode) rendSetsP.first.upRMode->Start();
        for (auto& rendSet : rendSetsP.second) rendSet->Draw();
        if (rendSetsP.first.upRMode) rendSetsP.first.upRMode->End();
    }
    glBindVertexArray(0);
}

void ZC_Renderer::Add(ZC_RendererSet* pRS)
{
    auto rendSetsI = rendSets.find(pRS->GetLevel());
    if (rendSetsI != rendSets.end())
    {
        rendSetsI->second.emplace_front(pRS);
        return;
    }
    std::forward_list<ZC_RendererSet*> list;
    list.emplace_front(pRS);
    rendSets.emplace(ModeSet(pRS->GetLevel()), std::move(list));
}
    
void ZC_Renderer::Erase(ZC_RendererSet* pRS)
{
    auto rendSetsI = rendSets.find(pRS->GetLevel());
    if (rendSetsI != rendSets.end()) ZC_ForwardListErase(rendSetsI->second, pRS);
}

void ZC_Renderer::EnablePointSize()
{
    glEnable(GL_PROGRAM_POINT_SIZE);
}


//  ModelSet start

ZC_Renderer::ModeSet::ModeSet(unsigned char _level, ZC_uptr<ZC_RMode> _upMode)
    : level(_level),
    upRMode(std::move(_upMode))
{}

ZC_Renderer::ModeSet::ModeSet(ModeSet&& ms)
    : level(ms.level),
    upRMode(std::move(ms.upRMode))
{}

bool ZC_Renderer::ModeSet::operator < (const ModeSet& ms) const noexcept
{
    return level < ms.level;
}