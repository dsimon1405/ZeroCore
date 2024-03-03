#include "ZC_EventsHolder.h"

#include <ZC_Config.h>

#ifdef ZC_SDL_VIDEO
#include <Events/PC/SDL/ZC_SDL_EventsHolder.h>
ZC_uptr<ZC_EventsHolder> ZC_EventsHolder::MakeEventsHolder()
{
    return ZC_uptrMakeFromChild<ZC_EventsHolder, ZC_SDL_EventsHolder>();
}
#endif

ZC_EventsHolder::~ZC_EventsHolder()
{
    pEventsHolder = nullptr;
}

ZC_EventsHolder::ZC_EventsHolder()
{
    pEventsHolder = this;
    mouse.Init();
}