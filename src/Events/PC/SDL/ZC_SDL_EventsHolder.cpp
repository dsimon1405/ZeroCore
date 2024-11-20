#include "ZC_SDL_EventsHolder.h"

#include <ZC/ZC__System.h>

#include <SDL3/SDL_events.h>

// #include <iostream>
void ZC_SDL_EventsHolder::PollEvents(float previousFrameTime)
{
#ifdef _WIN32
    if (MissPollEvetn()) return;    //   miss poll event at Windows
#endif
    static SDL_Event event;

    sigHandleEventsStart(previousFrameTime);
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
        {
            if (funcWindowCloseButton) funcWindowCloseButton();
            else
            {
                ZC__System::BreakMainCycle();
                return;
            }
        } break;
        case SDL_EVENT_WINDOW_RESIZED: sigWindowResize(static_cast<float>(event.window.data1), static_cast<float>(event.window.data2)); break;
        case SDL_EVENT_KEY_DOWN: buttonHolder.ButtonDown(static_cast<ZC_ButtonID>(event.key.keysym.scancode), previousFrameTime); break;
        case SDL_EVENT_KEY_UP: buttonHolder.ButtonUp(static_cast<ZC_ButtonID>(event.key.keysym.scancode), previousFrameTime); break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN: buttonHolder.ButtonDown(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime); break;
        case SDL_EVENT_MOUSE_BUTTON_UP: buttonHolder.ButtonUp(static_cast<ZC_ButtonID>(event.button.button + 512), previousFrameTime); break;
        case SDL_EVENT_MOUSE_MOTION:
        {
            int widht, height;
            ZC__Window::GetSize(widht, height);
            //  sdl window have coords with Y start at the top left corner, but in all ZC system trying to make as in opengl Y start at the buttom left corner, so recalculate Y params to buttom left corner
            mouse.MouseMove(event.motion.x, static_cast<float>(height) - event.motion.y, event.motion.xrel, event.motion.yrel * -1.f, previousFrameTime);
        } break;
        case SDL_EVENT_MOUSE_WHEEL: mouse.MouseScroll(event.wheel.x, event.wheel.y, previousFrameTime); break;
        case SDL_EVENT_TEXT_INPUT: buttonHolder.buttonPressedDown.TextButtonDown(*event.text.text); break;
        }
    }
    buttonHolder.buttonPressedDown.CallPressedButtons(previousFrameTime);
    sigHandleEventsEnd(previousFrameTime);
}

#ifdef _WIN32
#include <SDL3/SDL_timer.h>

bool ZC_SDL_EventsHolder::MissPollEvetn()
{
    if (ZC__Window::IsFullScreen() || ZC__Window::IsCursorLimited()) return false;      //  on full screen mode window can't be moved or resized by dragging, so as and limited curcor
   
        //  Avoid blocking thread in SDL_PollEvent() at Windows on window grabbing resize or move. Button mouse left/right pressed out of the window don't poll event.
    static bool miss_poll_events = false;

    float cursor_global_x = 0.f;
    float cursor_global_y = 0.f;
    Uint32 mb_global_mask = SDL_GetGlobalMouseState(&cursor_global_x, &cursor_global_y);

    if (mb_global_mask == SDL_BUTTON_LMASK || mb_global_mask == SDL_BUTTON_RMASK)  //  pressed mouse button left or right or both
    {
        if (!miss_poll_events)      //  first frame of button pressed
        {       //  determin is button inside of the window or not
            int win_left_x = -1;
            int win_top_y = -1;
            ZC__Window::GetPosition(win_left_x, win_top_y);    //  window pos counting from top left corner of the display

            if (win_left_x <= cursor_global_x && win_top_y <= cursor_global_y)    //  if cursor pressed under the top border and righter then left border continue check
            {
                int win_width = -1;
                int win_height = -1;
                ZC__Window::GetSize(win_width, win_height);
                
                int win_right_x = win_left_x + win_width;
                int win_bottom_y = win_top_y + win_height;

                miss_poll_events = win_right_x <= cursor_global_x || win_bottom_y < cursor_global_y;     //  true if cursor out of right and bottom borders
            }
            else miss_poll_events = true;   //  button pressed out of top or left border of the window
        }
    }   //  TO THAT ELSE TRYED TO ADD MOUSE BUTTON DOWN EVENT TO AVOID RIGHT BUTTON MOUSE BLOKICN INTO THE PollEvent().
        //  Also tryed SDL_FlushEvent(SDL_EVENT_MOUSE_BUTTON_DOWN / SDL_EVENT_MOUSE_BUTTON_up), SDL_PumpEvents(), to discard mouse button down/up event, nothing helphs...
    else miss_poll_events = false;  //  stop missing poll event

    return miss_poll_events;   //  don't poll events
}
#endif



// #ifdef ZC_IMGUI
// #include <Video/imgui/ZC_ImGui.h>
// #include <ZC_IGWindow.h>
// #include <ZC_IGInputText.h>
// #endif
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