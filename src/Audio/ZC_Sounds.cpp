#include <ZC/Audio/ZC_Sounds.h>

#include "ZC_WAVHeader.h"
#include <ZC/Tools/Container/ZC_DA.h>

bool ZC_Sounds::LoadWAV(int id, const char* path)
{
    std::unique_lock<std::shared_mutex> soundsULock(soundsSMutex);
    auto soundsIter = sounds.find(id);
    if (soundsIter != sounds.end())
    {
        ZC_ErrorLogger::Err("Already exists sound: " + id, __FILE__, __LINE__);
        return false;
    }

    soundsIter = sounds.emplace(id, ZC_SoundData()).first;

    soundsULock.unlock();
    soundsCVA.notify_all();

    ZC_ErrorLogger::Clear();
    ZC_SoundData soundData = ReadWAV(path);
    
    soundsULock.lock();
    if (soundData.GetAudioSet().frequency == 0)
    {
        sounds.erase(soundsIter);
        soundsULock.unlock();
        soundsCVA.notify_all();

        if (ZC_ErrorLogger::WasError()) return false;
        
        ZC_ErrorLogger::Err("Empty sound: " + std::string(path), __FILE__, __LINE__);
        return false;
    }

    soundsIter->second = std::move(soundData);
    soundsULock.unlock();
    soundsCVA.notify_all();

    return true;
}

bool ZC_Sounds::LoadWAV(int id, const std::string& path)
{
    return LoadWAV(id, path.c_str());
}

ZC_upSound ZC_Sounds::GetSound(int id)
{
    std::shared_lock<std::shared_mutex> soundsSLock(soundsSMutex);
    std::map<int, ZC_SoundData>::iterator soundsIter;
    soundsCVA.wait(soundsSLock, [&]
        {
            soundsIter = sounds.find(id);
            if (soundsIter == sounds.end()) return true;
            
            return soundsIter->second.GetAudioSet().frequency != 0;
        });

    if (soundsIter == sounds.end()) return nullptr;
    
    return ZC_uptrMake<ZC_Sound>(&soundsIter->second);
}

