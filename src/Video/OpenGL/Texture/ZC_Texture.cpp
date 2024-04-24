#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Math/ZC_Math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Tools/stb_image.h>

ZC_Texture ZC_Texture::LoadTexture2D(const char* filePath, GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag)
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
    ZC_Texture texture(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data, wrapS, wrapT, filterMin, filterMag, true);
    if (ZC_ErrorLogger::WasError())
    {
        stbi_image_free(data);
    }
#ifdef ZC_PC
    stbi_image_free(data);
#endif
    return texture;
}

// ZC_Texture* ZC_Textures::LoadCubeMap(const char** filePaths)
// {
//     int width, height, channels;
//     unsigned char* datas[6];
//     for (size_t i = 0; i < 6; i++)
//     {
//         datas[i] = stbi_load(filePaths[i], &width, &height, &channels, 0);
//         if (!datas[i]) return nullptr;
//     }

//     ZC_ErrorLogger::Clear();
//     ZC_Texture texture(width, height, datas);
//     if (ZC_ErrorLogger::WasError())
//     {
//         for (size_t i = 0; i < 6; i++) stbi_image_free(datas[i]);
//         return nullptr;
//     }
// #ifdef ZC_PC
//     for (size_t i = 0; i < 6; i++) stbi_image_free(datas[i]);
// #endif
//     return &textures.emplace_back(std::move(texture));
// }

// void CreateMultisampleSet(ZC_Texture& rTexMultisample, ZC_Texture& rTexPostProcessing, int samples, int width, int height, GLenum format)
// {
//     rTexMultisample = ZC_Texture(samples, width, height, format);
//     rTexPostProcessing = ZC_Texture(0, width, height, format);
// }

ZC_Texture::ZC_Texture(GLenum _target, GLenum internalFormat, int width, int height, GLenum format, GLenum type, void* pData,
        GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag, bool needMimmap)
    : target(_target)
{
    CreateAndConfigure(internalFormat, width, height, format, type, pData, wrapS, wrapT, filterMin, filterMag, needMimmap);
}

ZC_Texture::ZC_Texture(int samples, int width, int height, GLenum internalFormat)
    : target(GL_TEXTURE_2D_MULTISAMPLE)
{
    glGenTextures(1, &id);
    glBindTexture(target, id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_TRUE);
    glBindTexture(target, 0);
}

// ZC_Texture::ZC_Texture(int width, int height, unsigned char** data)
//     : target(GL_TEXTURE_CUBE_MAP)
// {
//     glGenTextures(1, &id);
//     glBindTexture(target, id);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//     for (int i = 0; i < 6; ++i) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
//     glBindTexture(target, 0);
// }

ZC_Texture::ZC_Texture(ZC_Texture&& tex) noexcept
    : id(tex.id),
    target(tex.target)
{
    tex.id = 0;
}

ZC_Texture& ZC_Texture::operator = (ZC_Texture&& tex)
{
    id = tex.id;
    target = tex.target;
    tex.id = 0;
    return *this;
}

ZC_Texture::~ZC_Texture()
{
    if (id != 0) glDeleteTextures(1, &id);
}

void ZC_Texture::Bind()
{
    glBindTexture(target, id);
}

void ZC_Texture::Unbind()
{
    glBindTexture(target, 0);
}

void ZC_Texture::ActiveTexture(GLuint num) const
{
    glActiveTexture(GL_TEXTURE0 + num);
    glBindTexture(target, id);
}

void ZC_Texture::GenerateMimmap() const
{
    glBindTexture(target, id);
    glGenerateMipmap(target);
    glBindTexture(target, 0);
}

GLuint ZC_Texture::GetId() const noexcept
{
    return id;
}

void ZC_Texture::CreateAndConfigure(GLenum internalFormat, int width, int height, GLenum format, GLenum type, void* pData,
    GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag, bool needMimmap)
{
    glGenTextures(1, &id);
    glBindTexture(target, id);
    if (wrapS != GL_REPEAT) glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    if (wrapT != GL_REPEAT) glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
    if (filterMin != GL_NEAREST) glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filterMin);
    if (filterMag != GL_NEAREST) glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filterMag);
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, pData);
    if (needMimmap) glGenerateMipmap(target);
    glBindTexture(target, 0);
}