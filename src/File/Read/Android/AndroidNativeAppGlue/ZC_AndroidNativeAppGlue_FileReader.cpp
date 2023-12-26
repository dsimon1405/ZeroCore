#include "ZC_AndroidNativeAppGlue_FileReader.h"

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_AndroidNativeAppGlue_FileReader::ZC_AndroidNativeAppGlue_FileReader(const char* _path)
    : ZC_FileReader(std::string(_path)),
    file(AAssetManager_open(pAndroidApp->activity->assetManager, _path, AASSET_MODE_BUFFER))
{
    OpenCheck();
}

ZC_AndroidNativeAppGlue_FileReader::~ZC_AndroidNativeAppGlue_FileReader()
{
    if (file)
    {
        AAsset_close(file);
        file = nullptr;
    }
}

long ZC_AndroidNativeAppGlue_FileReader::Read(char* pContainer, long count)
{
    if (!OpenCheck()) return 0;
    if (count <= -1)
    {
        ZC_ErrorLogger::Err("Try to read " + std::to_string(count) + " < 1 bytes from file: " + std::string(path), __FILE__, __LINE__);
        return -1;
    }
    if (AAsset_getRemainingLength(file) == 0)
    {
        ZC_ErrorLogger::Err("End of file reached: " + std::string(path), __FILE__, __LINE__);
        return -1;
    }

    int readCount = AAsset_read(file, reinterpret_cast<void*>(pContainer), count);
    if (readCount == 0 && count != 0)
    {
        ZC_ErrorLogger::Err("Fail read file: " + std::string(path), __FILE__, __LINE__);
        return -1;
    }

    ZC_ErrorLogger::Clear();
    return count;
}

long ZC_AndroidNativeAppGlue_FileReader::Seek(long offset)
{
    if (!OpenCheck()) return 0;

    long remainingLength = AAsset_getRemainingLength(file);
    if ((offset > 0 && offset > remainingLength) || (offset < 0 && offset * -1 > AAsset_getLength(file) - remainingLength))
    {
        ZC_ErrorLogger::Err("offset greater than length to file boundary: " + std::string(path), __FILE__, __LINE__);
        return 0;
    }

    long seekResult = AAsset_seek(file, offset, SEEK_CUR);
    if (seekResult == -1)
    {
        ZC_ErrorLogger::Err("Fail seek file: " + std::string(path), __FILE__, __LINE__);
        return 0;
    }

    ZC_ErrorLogger::Clear();
    return offset;
}

void ZC_AndroidNativeAppGlue_FileReader::Close()
{
    if (file)
    {
        AAsset_close(file);
        file = nullptr;
    }
}

bool ZC_AndroidNativeAppGlue_FileReader::Eof()
{
    if (!OpenCheck()) return true;
    return AAsset_getRemainingLength(file) == 0;
}

long ZC_AndroidNativeAppGlue_FileReader::CurrentReadPosition()
 {
     if (!OpenCheck()) return -1;
     return AAsset_getLength(file) - AAsset_getRemainingLength(file);
 }

long ZC_AndroidNativeAppGlue_FileReader::Size() const
{
    if (!OpenCheck()) return -1;
    return AAsset_getLength(file);
}

long ZC_AndroidNativeAppGlue_FileReader::RemainingLength()
{
    if (!OpenCheck()) return -1;
    return AAsset_getRemainingLength(file);
}

bool ZC_AndroidNativeAppGlue_FileReader::OpenCheck() const
{
    if (!file)
    {
        ZC_ErrorLogger::Err("The file is not open: " + std::string(path), __FILE__, __LINE__);
        return false;
    }
    return true;
}