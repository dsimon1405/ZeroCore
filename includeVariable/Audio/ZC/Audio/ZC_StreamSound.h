#pragma once

#include "ZC_SoundData.h"
#include <ZC/Tools/Signal/ZC_SConnection.h>

#include <mutex>

class ZC_StreamSound
{
public:
    enum SoundState
    {
        Play,
        PlayLoop,
        Pause,
        Stop
    };

    ZC_StreamSound() = delete;

    ZC_StreamSound(ZC_StreamSound&&) = delete;
    ZC_StreamSound& operator = (const ZC_StreamSound&) = delete;
    ZC_StreamSound(const ZC_StreamSound&) = delete;
    ZC_StreamSound& operator = (ZC_StreamSound&&) = delete;

    virtual ~ZC_StreamSound() = default;

    template <ZC_cBitsPerSample T>
    bool Pop(T& value) noexcept;

protected:
    SoundState soundState = SoundState::Stop;
    const ZC_SoundData* soundData;
    unsigned long soundDataIndex = 0;
    float volume = 1.f;
    ZC_SConnection sconGetpZC_StreamSound;
    std::mutex soundStateMutex;

    ZC_StreamSound(const ZC_SoundData* _soundData) noexcept;
};

template <ZC_cBitsPerSample T>
bool ZC_StreamSound::Pop(T& value) noexcept
{
    unsigned long soundDataSize = soundData->Size<T>();
    std::lock_guard<std::mutex> lock(soundStateMutex);
    if (soundState == SoundState::Stop || soundState == SoundState::Pause || soundDataSize == 0)
    {
        value = 0;
        return false;
    }

    value = static_cast<T>(static_cast<float>(soundData->GetValue<T>(soundDataIndex++)) * volume);
    if (soundDataIndex >= soundDataSize)
    {
        soundDataIndex = 0;
        if (soundState != SoundState::PlayLoop)
        {
            soundState = SoundState::Stop;
            sconGetpZC_StreamSound.Disconnect();
            return false;
        }
    }

    return true;
}