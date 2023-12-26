#pragma once

#include <ZC/Tools/ZC_string.h>

/*
Create error string.

Params:
msg - error message.
callingFilePath - path to the calling file (optional, required to log an error, can be use __FILE__).
callingFileLine - line in the calling file (optional, required to log an error, can be use __LINE__).

Return:
A string created from the parameters. 
*/
std::string ZC_MakeErrorString(const std::string& msg, const char* callingFilePath, int callingFileLine);