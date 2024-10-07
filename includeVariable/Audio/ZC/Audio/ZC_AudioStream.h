#pragma once

#include <ZC/Tools/Signal/ZC_Signal.h>
#include "ZC_StreamSound.h"
#include "ZC_Sound.h"

#include <vector>

enum ZC_AS_State
{
    ZC_AS__None,        //  Stream does not exist.
    ZC_AS__Active,     //  The stream will play sounds.
    ZC_AS__Passive     //  The stream will not play sounds.
};

//  Audio stream class.
class ZC_AudioStream
{
    friend class ZC_Sound;
public:
    ZC_AudioStream(const ZC_AudioStream&) = delete;
    ZC_AudioStream& operator = (const ZC_AudioStream&) = delete;
    ZC_AudioStream(ZC_AudioStream&&) = delete;
    ZC_AudioStream& operator = (ZC_AudioStream&&) = delete;

    virtual ~ZC_AudioStream() = default;

    //  Make the audio stream active if it exists.
    static void MakeActive() noexcept;

    //  Make the audio stream passive if it exists.
    static void MakePassive() noexcept;

    /*
    Helps to get the state of an audio stream.

    Return:
    Current ZC_AudioStream::ZC_AS_State.
    */
    static ZC_AS_State GetState() noexcept;

    /*
    Helps to get the set of an audio stream.

    Return:
    ZC_AudioSet.
    */
    static const ZC_AudioSet& GetAudioSet() noexcept;

protected:
    static inline ZC_AudioSet audioSet;

    ZC_AudioStream() = default;
    ZC_AudioStream(const ZC_AudioSet& _audioSet) noexcept;

    static void GetStreamData(void* pDataContainer, int bytesCount);

private:
    static inline ZC_AS_State stateAudioStream = ZC_AS__None;
    static inline ZC_Signal<ZC_StreamSound*()> sGetpZC_StreamSound { false };

    template <ZC_cBitsPerSample T>
    static void FillData(void* pDataContainer, int bytesCount, std::vector<ZC_StreamSound*>& sounds);
};

template <ZC_cBitsPerSample T>
void ZC_AudioStream::FillData(void* pDataContainer, int bytesCount, std::vector<ZC_StreamSound*>& sounds)
{
    T* pData = static_cast<T*>(pDataContainer);
    int pDataSize = bytesCount / static_cast<int>(sizeof(T));
    if (stateAudioStream == ZC_AS__Passive)
    {
        std::fill(pData, pData + pDataSize, 0);
        return;
    }
    for (T pDataIndex = 0; pDataIndex < pDataSize; ++pDataIndex)
    {
        if (sounds.size() == 0)
        {
            std::fill(pData + pDataIndex, pData + pDataSize, 0);
            return;
        }
        else
        {
            T divisor = sounds.size() == 1 ? static_cast<T>(2) : static_cast<T>(sounds.size());
            T result = 0;
            for (auto soundsIter = sounds.begin(); soundsIter != sounds.end();)
            {
                T data = 0;
                soundsIter = (*soundsIter)->Pop(data) ? ++soundsIter : sounds.erase(soundsIter);
                result += data / divisor;
            }
            pData[pDataIndex] = result;
        }
    }
}