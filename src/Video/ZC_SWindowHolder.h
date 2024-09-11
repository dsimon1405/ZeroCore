#pragma once

#include <Events/ZC_EventsHolder.h>
#include <Tools/Time/ZC_FPS.h>
#include <ZC/Video/OpenGL/Renderer/ZC_Renderer.h>
#include <Collision/ZC_MouseCollisionWindowController.h>
#include <ZC/GUI/Backend/ZC_GUI.h>

class ZC_SWindowHolder
{
public:
    static inline ZC_uptr<ZC_SWindowHolder> upWindowHolder;

    static bool MakeWindowHolder(int flags, int width, int height, const char* name);

    virtual ~ZC_SWindowHolder();

    virtual void VGetSize(int& width, int& height) const noexcept = 0;
    virtual void VHideCursor() {}
    virtual void VShowCursor() {}
    virtual void VLimitCursor() {}
    virtual void VUnlimitCursor() {}
    virtual void VStartInputText() {}
    virtual void VStopInputText() {}

    void RunMainCycle();
    void SetFPS(long limit);
    float GetPreviousFrameTime() const noexcept;
    void NeedDrawFPS(bool needDraw);
    bool IsFPSDrawing();
    void GetCursorPosition(float& posX, float& posY);

protected:
    ZC_SWindowHolder();

private:
    ZC_uptr<ZC_EventsHolder> upEventsHolder;
	ZC_FPS fps;
    ZC_Renderer renderer;
    ZC_uptr<ZC_GUI> upGUI;

    ZC_MouseCollisionWindowController mcwc;

    void LoadShProgs();
    void AddZC_Render();

    //  Changes the current buffer to a buffer with a prepared sketch
    virtual void SwapBuffer() = 0;
};