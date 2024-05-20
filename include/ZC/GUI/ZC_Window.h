#pragma once

#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>
#include <ZC/Tools/ZC_uptr.h>

struct ZC_Window
{
    unsigned int width,
        height;
    static inline ZC_uptr<ZC_Texture> texBackground;
};