#include <ZC/ErrorLogger/ZC_MakeErrorString.h>

#include <sstream>

std::string ZC_MakeErrorString(const std::string& msg, const char* callingFilePath, int callingFileLine)
{
    std::stringstream str;
    str << "\n=> Error: " << std::move(msg);
    if (callingFilePath) str << "\n=> File: " << callingFilePath;
    callingFileLine != 0 ? str << "\n=> Line: " << callingFileLine << "\n" : str << "\n";
    return str.str();
}