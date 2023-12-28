#pragma once

#include <ZC/Tools/ZC_string.h>
#include <ZC_Config.h>

#ifdef ZC_PC
#include <filesystem>
#endif

auto ZC_MakePath(const char* path)
{
#ifdef ZC_PC
    return std::filesystem::current_path().append(path).string();
#elif defined ZC_ANDROID
    return path;
#endif
}