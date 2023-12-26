#pragma once

#include <ZC/Audio/ZC_AudioStream.h>

#include <SDL3/SDL_audio.h>

class ZC_SDL_AudioStream : public ZC_AudioStream
{
public:
    ZC_SDL_AudioStream(const ZC_AudioSet& _audioSet);

    ~ZC_SDL_AudioStream() override;

private:
    SDL_AudioStream* audioStream = nullptr;
    
    static void SDLCALL AudioCallback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount);
};