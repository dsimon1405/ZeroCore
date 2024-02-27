#pragma once

namespace ZC_ImGui
{
    bool Init(void* pWindow, void* pGlContext);
    void Destroy();
    void FrameStart();
    void FrameEnd();
    void PollEvents(void* event);
    // void NeedDraw(bool needDraw) noexcept;   //  disable all ImGui throught removing level in ZC_Renderer in another map
};