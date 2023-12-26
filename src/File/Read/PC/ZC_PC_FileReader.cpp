#include "ZC_PC_FileReader.h"

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC_Config.h>

#include <filesystem>

ZC_PC_FileReader::ZC_PC_FileReader(const char* _path)
    : ZC_FileReader(std::filesystem::current_path().append(ZC_ASSETS_PATH).append(_path).string()),
    file(path, std::ios::binary),
    size(CalculateSize())
{}

ZC_PC_FileReader::~ZC_PC_FileReader()
{
    file.close();
}

long ZC_PC_FileReader::Read(char* pContainer, long count)
{
    if (!OpenCheck()) return 0;
    if (count <= 0)
    {
        ZC_ErrorLogger::Err("Try to read " + std::to_string(count) + " < 1 bytes from file: " + std::string(path), __FILE__, __LINE__);
        return -1;
    }
    
    long remainingSize = size - static_cast<long>(file.tellg());
    if (remainingSize == 0)
    {
        ZC_ErrorLogger::Err("End of file reached: " + std::string(path), __FILE__, __LINE__);
        return -1;
    }

    count = count > remainingSize ? remainingSize : count;
    file.read(pContainer, count);
    if (file.fail())
    {
        ZC_ErrorLogger::Err("Fail read file: " + std::string(path), __FILE__, __LINE__);
        return -1;
    }
    
    ZC_ErrorLogger::Clear();
    return count;
}

long ZC_PC_FileReader::Seek(long offset)
{
    if (!OpenCheck()) return 0;

    long remainingLength = size - static_cast<long>(file.tellg());
    if ((offset > 0 && offset > remainingLength) || (offset < 0 && -offset > static_cast<long>(size) - remainingLength))
    {
        ZC_ErrorLogger::Err("offset greater than length to file boundary: " + std::string(path), __FILE__, __LINE__);
        return 0;
    }
    
    file.seekg(offset, file.cur);
    if (file.fail())
    {
        ZC_ErrorLogger::Err("Fail seek file: " + std::string(path), __FILE__, __LINE__);
        return 0;
    }

    ZC_ErrorLogger::Clear();
    return offset;
}

void ZC_PC_FileReader::Close()
{
    file.close();
}

bool ZC_PC_FileReader::Eof()
{
    if (!OpenCheck()) return true;
    return size - file.tellg() == 0;
}

long ZC_PC_FileReader::CurrentReadPosition()
{
    if (!OpenCheck()) return -1;
    return static_cast<long>(file.tellg());
}

long ZC_PC_FileReader::Size() const
{
    if (!OpenCheck()) return -1;
    return size;
}

long ZC_PC_FileReader::RemainingLength()
{
    if (!OpenCheck()) return -1;
    return size - static_cast<long>(file.tellg());
}

bool ZC_PC_FileReader::OpenCheck() const
{
    if (!file.is_open())
    {
        ZC_ErrorLogger::Err("The file is not open: " + std::string(path), __FILE__, __LINE__);
        return false;
    }
    return true;
}

long ZC_PC_FileReader::CalculateSize()
{
    ZC_ErrorLogger::Clear();
    if (!OpenCheck()) return -1;

    long currentPosition = static_cast<long>(file.tellg());
    file.seekg(0, file.end);
    if (file.fail())
    {
        ZC_ErrorLogger::Err("Fail seek file: " + std::string(path), __FILE__, __LINE__);
        return -1;
    }

    long fullLength = static_cast<long>(file.tellg());
    file.seekg(currentPosition, file.beg);
    if (file.fail())
    {
        ZC_ErrorLogger::Err("Fail seek file: " + std::string(path), __FILE__, __LINE__);
        return -1;
    }

    return fullLength;
}