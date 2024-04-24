#pragma once

#include <Events/ZC_EventsHolder.h>
#include <Tools/Time/ZC_FPS.h>
#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>
#include <Collision/ZC_MouseCollisionWindowController.h>

class ZC_WindowHolder
{
public:
    static inline ZC_uptr<ZC_WindowHolder> upWindowHolder;

    static bool MakeWindowHolder(int flags, int width, int height, const char* name);

    virtual ~ZC_WindowHolder() = default;

    virtual void VGetSize(int& width, int& height) const noexcept = 0;
    virtual void VHideCursor() {}
    virtual void VShowCursor() {}
    virtual void VLimitCursor() {}
    virtual void VUnlimitCursor() {}

    void RunMainCycle();
    void SetFPS(long limit);
    float GetPreviousFrameTime() const noexcept;
    void NeedDrawFPS(bool needDraw);
    bool IsFPSDrawing();
    void GetCursorPosition(float& posX, float& posY);

protected:
    ZC_WindowHolder();

private:
    ZC_uptr<ZC_EventsHolder> upEventsHolder;
	ZC_FPS fps;
    ZC_Renderer renderer;
    ZC_MouseCollisionWindowController mcwc;

    void LoadShProgs();
    void AddZC_RenderLevels();

    /*
    Changes the current buffer to a buffer with a prepared sketch
    */
    virtual void SwapBuffer() = 0;
};