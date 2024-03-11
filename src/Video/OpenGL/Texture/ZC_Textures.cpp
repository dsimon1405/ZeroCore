#include "ZC/Video/OpenGL/Texture/ZC_Textures.h"

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Math/ZC_Math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Tools/stb_image.h>


ZC_Texture ZC_Textures::LoadTexture2D(const char* filePath, GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag)
{
    int width, height, channels;
    uchar* data = stbi_load(filePath, &width, &height, &channels, 0);
    
    // if (!data) return nullptr;

    // auto size = width * height *channels;
    // auto newSize = size + size / 3;
    // unsigned char* test = new unsigned char[newSize];
    // for (int i = 0, pixelI = 1; i < newSize; ++pixelI)
    // {
    //         test[i++] = 128;
    //         test[i++] = 128;
    //         test[i++] = 128;
    //     if (pixelI <= width || pixelI > width * width - width || pixelI % width == 0 || pixelI % width == 1)
    //     {
    //     test[i++] = 0;
    //     }
    //     else
    //     {
    //     test[i++] = 1;
    //     }
    //     // test[i++] = data[dataI] != 128 ? 0 : 255;
    // }
    // channels = 4;

    GLenum format = 0;
    switch (channels)
    {
    case 1: format = GL_RED; break;
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    default: assert(false);
    }

    ZC_ErrorLogger::Clear();
    // ZC_Texture texture(GL_TEXTURE_2D, format, width, height, test, wrapS, wrapT, filterMin, filterMag);
    ZC_Texture texture(GL_TEXTURE_2D, format, width, height, data, wrapS, wrapT, filterMin, filterMag);
    if (ZC_ErrorLogger::WasError())
    {
        // stbi_image_free(data);
    }
#ifdef ZC_PC
    // stbi_image_free(data);
#endif
    return texture;
}

ZC_Texture* ZC_Textures::LoadCubeMap(const char** filePaths)
{
    int width, height, channels;
    unsigned char* datas[6];
    for (size_t i = 0; i < 6; i++)
    {
        datas[i] = stbi_load(filePaths[i], &width, &height, &channels, 0);
        if (!datas[i]) return nullptr;
    }

    ZC_ErrorLogger::Clear();
    ZC_Texture texture(GL_TEXTURE_CUBE_MAP, width, height, datas);
    if (ZC_ErrorLogger::WasError())
    {
        for (size_t i = 0; i < 6; i++) stbi_image_free(datas[i]);
        return nullptr;
    }
#ifdef ZC_PC
    for (size_t i = 0; i < 6; i++) stbi_image_free(datas[i]);
#endif
    return &textures.emplace_back(std::move(texture));
}
