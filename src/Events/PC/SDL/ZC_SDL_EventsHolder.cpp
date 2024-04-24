#include "ZC_SDL_EventsHolder.h"

#include <ZC/Video/ZC_Window.h>
#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#ifdef ZC_IMGUI
#include <Video/imgui/ZC_ImGui.h>
#include <ZC_IGWindow.h>
#endif

#include <SDL3/SDL_events.h>

bool ZC_SDL_EventsHolder::PollEvents(float previousFrameTime)
{
    static SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
#ifdef ZC_IMGUI
    ZC_ImGui::PollEvents(&event);
#endif
        switch (event.type)
        {
        case SDL_EVENT_QUIT: return false;
        case SDL_EVENT_WINDOW_RESIZED: sigWindowResize(static_cast<float>(event.window.data1), static_cast<float>(event.window.data2)); break;
        case SDL_EVENT_KEY_DOWN: button.AddActiveDownButton(static_cast<ZC_ButtonID>(event.key.keysym.scancode)); break;
        case SDL_EVENT_KEY_UP: button.CallUpButton(static_cast<ZC_ButtonID>(event.key.keysym.scancode), previousFrameTime); break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
#ifdef ZC_IMGUI		//	if mouse cursor is in one of the ImGui windows, don't poll mouse button down event (same for mouse wheel events)
            if (!ZC_IGWindow::IsCursorInOneOfWindows()) button.AddActiveDownButton(static_cast<ZC_ButtonID>(event.button.button + 512));
            break;
#else
            button.AddActiveDownButton(static_cast<ZC_ButtonID>(event.button.button + 512)); break;
#endif
        case SDL_EVENT_MOUSE_BUTTON_UP: button.CallUpButton(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime); break;
        case SDL_EVENT_MOUSE_MOTION:
        {
            int widht, height;
            ZC_Window::GetSize(widht, height);
            //  sdl window have coords with Y start at the top left corner, but in all ZC system trying to make as in opengl Y start at the buttom left corner, so recalculate Y params to buttom left corner
            mouse.MouseMove(event.motion.x, height - event.motion.y, event.motion.xrel, event.motion.yrel * -1.f, previousFrameTime);
        } break;
        case SDL_EVENT_MOUSE_WHEEL:
#ifdef ZC_IMGUI
            if (!ZC_IGWindow::IsCursorInOneOfWindows()) mouse.MouseScroll(event.wheel.x, event.wheel.y, previousFrameTime);
            break;
#else
            mouse.MouseScroll(event.wheel.x, event.wheel.y, previousFrameTime); break;
#endif
        }
    }
    button.CallActiveButtons(previousFrameTime);
    sigHandleEventsEnd(previousFrameTime);
    return true;
}