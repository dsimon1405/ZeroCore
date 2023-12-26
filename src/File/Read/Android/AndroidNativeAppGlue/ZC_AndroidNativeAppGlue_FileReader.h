#pragma once

#include <ZC/File/Read/ZC_FileReader.h>

#include <ZC/Video/ZC_android_app.h>

class ZC_AndroidNativeAppGlue_FileReader : public ZC_FileReader, public ZC_android_app
{
public:
    ZC_AndroidNativeAppGlue_FileReader(const char* _path);

    ~ZC_AndroidNativeAppGlue_FileReader() override;

    long Read(char* pContainer, long count) override;
    long Seek(long offset) override;
    void Close() override;
    bool Eof() override;
    long CurrentReadPosition() override;
    long Size() const override;
    long RemainingLength() override;

private:
    AAsset* file;

    bool OpenCheck() const;
};