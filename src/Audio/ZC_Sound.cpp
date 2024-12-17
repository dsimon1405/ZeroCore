#include <ZC/Audio/ZC_Sound.h>

#include <ZC/Audio/ZC_AudioStream.h>

ZC_Sound::ZC_Sound(const ZC_sptr<ZC_StreamSound>& _spStream_sound)
    : spStream_sound(_spStream_sound)
{
    if (spStream_sound) spStream_sound->p_scon_GetpZC_StreamSound = &sconGetpZC_StreamSound;
}

ZC_Sound::ZC_Sound(ZC_Sound&& s)
    : spStream_sound(std::move(s.spStream_sound))
{
    if (spStream_sound)
    {
        spStream_sound->p_scon_GetpZC_StreamSound = &sconGetpZC_StreamSound;
        if (s.sconGetpZC_StreamSound.IsConnected())
        {
            s.sconGetpZC_StreamSound.Disconnect();
            sconGetpZC_StreamSound = ZC_AudioStream::sGetpZC_StreamSound.Connect(ZC_Function<ZC_sptr<ZC_StreamSound>()>(&ZC_Sound::GetpZC_StreamSound, this));
        }
    }
}

ZC_Sound& ZC_Sound::operator = (ZC_Sound&& s)
{
    spStream_sound = std::move(s.spStream_sound);
    if (spStream_sound)
    {
        spStream_sound->p_scon_GetpZC_StreamSound = &sconGetpZC_StreamSound;
        if (s.sconGetpZC_StreamSound.IsConnected())
        {
            s.sconGetpZC_StreamSound.Disconnect();
            sconGetpZC_StreamSound = ZC_AudioStream::sGetpZC_StreamSound.Connect(ZC_Function<ZC_sptr<ZC_StreamSound>()>(&ZC_Sound::GetpZC_StreamSound, this));
        }
    }
    return *this;
}

ZC_Sound::~ZC_Sound()
{
    Stop();
    sconGetpZC_StreamSound.Disconnect();
}

void ZC_Sound::Play()
{
    if (spStream_sound && ZC_AudioStream::GetState() != ZC_AS__None && spStream_sound->soundData->GetAudioSet() == ZC_AudioStream::GetAudioSet())
    {
        std::lock_guard<std::mutex> lock(spStream_sound->soundStateMutex);
        if (spStream_sound->soundState == ZC_SS__Pause || spStream_sound->soundState == ZC_SS__Stop)
        {
            sconGetpZC_StreamSound = ZC_AudioStream::sGetpZC_StreamSound.Connect(ZC_Function<ZC_sptr<ZC_StreamSound>()>(&ZC_Sound::GetpZC_StreamSound, this));
        }
        spStream_sound->soundState = ZC_SS__Play;
    }
}

void ZC_Sound::PlayLoop()
{
    if (spStream_sound && ZC_AudioStream::GetState() != ZC_AS__None && spStream_sound->soundData->GetAudioSet() == ZC_AudioStream::GetAudioSet())
    {
        std::lock_guard<std::mutex> lock(spStream_sound->soundStateMutex);
        if (spStream_sound->soundState == ZC_SS__Pause || spStream_sound->soundState == ZC_SS__Stop)
        {
            sconGetpZC_StreamSound = ZC_AudioStream::sGetpZC_StreamSound.Connect(ZC_Function<ZC_sptr<ZC_StreamSound>()>(&ZC_Sound::GetpZC_StreamSound, this));
        }
        spStream_sound->soundState = ZC_SS__PlayLoop;
    }
}

void ZC_Sound::Pause()
{
    if (!spStream_sound) return;

    std::lock_guard<std::mutex> lock(spStream_sound->soundStateMutex);
    if (spStream_sound->soundState == ZC_SS__Play || spStream_sound->soundState == ZC_SS__PlayLoop)
    {
        sconGetpZC_StreamSound.Disconnect();
        spStream_sound->soundState = ZC_SS__Pause;
    }
}

void ZC_Sound::Stop()
{
    if (!spStream_sound) return;

    std::lock_guard<std::mutex> lock(spStream_sound->soundStateMutex);
    if (spStream_sound->soundState != ZC_SS__Stop)
    {
        sconGetpZC_StreamSound.Disconnect();
        spStream_sound->soundDataIndex = 0;
        spStream_sound->soundState = ZC_SS__Stop;
    }
}

ZC_SoundState ZC_Sound::GetState()
{
    if (!spStream_sound) return ZC_SS__Stop;

    std::lock_guard<std::mutex> lock(spStream_sound->soundStateMutex);
    return spStream_sound->soundState;
}

void ZC_Sound::SetVolume(float _volume) noexcept
{
    if (spStream_sound && _volume >= 0 && _volume <= 100.f) spStream_sound->volume = _volume / 100.f;
}

short ZC_Sound::GetVolume() const noexcept
{
    return spStream_sound ? spStream_sound->volume * 100 : 0;
}

ZC_Sound ZC_Sound::GetSameSound() const
{
    return ZC_Sound(spStream_sound ? ZC_sptrMake<ZC_StreamSound>(spStream_sound->soundData) : nullptr);
}

const ZC_AudioSet* ZC_Sound::GetAudioSet() const noexcept
{
    return spStream_sound ? &(spStream_sound->soundData->GetAudioSet()) : nullptr;
}

ZC_sptr<ZC_StreamSound> ZC_Sound::GetpZC_StreamSound()
{
    return spStream_sound;
}