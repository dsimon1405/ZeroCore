#pragma once

#include <ZC/File/Read/ZC_FileReader.h>

#include <fstream>

class ZC_PC_FileReader : public ZC_FileReader
{
public:
    ZC_PC_FileReader(const char* _path);

    ~ZC_PC_FileReader() override;
    
    long Read(char* pContainer, long count) override;
    long Seek(long offset) override;
    void Close() override;
    bool Eof() override;
    long CurrentReadPosition() override;
    long Size() const override;
    long RemainingLength() override;

private:
    std::ifstream file;
    const long size;

    bool OpenCheck() const;
    long CalculateSize();
};