#pragma once

#include "ZC_AudioSet.h"
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Container/ZC_DynamicArray.h>

#include <concepts>

template <typename T>
concept ZC_cBitsPerSample = std::same_as<T, int16_t> || std::same_as<T, int32_t>;

class ZC_SoundData
{
public:
    
    ZC_SoundData() = default;
    ZC_SoundData(ZC_DynamicArray<char>&& _data, const ZC_AudioSet& _audioSet) noexcept;

    ZC_SoundData(const ZC_SoundData&) = delete;
    ZC_SoundData& operator = (const ZC_SoundData&) = delete;

    ZC_SoundData(ZC_SoundData&& soundData) noexcept;
    ZC_SoundData& operator = (ZC_SoundData&& soundData);

    ~ZC_SoundData() = default;

    template <ZC_cBitsPerSample T>
    unsigned long Size() const;

    template <ZC_cBitsPerSample T>
    T GetValue(unsigned long index) const;

    ZC_AudioSet GetAudioSet() const noexcept;

private:
    ZC_DynamicArray<char> data;
    ZC_AudioSet audioSet;
};

template <ZC_cBitsPerSample T>
unsigned long ZC_SoundData::Size() const
{
    return data.size / sizeof(T);
}

template <ZC_cBitsPerSample T>
T ZC_SoundData::GetValue(unsigned long index) const
{
    unsigned long size = Size<T>();
    if (index >= size)
    {
        ZC_ErrorLogger::Err("ZC_SoundData out of range exception!", __FILE__, __LINE__);
        return 0;
    }

    return (reinterpret_cast<T*>(data.pHead))[index];
}