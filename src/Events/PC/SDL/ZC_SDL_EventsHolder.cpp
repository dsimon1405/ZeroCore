#include "ZC_SDL_EventsHolder.h"

#include <ZC/Video/ZC_SWindow.h>
// #ifdef ZC_IMGUI
// #include <Video/imgui/ZC_ImGui.h>
// #include <ZC_IGWindow.h>
// #include <ZC_IGInputText.h>
// #endif

#include <SDL3/SDL_events.h>

// bool ZC_SDL_EventsHolder::PollEvents(float previousFrameTime, ZC_GUI_EventManager* pGUI_EM)
// {
//     static SDL_Event event;
    
//     sigHandleEventsStart(previousFrameTime);

//     while (SDL_PollEvent(&event) != 0)
//     {
// #ifdef ZC_IMGUI
//     ZC_ImGui::PollEvents(&event);
// #endif
//         switch (event.type)
//         {
//         case SDL_EVENT_QUIT: return false;
//         case SDL_EVENT_WINDOW_RESIZED: sigWindowResize(static_cast<float>(event.window.data1), static_cast<float>(event.window.data2)); break;
//         case SDL_EVENT_KEY_DOWN: if (!ZC_IGInputText::IsOneOfInputTextFocused()) buttonHolder.ButtonDown(static_cast<ZC_ButtonID>(event.key.keysym.scancode), previousFrameTime); break;
//         case SDL_EVENT_KEY_UP: buttonHolder.ButtonUp(static_cast<ZC_ButtonID>(event.key.keysym.scancode), previousFrameTime); break;
//         case SDL_EVENT_MOUSE_BUTTON_DOWN: if (!ZC_IGWindow::IsCursorInOneOfWindows()) buttonHolder.ButtonDown(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime); break;
//         case SDL_EVENT_MOUSE_BUTTON_UP: buttonHolder.ButtonUp(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime); break;
//         case SDL_EVENT_MOUSE_MOTION:
//         {
//             int widht, height;
//             ZC_SWindow::GetSize(widht, height);
//             //  sdl window have coords with Y start at the top left corner, but in all ZC system trying to make as in opengl Y start at the buttom left corner, so recalculate Y params to buttom left corner
//             mouse.MouseMove(event.motion.x, height - event.motion.y, event.motion.xrel, event.motion.yrel * -1.f, previousFrameTime);
//         } break;
//         case SDL_EVENT_MOUSE_WHEEL: if (!ZC_IGWindow::IsCursorInOneOfWindows()) mouse.MouseScroll(event.wheel.x, event.wheel.y, previousFrameTime); break;
//         }
//     }
//     buttonHolder.buttonPressedDown.CallPressedButtons(previousFrameTime);
//     sigHandleEventsEnd(previousFrameTime);
//     return true;
// }

// #include <iostream>
// std::cout<<"x = "<<event.motion.x<<"; y = "<<static_cast<float>(height) - event.motion.y<<"; rel_x = "<<event.motion.xrel<<"; rel_y = "<<(event.motion.yrel * -1.f)<<std::endl;
bool ZC_SDL_EventsHolder::PollEvents(float previousFrameTime, ZC_GUI_EventManager& gui_eventManager)
{
    static SDL_Event event;

    sigHandleEventsStart(previousFrameTime);

    ZC_ButtonID butID;  //  save scancode in SDL_EVENT_KEY_DOWN (calls frist) for SDL_EVENT_MOUSE_MOTION

    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT: return false;
        case SDL_EVENT_WINDOW_RESIZED: sigWindowResize(static_cast<float>(event.window.data1), static_cast<float>(event.window.data2)); break;
        case SDL_EVENT_KEY_DOWN:
        {
            butID = static_cast<ZC_ButtonID>(event.key.keysym.scancode);
            buttonHolder.ButtonDown(static_cast<ZC_ButtonID>(event.key.keysym.scancode), previousFrameTime);
        }  break;
        case SDL_EVENT_KEY_UP: buttonHolder.ButtonUp(static_cast<ZC_ButtonID>(event.key.keysym.scancode), previousFrameTime); break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        {
            if (gui_eventManager.ButtonDown(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime))
                buttonHolder.ButtonDown(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime);
        } break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            if (gui_eventManager.ButtonUp(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime))
                buttonHolder.ButtonUp(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime);
        } break;
        case SDL_EVENT_MOUSE_MOTION:
        {
            int widht, height;
            ZC_SWindow::GetSize(widht, height);
            //  sdl window have coords with Y start at the top left corner, but in all ZC system trying to make as in opengl Y start at the buttom left corner, so recalculate Y params to buttom left corner
            mouse.MouseMove(event.motion.x, static_cast<float>(height) - event.motion.y, event.motion.xrel, event.motion.yrel * -1.f, previousFrameTime);
        } break;
        case SDL_EVENT_MOUSE_WHEEL: mouse.MouseScroll(event.wheel.x, event.wheel.y, previousFrameTime); break;
        case SDL_EVENT_TEXT_INPUT:
        {
            unsigned char ch = *event.text.text;
            //  use butID
        } break;
        }
    }
    buttonHolder.buttonPressedDown.CallPressedButtons(previousFrameTime);
    sigHandleEventsEnd(previousFrameTime);
    return true;
}