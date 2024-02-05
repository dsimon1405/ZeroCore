#include <ZC/Audio/ZC_SoundData.h>

ZC_SoundData::ZC_SoundData(ZC_DA<char>&& _data, const ZC_AudioSet& _audioSet) noexcept
  : data(std::move(_data)),
  audioSet(_audioSet)
{}

ZC_SoundData::ZC_SoundData(ZC_SoundData&& soundData) noexcept
  : ZC_SoundData(std::move(soundData.data), soundData.audioSet)
{
  soundData.audioSet.frequency = 0;
}

ZC_SoundData& ZC_SoundData::operator = (ZC_SoundData&& soundData)
{
  data = std::move(soundData.data);
  audioSet = soundData.audioSet;

  soundData.audioSet.frequency = 0;
  return *this;
}

ZC_AudioSet ZC_SoundData::GetAudioSet() const noexcept
{
  return audioSet;
}