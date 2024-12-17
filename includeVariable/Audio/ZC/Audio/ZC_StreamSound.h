#pragma once

#include "ZC_SoundData.h"
#include <ZC/Tools/Signal/ZC_SConnection.h>

#include <mutex>

enum ZC_SoundState
{
    ZC_SS__Play,
    ZC_SS__PlayLoop,
    ZC_SS__Pause,
    ZC_SS__Stop
};

class ZC_StreamSound
{
public:
    std::mutex soundStateMutex;
    const ZC_SoundData* soundData;
    ZC_SoundState soundState = ZC_SS__Stop;
    unsigned long soundDataIndex = 0;
    float volume = 1.f;
    ZC_SConnection* p_scon_GetpZC_StreamSound = nullptr;

    ZC_StreamSound(const ZC_SoundData* _soundData);

    ZC_StreamSound(ZC_StreamSound&&) = delete;
    ZC_StreamSound& operator = (const ZC_StreamSound&) = delete;
    ZC_StreamSound(const ZC_StreamSound&) = delete;
    ZC_StreamSound& operator = (ZC_StreamSound&&) = delete;

    template <ZC_cBitsPerSample T>
    bool Pop(T& value) noexcept;
};

template <ZC_cBitsPerSample T>
bool ZC_StreamSound::Pop(T& value) noexcept
{
    std::lock_guard<std::mutex> lock(soundStateMutex);

    unsigned long soundDataSize = soundData->Size<T>();
    if (soundState == ZC_SS__Stop || soundState == ZC_SS__Pause || soundDataSize == 0)
    {
        value = 0;
        return false;
    }

    value = static_cast<T>(static_cast<float>(soundData->GetValue<T>(soundDataIndex++)) * volume);
    if (soundDataIndex >= soundDataSize)
    {
        soundDataIndex = 0;
        if (soundState != ZC_SS__PlayLoop)
        {
            soundState = ZC_SS__Stop;
            p_scon_GetpZC_StreamSound->Disconnect();   //  sconGetpZC_StreamSound must be setted by ZC_Sound
            return false;
        }
    }

    return true;
}