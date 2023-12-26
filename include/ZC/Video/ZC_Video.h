#pragma once

#include "ZC_Window.h"
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/ZC_uptr.h>

using ZC_upWindow = ZC_uptr<ZC_Window>;

class ZC_Video
{
public:
    ZC_Video() = delete;

    /*
    Create window. Parameters have effect only in ZC_PC build.

    Params:
    border - on false - fullscrean window without border(ignore all next parameters), on true - window with border (if width <= 0 or height <= 0 fullscreen).
    width - window with border width.
    height - window with border height.
    name - window on border name.

    Return:
    On success unique pointer of ZC_Window, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    static ZC_upWindow MakeWindow(bool border = false, int width = 0, int height = 0, const char* name = "");
};