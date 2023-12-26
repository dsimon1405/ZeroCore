#pragma once

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/ZC_uptr.h>
#include <ZC/Tools/ZC_string.h>

class ZC_FileReader;
using ZC_upFileReader = ZC_uptr<ZC_FileReader>;

/*
File read wrapper.
*/
class ZC_FileReader
{
public:
    ZC_FileReader() = delete;

    ZC_FileReader(const ZC_FileReader&) = delete;
    ZC_FileReader& operator=(const ZC_FileReader&) = delete;
    ZC_FileReader(ZC_FileReader&&) = delete;
    ZC_FileReader& operator=(ZC_FileReader&&) = delete;

    virtual ~ZC_FileReader() = default;

    /*
    Create ZC_FileReader.

    Params:
    path - relative path from ZC_ASSETS_PATH to the file (CMakeLists.txt ZC_ASSETS_PATH - for more information)

    Return:
    If ZC_FileReader created - ZC_uptr<ZC_FileReader>, otherwise nullptr (ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    static ZC_upFileReader MakeReader(const char* path);

    /*
    Read a sequence of bytes from the current read position.

    Params:
    pContainer - pointer to the element in which the bytes will be written.
    count - the number of bytes that will be read from the file (the read pointer will be moved to this value or to the end of the file).

    Return:
    Number of bytes read or 0 in case of error (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    virtual long Read(char* pContainer, long count) = 0;

    /*
    Move read pointer. If the move is outside the file range, the move pointer will not move.

    Params:
    offset - value to move the pointer to.

    Return:
    The offset to which the pointer was moved. If the move is outside the file's range, it will return 0
    in that case ZC_ErrorLogger::ErrorMessage() - for more information.
    */
    virtual long Seek(long offset) = 0;

    /*
    Close file.
    */
    virtual void Close() = 0;

    /*
    Checking whether the read pointer has reached the end of the file.

    Return:
    At the end of the file - true, otherwise - false. Checking for a closed(not open) file will return true and generate ZC_ErrorLogger::ErrorMessage().
    */
    virtual bool Eof() = 0;

    /*
    Help to know current read position.

    Return:
    Current reading position or -1 in case of error (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
    */
   virtual long CurrentReadPosition() = 0;

    /*
    File lenth in bytes.

    Return:
    Full file length or -1 in case of error (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    virtual long Size() const = 0;

    /*
    Length to files end.

    Return:
    Length to the files end or -1 in case of error (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    virtual long RemainingLength() = 0;

protected:
    std::string path;

    ZC_FileReader(std::string&& _path) noexcept;
};