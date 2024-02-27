#pragma once

#include <ZC/Tools/Signal/ZC_Signal.h>
#include "ZC_StreamSound.h"
#include "ZC_Sound.h"

#include <vector>

//  Audio stream class.
class ZC_AudioStream
{
    friend class ZC_Sound;
public:
    enum State
    {
        //  The stream will play sounds.
        Active,
        //  The stream will not play sounds.
        Passive,
        //  Stream does not exist.
        Null
    };

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
    Current ZC_AudioStream::State.
    */
    static typename ZC_AudioStream::State GetState() noexcept;

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
    static inline typename ZC_AudioStream::State stateAudioStream = ZC_AudioStream::State::Null;
    static inline ZC_Signal<ZC_StreamSound*()> sGetpZC_StreamSound { false };

    template <ZC_cBitsPerSample T>
    static void FillData(void* pDataContainer, int bytesCount, std::vector<ZC_StreamSound*>& sounds);
};

template <ZC_cBitsPerSample T>
void ZC_AudioStream::FillData(void* pDataContainer, int bytesCount, std::vector<ZC_StreamSound*>& sounds)
{
    T* pData = static_cast<T*>(pDataContainer);
    int pDataSize = bytesCount / static_cast<int>(sizeof(T));
    if (stateAudioStream == ZC_AudioStream::State::Passive)
    {
        std::fill(pData, pData + pDataSize, 0);
        return;
    }
    for (int pDataIndex = 0; pDataIndex < pDataSize; ++pDataIndex)
    {
        if (sounds.size() == 0)
        {
            std::fill(pData + pDataIndex, pData + (pDataSize - pDataIndex), 0);
            return;
        }
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