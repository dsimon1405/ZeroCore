#pragma once

#include <ZC/Tools/ZC_string.h>
#include <ZC_Config.h>

#ifdef ZC_PC
#include <filesystem>

typedef typename std::filesystem::__cxx11::path ZC_FSPath;
static const ZC_FSPath ZC_binDirPath = std::filesystem::current_path();
static const ZC_FSPath ZC_assetsDirPath = ZC_FSPath(ZC_binDirPath).append("assets");
static const ZC_FSPath ZC_ZCDirPath = ZC_FSPath(ZC_assetsDirPath).append("ZC");
#endif

// auto ZC_MakePath(const char* path)
// {
// #ifdef ZC_PC
//     return ZC_FSPath(ZC_binDirPath).append(path).string();
// #elif defined ZC_ANDROID
//     return path;
// #endif
// }