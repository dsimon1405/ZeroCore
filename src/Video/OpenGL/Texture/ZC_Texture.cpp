#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Math/ZC_Math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Tools/stb_image.h>

ZC_Texture ZC_Texture::LoadTexture2D(const char* filePath, GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag)
{
    int width, height, channels;
    uchar* pData = stbi_load(filePath, &width, &height, &channels, 0);
    
    // if (!pData) return nullptr;

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
    //     // test[i++] = pData[dataI] != 128 ? 0 : 255;
    // }
    // channels = 4;

    GLenum internalFormat = 0,
        format = 0;
    switch (channels)
    {
    case 1: { internalFormat = GL_R8; format = GL_RED; } break;
    case 3: { internalFormat = GL_RGB8; format = GL_RGB; } break;
    case 4: { internalFormat = GL_RGBA8; format = GL_RGBA; } break;
    default: assert(false);
    }

    ZC_ErrorLogger::Clear();
    ZC_Texture texture = TextureStorage2DFill(internalFormat, width, height, pData, format, GL_UNSIGNED_BYTE, true, wrapS, wrapT, filterMin, filterMag);
    if (ZC_ErrorLogger::WasError())
    {
        stbi_image_free(pData);
    }
#ifdef ZC_PC
    stbi_image_free(pData);
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

ZC_Texture ZC_Texture::TextureStorage2D(GLenum internalFormat, GLsizei width, GLsizei height, bool mimmap, GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag)
{
    ZC_Texture tex(GL_TEXTURE_2D);
    if (wrapS != GL_REPEAT) glTextureParameteri(tex.id, GL_TEXTURE_WRAP_S, wrapS);
    if (wrapT != GL_REPEAT) glTextureParameteri(tex.id, GL_TEXTURE_WRAP_T, wrapT);
    if (filterMin != GL_NEAREST) glTextureParameteri(tex.id, GL_TEXTURE_MIN_FILTER, filterMin);
    if (filterMag != GL_NEAREST) glTextureParameteri(tex.id, GL_TEXTURE_MAG_FILTER, filterMag);
    glTextureStorage2D(tex.id, 1, internalFormat, width, height);
    if (mimmap) glGenerateTextureMipmap(tex.id);
    return tex;
}

ZC_Texture ZC_Texture::TextureStorage2DFill(GLenum internalFormat, GLsizei width, GLsizei height, const void* pData, GLenum format, GLenum type, bool mimmap,
    GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag)
{
    auto tex = TextureStorage2D(internalFormat, width, height, mimmap, wrapS, wrapT, filterMin, filterMag);
    tex.GLTextureSubImage2D(0, 0, width, height, format, type, pData);
    return tex;
}

ZC_Texture ZC_Texture::TextureStorage2DMultisample(GLsizei samples, GLsizei width, GLsizei height, GLenum internalFormat)
{
    ZC_Texture tex(GL_TEXTURE_2D_MULTISAMPLE);
    glTextureStorage2DMultisample(tex.id, samples, internalFormat, width, height, GL_TRUE);
    return tex;
}

// ZC_Texture::ZC_Texture(int width, int height, unsigned char** pData)
//     : target(GL_TEXTURE_CUBE_MAP)
// {
//     glGenTextures(1, &id);
//     glBindTexture(target, id);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//     for (int i = 0; i < 6; ++i) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData[i]);
//     glBindTexture(target, 0);
// }

ZC_Texture::ZC_Texture(ZC_Texture&& tex) noexcept
    : id(tex.id)
{
    tex.id = 0;
}

ZC_Texture& ZC_Texture::operator = (ZC_Texture&& tex)
{
    id = tex.id;
    tex.id = 0;
    return *this;
}

ZC_Texture::~ZC_Texture()
{
    if (id != 0) glDeleteTextures(1, &id);
}

void ZC_Texture::GLBindTextureUnit(GLuint num) const
{
    glBindTextureUnit(num, id);
}

GLuint ZC_Texture::GetId() const noexcept
{
    return id;
}

void ZC_Texture::GLTextureSubImage2D(GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pData)
{
    glTextureSubImage2D(id, 0, xoffset, yoffset, width, height, format, type, pData);
}

ZC_Texture::ZC_Texture(GLenum target)
{
    glCreateTextures(target, 1, &id);
}