ZC_SoundData ZC_Sounds::ReadWAV(const char* path)
{
    ZC_upFileReader file = ZC_FileReader::MakeReader(path);
    if (!file) return ZC_SoundData();

    WAVHeader header;

    if (file->Seek(8) != 8)
    {
        file->Close();
        return ZC_SoundData();
    }

    static constexpr long formatSize = sizeof(header.format);
    if (file->Read(&header.format[0], formatSize) != formatSize)
    {
        file->Close();
        return ZC_SoundData();
    }

    static constexpr const char* WAVEString = "WAVE";
    if (!ConstCharEqual(WAVEString, header.format))
    {
        ZC_ErrorLogger::Err("File format != 'WAVE': " + std::string(path), __FILE__, __LINE__);
        file->Close();
        return ZC_SoundData();
    }

    if (file->Seek(4) != 4)
    {
        file->Close();
        return ZC_SoundData();
    }

    static constexpr long subchunk1SizeSize = sizeof(header.subchunk1Size);
    if (file->Read(reinterpret_cast<char*>(&header.subchunk1Size), subchunk1SizeSize) != subchunk1SizeSize)
    {
        file->Close();
        return ZC_SoundData();
    }

    if (file->Seek(2) != 2)
    {
        file->Close();
        return ZC_SoundData();
    }

    static constexpr long channelsSize = sizeof(header.channels);
    if (file->Read(reinterpret_cast<char*>(&header.channels), channelsSize) != channelsSize)
    {
        file->Close();
        return ZC_SoundData();
    }

    static constexpr long frequencySize = sizeof(header.frequency);
    if (file->Read(reinterpret_cast<char*>(&header.frequency), frequencySize) != frequencySize)
    {
        file->Close();
        return ZC_SoundData();
    }

    if (file->Seek(6) != 6)
    {
        file->Close();
        return ZC_SoundData();
    }

    static constexpr long bitsPerSampleSize = sizeof(header.bitsPerSample);
    if (file->Read(reinterpret_cast<char*>(&header.bitsPerSample), bitsPerSampleSize) != bitsPerSampleSize)
    {
        file->Close();
        return ZC_SoundData();
    }

    if (header.subchunk1Size != 16)
    {
        if (file->Seek(header.subchunk1Size - 16) != static_cast<long>(header.subchunk1Size - 16))
        {
            file->Close();
            return ZC_SoundData();
        }
    }

    static constexpr long subchunk2IdSize = sizeof(header.subchunk2Id);
    if (file->Read(&header.subchunk2Id[0], subchunk2IdSize) != subchunk2IdSize)
    {
        file->Close();
        return ZC_SoundData();
    }

    static constexpr long subchunk2SizeSize = sizeof(header.subchunk2Size);
    if (file->Read(reinterpret_cast<char*>(&header.subchunk2Size), subchunk2SizeSize) != subchunk2SizeSize)
    {
        file->Close();
        return ZC_SoundData();
    }

    static const char* dataString = "data";
    //  if subchunk2Id != 'data', miss all till find 'data'
    while (!ConstCharEqual(dataString, header.subchunk2Id))
    {
        if (file->Seek(header.subchunk2Size) != static_cast<long>(header.subchunk2Size))
        {
            file->Close();
            return ZC_SoundData();
        }

        if (file->Read(&header.subchunk2Id[0], subchunk2IdSize) != subchunk2IdSize)
        {
            file->Close();
            return ZC_SoundData();
        }

        if (file->Read(reinterpret_cast<char*>(&header.subchunk2Size), subchunk2SizeSize) != subchunk2SizeSize)
        {
            file->Close();
            return ZC_SoundData();
        }

        if (file->Eof())
        {
            ZC_ErrorLogger::Err("Can't find chunk2Id == 'data': " + std::string(path), __FILE__, __LINE__);
            file->Close();
            return ZC_SoundData();
        }
    }

    ZC_AudioSet audioSet(
        header.channels == 1 ? ZC_AudioSet::Channels::Mono : ZC_AudioSet::Channels::Stereo,
        header.frequency,
        header.bitsPerSample == 16 ? ZC_AudioSet::BitsPerSample::S16 : ZC_AudioSet::BitsPerSample::S32);

    switch (audioSet.bitsPerSample)
    {
        case ZC_AudioSet::BitsPerSample::S16:
            if (!DeleteStartNullData<int16_t>(file, header.subchunk2Size, path)) return ZC_SoundData();
            header.subchunk2Size -= header.subchunk2Size % sizeof(int16_t);
            break;
        case ZC_AudioSet::BitsPerSample::S32:
            if (!DeleteStartNullData<int32_t>(file, header.subchunk2Size, path)) return ZC_SoundData();
            header.subchunk2Size -= header.subchunk2Size % sizeof(int32_t);
            break;
    }

    ZC_DA<char> data(header.subchunk2Size);
    if (file->Read(data.pHead, header.subchunk2Size) != static_cast<long>(header.subchunk2Size))
    {
        file->Close();
        return ZC_SoundData();
    }

    ZC_ErrorLogger::Clear();
    if (!file->Eof())
    {
        ZC_ErrorLogger::Err("Read " + std::to_string(header.subchunk2Size)
            + " bytes, until the end of the file " + std::to_string(file->RemainingLength())
            + " bytes in the file: " + std::string(path), __FILE__, __LINE__);
    }

    return ZC_SoundData(std::move(data), audioSet);
}

bool ZC_Sounds::ConstCharEqual(const char* first, char* second) noexcept
{
    for (ulong i = 0; ; ++i)
    {
        if (first[i] == '\0')
        {
            return true;
        }
        if (first[i] != second[i])
        {
            return false;
        }
    }
}