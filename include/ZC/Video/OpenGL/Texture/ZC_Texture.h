#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>

class ZC_Texture
{
public:
    static ZC_Texture LoadTexture2D(const char* filePath, GLenum wrapS = GL_CLAMP_TO_EDGE,   // wrapS = wrapT = GL_REPEAT
        GLenum wrapT = GL_CLAMP_TO_EDGE, GLenum filterMin = GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag = GL_LINEAR);
    // static ZC_Texture* LoadCubeMap(const char** filePaths);
    
    /*
    Creates a multisample texture for use in the framebuffer and a simple 2D texture for post-processing.

    Params:

    */
    // static void CreateMultisampleSet(ZC_Texture& rTexMultisample, ZC_Texture& rTexPostProcessing, int samples, int width, int height, GLenum format);

    ZC_Texture() = default;

    /*
    Texture.

    Params:
    _type - GL_TEXTURE_2D most used.
    internalFormat - number of color components in texture. Most used: GL_RED, GL_RG, GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL. More info: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
    width - textures width (max 1024).
    height - textures heiht (max 1024).
    format - format of the pixel data. Most used: GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL.
    type - usual GL_UNSIGNED_BYTE. All types: GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV, GL_UNSIGNED_INT_10_10_10_2, and GL_UNSIGNED_INT_2_10_10_10_REV.
    pData - pointer on data (nullpt just reserve place in texture).
    wrapS - GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER (default GL_REPEAT). Texture axis S (as X).
    wrapT - GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER (default GL_REPEAT). Texture axis T (as Y).
    filterMin - GL_NEAREST, GL_LINEAR (default GL_NEArEST).
    filterMag - GL_NEAREST, GL_LINEAR (default GL_NEArEST).
    needMipmap - need to create mipmap or not.
    */
    ZC_Texture(GLenum _target, GLenum internalFormat, int width, int height, GLenum format, GLenum type, void* pData,
        GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT, GLenum filterMin = GL_NEAREST, GLenum filterMag = GL_NEAREST, bool needMimmap = true);
    
    /*
    Multisample texture for framebuffer.

    Params:
    samples - count of samples on pixel (usual 2 or 4).
    width - textures width (max 1024).
    height - textures heiht (max 1024).
    internalFormat - most used: GL_RED, GL_RG, GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL. More info: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
    */
    ZC_Texture(int samples, int width, int height, GLenum internalFormat);
    // //  Cube map ctr
    // ZC_Texture(int width, int height, unsigned char** data);

	ZC_Texture(ZC_Texture&& tex) noexcept;
	ZC_Texture& operator = (ZC_Texture&& tex);

    ~ZC_Texture();

    void Bind();
    void Unbind();
    void ActiveTexture(GLuint num = 0) const;
    void GenerateMimmap() const;
    GLuint GetId() const noexcept;

private:
    GLuint id = 0;
    GLenum target = 0;

    void CreateAndConfigure(GLenum internalFormat, int width, int height, GLenum format, GLenum type, void* data,
        GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag, bool needMimmap);
};