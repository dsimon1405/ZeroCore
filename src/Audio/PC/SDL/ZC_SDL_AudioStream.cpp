#include "ZC_SDL_AudioStream.h"

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Container/ZC_DA.h>

#include <SDL3/SDL_init.h>

ZC_SDL_AudioStream::ZC_SDL_AudioStream(const ZC_AudioSet& _audioSet)
    : ZC_AudioStream(_audioSet)
{
	static bool sdlAudioInited = false;
	if (!sdlAudioInited)
	{
    	if (SDL_Init(SDL_INIT_AUDIO) != 0)
    	{
        	ZC_ErrorLogger::Err("SDL_Init(SDL_INIT_AUDIO) fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
            return;
    	}
		sdlAudioInited = true;
	}

    if (_audioSet.frequency <= 0)
    {
        ZC_ErrorLogger::Err("_audioSet.frequency must be > 0!", __FILE__, __LINE__);
        return;
    }
    
    SDL_AudioSpec spec;
    spec.channels = audioSet.channels;
    spec.freq = audioSet.frequency;
    switch (audioSet.bitsPerSample)
    {
        case ZC_AudioSet::BitsPerSample::S16:
            spec.format = SDL_AUDIO_S16;
            break;
        case ZC_AudioSet::BitsPerSample::S32:
            spec.format = SDL_AUDIO_S32;
            break;
    }

    audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, &spec, ZC_SDL_AudioStream::AudioCallback, nullptr);
    if (!audioStream)
    {
        ZC_ErrorLogger::Err("SDL_OpenAudioDeviceStream() fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
        return;
    }
    
    SDL_AudioDeviceID device = SDL_GetAudioStreamDevice(audioStream);
    if (device == 0)
    {
        ZC_ErrorLogger::Err("SDL_GetAudioStreamDevice() fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
        return;
    }

    int ress = SDL_ResumeAudioDevice(device);
    if (ress != 0)
    {
        ZC_ErrorLogger::Err("SDL_ResumeAudioDevice() fail: " + std::string(SDL_GetError()), __FILE__, __LINE__);
        return;
    }
    
    ZC_ErrorLogger::Clear();
}

ZC_SDL_AudioStream::~ZC_SDL_AudioStream()
{
    SDL_DestroyAudioStream(audioStream);
}

void SDLCALL ZC_SDL_AudioStream::AudioCallback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount)
{
    if (additional_amount <= 0)
    {
        ZC_ErrorLogger::Err("ZC_SDL_AudioStream::MyAudioCallback(){additional_amount < 0}: "
            + std::string(SDL_GetError()), __FILE__, __LINE__);
        return;
    }
#if _WIN32
    ZC_DynamicArray<char> dynamicData(static_cast<unsigned long>(additional_amount));
    char* pData = dynamicData.pArray;
#else
    char pData[additional_amount];
#endif
    GetStreamData(static_cast<void*>(pData), additional_amount);
    SDL_PutAudioStreamData(stream, static_cast<const void*>(pData), additional_amount);
}