#include <ZC/Video/OpenGL/Buffer/ZC_UBOs.h>

#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>
#include <ZC/Video/OpenGL/Renderer/ZC_Renders.h>

void ZC_UBOs::AddUpdateFunction(ZC_UBO* pUbo, ZC_Function<void()>&& fUpdate, ZC_RenderLevel renderLevel)
{
    renderLevel == ZC_AddToRenderer ? ZC_Renderer::AddUBOs(pUbo, std::move(fUpdate)) : ZC_Renders::GetRender(renderLevel)->AddUBO(pUbo, std::move(fUpdate));
}

void ZC_UBOs::AddUBO(ZC_UBO* pUbo, ZC_Function<void()>&& fUpdate)
{
    ubos.emplace_front(Pair{ pUbo, std::move(fUpdate) });
}

void ZC_UBOs::EraseUBO(ZC_UBO* pUbo)
{
    if (!ZC_ForwardListErase(ubos, pUbo)) ZC_ErrorLogger::Err("EraseUBO(), Can't find ubo to erase!", __FILE__, __LINE__);
}

void ZC_UBOs::UpdateUBO()
{
    for (auto& uboPair : ubos) uboPair.fUpdate();
}


//  Pair

bool ZC_UBOs::Pair::operator == (ZC_UBO* pUbo) const noexcept
{
    return ubo == pUbo;
}
