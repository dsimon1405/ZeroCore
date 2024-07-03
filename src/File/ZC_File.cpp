#include <ZC/File/ZC_File.h>

ZC_Path::ZC_binDirPath = std::filesystem::current_path();
ZC_Path::ZC_assetsDirPath = ZC_FSPath(ZC_binDirPath).append("assets");
ZC_Path::ZC_ZCDirPath = ZC_FSPath(ZC_assetsDirPath).append("ZC");