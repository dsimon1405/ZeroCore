#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

#include <ZC/ErrorLogger/ZC_MakeErrorString.h>

void ZC_ErrorLogger::Err(const std::string& msg, const char* callingFilePath, int callingFileLine)
{
    errorMessage = ZC_MakeErrorString(msg, callingFilePath, callingFileLine);
}