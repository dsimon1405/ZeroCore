#include <ZC/Audio/ZC_AudioStream.h>

void ZC_AudioStream::MakeActive() noexcept
{
    if (stateAudioStream == ZC_AudioStream::State::Passive) stateAudioStream = ZC_AudioStream::State::Active;
}

void ZC_AudioStream::MakePassive() noexcept
{
    if (stateAudioStream == ZC_AudioStream::State::Active) stateAudioStream = ZC_AudioStream::State::Passive;
}

typename ZC_AudioStream::State ZC_AudioStream::GetState() noexcept
{
    return stateAudioStream;
}

const ZC_AudioSet& ZC_AudioStream::GetAudioSet() noexcept
{
    return audioSet;
}

ZC_AudioStream::ZC_AudioStream(const ZC_AudioSet& _audioSet) noexcept
{
    stateAudioStream = ZC_AudioStream::State::Active;
    audioSet = _audioSet;
}

void ZC_AudioStream::GetStreamData(void* pDataContainer, int bytesCount)
{
    std::vector<ZC_StreamSound*> sounds;
    sGetpZC_StreamSound(sounds);
    
    switch (audioSet.bitsPerSample)
    {
        case ZC_AudioSet::BitsPerSample::S16:
            FillData<int16_t>(pDataContainer, bytesCount, sounds);
            break;
        case ZC_AudioSet::BitsPerSample::S32:
            FillData<int32_t>(pDataContainer, bytesCount, sounds);
            break;
    }
}