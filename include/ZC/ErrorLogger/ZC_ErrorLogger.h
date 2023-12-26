#pragma once

#include <ZC/Tools/ZC_string.h>

/*
Error logging class.
If CMakeLists.txt(ZC_DEBUG - ON) will throw the ZC_Exception().
If CMakeLists.txt(ZC_DEBUG - OFF) will save error message.
*/
class ZC_ErrorLogger
{
public:
    ZC_ErrorLogger() = delete;

    /*
    Logging the error.

    Params:
    msg - error message.
    callingFilePath - path to the file with the error.
    callingFileLine - error line number.
    */
    static void Err(const std::string& msg = "", const char* callingFilePath = nullptr, int callingFileLine = 0);

    /*
    Tells if there was an error.

    Return:
    If there was an error - true, otherwise false.
    */
    static bool WasError() noexcept
    {
        return !errorMessage.empty();
    }

    /*
    Allows to get error message.

    Return:
    If there was an error - message, otherwise nullptr.
    */
    static const char* ErrorMessage() noexcept;

    /*
    Delete error message.
    */
    static void Clear() noexcept;

private:
    static inline std::string errorMessage;
};