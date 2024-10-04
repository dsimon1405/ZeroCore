#include <ZC/Audio/ZC_Sound.h>

#include <ZC/Audio/ZC_AudioStream.h>

ZC_Sound::ZC_Sound(const ZC_SoundData* _soundData)
    : ZC_StreamSound(_soundData)
{}

ZC_Sound::~ZC_Sound() noexcept
{
    sconGetpZC_StreamSound.Disconnect();
}

void ZC_Sound::Play()
{
    if (ZC_AudioStream::GetState() != ZC_AS__None && soundData->GetAudioSet() == ZC_AudioStream::GetAudioSet())
    {
        std::lock_guard<std::mutex> lock(soundStateMutex);
        if (soundState == ZC_SS__Pause || soundState == ZC_SS__Stop)
        {
            sconGetpZC_StreamSound = ZC_AudioStream::sGetpZC_StreamSound.Connect(ZC_Function<ZC_StreamSound*()>(&ZC_Sound::GetpZC_StreamSound, this));
        }
        soundState = ZC_SS__Play;
    }
}

void ZC_Sound::PlayLoop()
{
    if (ZC_AudioStream::GetState() != ZC_AS__None && soundData->GetAudioSet() == ZC_AudioStream::GetAudioSet())
    {
        std::lock_guard<std::mutex> lock(soundStateMutex);
        if (soundState == ZC_SS__Pause || soundState == ZC_SS__Stop)
        {
            sconGetpZC_StreamSound = ZC_AudioStream::sGetpZC_StreamSound.Connect(ZC_Function<ZC_StreamSound*()>(&ZC_Sound::GetpZC_StreamSound, this));
        }
        soundState = ZC_SS__PlayLoop;
    }
}

void ZC_Sound::Pause()
{
    std::lock_guard<std::mutex> lock(soundStateMutex);
    if (soundState == ZC_SS__Play || soundState == ZC_SS__PlayLoop)
    {
        sconGetpZC_StreamSound.Disconnect();
        soundState = ZC_SS__Pause;
    }
}

void ZC_Sound::Stop()
{
    std::lock_guard<std::mutex> lock(soundStateMutex);
    if (soundState != ZC_SS__Stop)
    {
        sconGetpZC_StreamSound.Disconnect();
        soundDataIndex = 0;
        soundState = ZC_SS__Stop;
    }
}

ZC_SoundState ZC_Sound::GetState()
{
    std::lock_guard<std::mutex> lock(soundStateMutex);
    return soundState;
}

void ZC_Sound::SetVolume(float _volume) noexcept
{
    if (_volume >= 0 && _volume <= 100.f) volume = _volume / 100.f;
}

short ZC_Sound::GetVolume() const noexcept
{
    return volume * 100;
}

ZC_StreamSound* ZC_Sound::GetpZC_StreamSound() noexcept
{
    return dynamic_cast<ZC_StreamSound*>(this);
}

ZC_upSound ZC_Sound::GetSameSound() const
{
    return ZC_uptrMake<ZC_Sound>(soundData);
}

ZC_AudioSet ZC_Sound::GetAudioSet() const noexcept
{
    return this->soundData->GetAudioSet();
}