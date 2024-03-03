#include "ZC_SDL_EventsHolder.h"

#include <ZC/Video/ZC_Window.h>
#include <Video/OpenGL/ZC_OpenGL.h>
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
        case SDL_EVENT_WINDOW_RESIZED: WindowResize(); break;
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
        case SDL_EVENT_MOUSE_MOTION: mouse.MouseMove(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel, previousFrameTime); break;
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

void ZC_SDL_EventsHolder::WindowResize()
{
    int width = 0, height = 0;
    ZC_Window::GetSize(width, height);
    glViewport(0, 0, width, height);										//	в ZC_Renderer ????????????????????????
    sigWindowResize(static_cast<float>(width), static_cast<float>(height));
}