#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/Tools/Math/ZC_Math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ZC_Texture ZC_Texture::LoadTexture2D(const char* filePath, GLuint _binding, GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag)
{
    int width = 0,
        height = 0,
        channels = 0;

    stbi_set_flip_vertically_on_load(true);
    
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
    ZC_Texture texture = TextureStorage2DFill(internalFormat, _binding, width, height, pData, format, GL_UNSIGNED_BYTE, true, wrapS, wrapT, filterMin, filterMag);
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

ZC_Texture ZC_Texture::TextureStorage2D(GLenum internalFormat, GLuint _binding, GLsizei width, GLsizei height, bool mimmap, GLenum wrapS, GLenum wrapT,
    GLenum filterMin, GLenum filterMag)
{
    ZC_Texture tex(GL_TEXTURE_2D, _binding, width, height);
    if (wrapS != GL_REPEAT) glTextureParameteri(tex.id, GL_TEXTURE_WRAP_S, wrapS);
    if (wrapT != GL_REPEAT) glTextureParameteri(tex.id, GL_TEXTURE_WRAP_T, wrapT);
    if (filterMin != GL_NEAREST) glTextureParameteri(tex.id, GL_TEXTURE_MIN_FILTER, filterMin);
    if (filterMag != GL_NEAREST) glTextureParameteri(tex.id, GL_TEXTURE_MAG_FILTER, filterMag);
    glTextureStorage2D(tex.id, 1, internalFormat, width, height);
    if (mimmap) glGenerateTextureMipmap(tex.id);
    return tex;
}

ZC_Texture ZC_Texture::TextureStorage2DFill(GLenum internalFormat, GLuint _binding, GLsizei width, GLsizei height, const void* pData,
    GLenum format, GLenum type, bool mimmap, GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag)
{
    auto tex = TextureStorage2D(internalFormat, _binding, width, height, mimmap, wrapS, wrapT, filterMin, filterMag);
    tex.GLTextureSubImage2D(0, 0, width, height, format, type, pData);
    return tex;
}

ZC_Texture ZC_Texture::TextureStorage2DMultisample(GLsizei samples, GLsizei width, GLsizei height, GLenum internalFormat)
{
    ZC_Texture tex(GL_TEXTURE_2D_MULTISAMPLE, 0, width, height);
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
    : id(tex.id),
    binding(tex.binding),
    width(tex.width),
    height(tex.height)
{
    tex.id = 0;
}

ZC_Texture& ZC_Texture::operator = (ZC_Texture&& tex)
{
    if (id != 0) glDeleteTextures(1, &id);
    id = tex.id;
    binding = tex.binding;
    width = tex.width;
    height = tex.height;
    
    tex.id = 0;
    tex.binding = 0;
    return *this;
}

ZC_Texture::~ZC_Texture()
{
    if (id != 0) glDeleteTextures(1, &id);
}

void ZC_Texture::GLBindTextureUnit(GLuint DELETE_BINDING) const
{
    if (DELETE_BINDING == 1000000) glBindTextureUnit(binding, id);
    else glBindTextureUnit(DELETE_BINDING, id);
}

GLuint ZC_Texture::GetId() const noexcept
{
    return id;
}

void ZC_Texture::GLTextureSubImage2D(GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pData)
{
    glTextureSubImage2D(id, 0, xoffset, yoffset, width, height, format, type, pData);
}

int ZC_Texture::GetWidth() const noexcept
{
    return width;
}

int ZC_Texture::GetHeight() const noexcept
{
    return height;
}

ZC_Texture::ZC_Texture(GLenum target, GLuint _binding, int width, int height)
    : binding(_binding),
    width(width),
    height(height)
{
    glCreateTextures(target, 1, &id);
}

GLenum GetFormat(GLenum internalFormat)
{
    switch (internalFormat)
    {
    case GL_R8: return GL_RED;
    case GL_R8_SNORM: return GL_RED;
    case GL_R16: return GL_RED;
    case GL_R16_SNORM: return GL_RED;
    case GL_RG8: return GL_RG;
    case GL_RG8_SNORM: return GL_RG;
    case GL_RG16: return GL_RG;
    case GL_RG16_SNORM: return GL_RG;
    case GL_R3_G3_B2: return GL_RGB;
    case GL_RGB4: return GL_RGB;
    case GL_RGB5: return GL_RGB;
    case GL_RGB8: return GL_RGB;
    case GL_RGB8_SNORM: return GL_RGB;
    case GL_RGB10: return GL_RGB;
    case GL_RGB12: return GL_RGB;
    case GL_RGB16_SNORM: return GL_RGB;
    case GL_RGBA2: return GL_RGB;
    case GL_RGBA4: return GL_RGB;
    case GL_RGB5_A1: return GL_RGBA;
    case GL_RGBA8: return GL_RGBA;
    case GL_RGBA8_SNORM: return GL_RGBA;
    case GL_RGB10_A2: return GL_RGBA;
    case GL_RGB10_A2UI: return GL_RGBA;
    case GL_RGBA12: return GL_RGBA;
    case GL_RGBA16: return GL_RGBA;
    case GL_SRGB8: return GL_RGB;
    case GL_SRGB8_ALPHA8: return GL_RGBA;
    case GL_R16F: return GL_RED;
    case GL_RG16F: return GL_RG;
    case GL_RGB16F: return GL_RGB;
    case GL_RGBA16F: return GL_RGBA;
    case GL_R32F: return GL_RED;
    case GL_RG32F: return GL_RG;
    case GL_RGB32F: return GL_RGB;
    case GL_RGBA32F: return GL_RGBA;
    case GL_R11F_G11F_B10F: return GL_RGB;
    case GL_RGB9_E5: return GL_RGB;
    case GL_R8I: return GL_RED;
    case GL_R8UI: return GL_RED;
    case GL_R16I: return GL_RED;
    case GL_R16UI: return GL_RED;
    case GL_R32I: return GL_RED;
    case GL_R32UI: return GL_RED;
    case GL_RG8I: return GL_RG;
    case GL_RG8UI: return GL_RG;
    case GL_RG16I: return GL_RG;
    case GL_RG16UI: return GL_RG;
    case GL_RG32I: return GL_RG;
    case GL_RG32UI: return GL_RG;
    case GL_RGB8I: return GL_RGB;
    case GL_RGB8UI: return GL_RGB;
    case GL_RGB16I: return GL_RGB;
    case GL_RGB16UI: return GL_RGB;
    case GL_RGB32I: return GL_RGB;
    case GL_RGB32UI: return GL_RGB;
    case GL_RGBA8I: return GL_RGBA;
    case GL_RGBA8UI: return GL_RGBA;
    case GL_RGBA16I: return GL_RGBA;
    case GL_RGBA16UI: return GL_RGBA;
    case GL_RGBA32I: return GL_RGBA;
    case GL_RGBA32UI: return GL_RGBA; 
    default: assert(false); return 0;
    }
}