#include <ZC/Audio/ZC_Audio.h>

#include <ZC_Config.h>
#include <Tools/ZC_uptrCreateWithErrorCheck.h>

#if defined(ZC_SDL_AUDIO)
    #include "PC/SDL/ZC_SDL_AudioStream.h"
    bool ZC_Audio::OpenAudioStream(const ZC_AudioSet& audioSet)
    {
        if (!upAudioStream) upAudioStream = ZC_uptrCreateWithErrorCheck<ZC_AudioStream, ZC_SDL_AudioStream>(audioSet);
        return upAudioStream;
    }
#elif defined(ZC_OBOE)
    #include "Android/oboe/ZC_oboe_AudioStream.h"
    bool ZC_Audio::OpenAudioStream(const ZC_AudioSet& audioSet)
    {
        if (!upAudioStream) upAudioStream = ZC_uptrCreateWithErrorCheck<ZC_AudioStream, ZC_oboe_AudioStream>(audioSet);
        return upAudioStream;
    }
#endif

void ZC_Audio::CloseAudioStream() noexcept
{
    upAudioStream = nullptr;
}

bool ZC_Audio::ReopenAudioStream()
{
    const ZC_AudioSet& setAudioStream = ZC_AudioStream::GetAudioSet();
    if (!upAudioStream && setAudioStream)
    {
        OpenAudioStream(setAudioStream);
        return true;
    }

    return false;
}