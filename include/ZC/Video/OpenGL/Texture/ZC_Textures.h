#pragma once

#include "ZC_Texture.h"

#include <list>

class ZC_Textures
{
public:
    ZC_Textures() = delete;

    static ZC_Texture LoadTexture2D(const char* filePath, GLenum wrapS = GL_CLAMP_TO_EDGE,   // wrapS = wrapT = GL_REPEAT
        GLenum wrapT = GL_CLAMP_TO_EDGE, GLenum filterMin = GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag = GL_LINEAR);
    static ZC_Texture* LoadCubeMap(const char** filePaths);

private:
    static inline std::list<ZC_Texture> textures;
};