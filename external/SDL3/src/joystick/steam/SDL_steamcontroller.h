/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2023 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SDL_steamcontroller_h_
#define SDL_steamcontroller_h_

#include "SDL_internal.h"

typedef SDL_bool (*SteamControllerConnectedCallback_t)(const char *name, SDL_JoystickGUID guid, SDL_JoystickID *device_instance);
typedef void (*SteamControllerDisconnectedCallback_t)(SDL_JoystickID device_instance);

void SDL_InitSteamControllers(SteamControllerConnectedCallback_t connectedCallback,
                              SteamControllerDisconnectedCallback_t disconnectedCallback);
void SDL_GetSteamControllerInputs(int *nbuttons, int *naxes, int *nhats);
void SDL_UpdateSteamControllers(void);
void SDL_UpdateSteamController(SDL_Joystick *joystick);
void SDL_QuitSteamControllers(void);

#endif /* SDL_steamcontroller_h_ */