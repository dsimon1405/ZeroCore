#pragma once

namespace ZC_ImGui
{
    bool Init(void* pWindow, void* pGlContext);
    void Destroy();
    void FrameStart();
    void FrameEnd();
    void PollEvents(void* event);
};