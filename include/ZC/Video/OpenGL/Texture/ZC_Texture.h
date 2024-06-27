#pragma once

#include <glad/glad.h>

class ZC_Texture
{
public:
    /*
    - wrapS - GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER (default GL_REPEAT). Texture axis S (as X).
    - wrapT - GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER (default GL_REPEAT). Texture axis T (as Y).
    - filterMin - GL_NEAREST, GL_LINEAR (default GL_NEAREST).
    - filterMag - GL_NEAREST, GL_LINEAR (default GL_NEAREST).
    */
    static ZC_Texture LoadTexture2D(const char* filePath, GLuint _binding,
    //  GLenum wrapS,   // wrapS = wrapT = GL_REPEAT
    //     GLenum wrapT, GLenum filterMin, GLenum filterMag);
     GLenum wrapS = GL_CLAMP_TO_EDGE,   // wrapS = wrapT = GL_REPEAT
        GLenum wrapT = GL_CLAMP_TO_EDGE, GLenum filterMin = GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag = GL_LINEAR);
    // static ZC_Texture* LoadCubeMap(const char** filePaths);
    
    /*
    Creates -> no resizable texture 2D.

    Params:
    internalFormat - number of color components in texture. Most used: GL_R8, GL_RGB8, GL_RGBA8, GL_DEPTH_COMPONENT24. Table 1: Sized Internal Format -> https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
    width - textures width (max 1024).
    height - textures heiht (max 1024).
    wrapS - GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER (default GL_REPEAT). Texture axis S (as X).
    wrapT - GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER (default GL_REPEAT). Texture axis T (as Y).
    filterMin - GL_NEAREST, GL_LINEAR (default GL_NEAREST).
    filterMag - GL_NEAREST, GL_LINEAR (default GL_NEAREST).
    mimmap - need to create mipmap or not.
    */
    static ZC_Texture TextureStorage2D(GLenum internalFormat, GLuint _binding, GLsizei width, GLsizei height, bool mimmap, GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT,
        GLenum filterMin = GL_NEAREST, GLenum filterMag = GL_NEAREST);

    /*
    Creates -> no resizable texture 2D, and fill it.

    Params:
    internalFormat - number of color components in texture. Most used: GL_R8, GL_RGB8, GL_RGBA8, GL_DEPTH_COMPONENT24. Table 1: Sized Internal Format -> https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2D.xhtml
    width - textures width (max 1024).
    height - textures heiht (max 1024).
    pData - pointer on data (nullpt just reserve place in texture).
    format - format of the pixel data. Most used: GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL.
    type - usual GL_UNSIGNED_BYTE. All types: GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV, GL_UNSIGNED_INT_10_10_10_2, and GL_UNSIGNED_INT_2_10_10_10_REV.
    wrapS - GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER (default GL_REPEAT). Texture axis S (as X).
    wrapT - GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER (default GL_REPEAT). Texture axis T (as Y).
    filterMin - GL_NEAREST, GL_LINEAR (default GL_NEAREST).
    filterMag - GL_NEAREST, GL_LINEAR (default GL_NEAREST).
    mimmap - need to create mipmap or not.
    */
    static ZC_Texture TextureStorage2DFill(GLenum internalFormat, GLuint _binding, GLsizei width, GLsizei height, const void* pData, GLenum format, GLenum type, bool mimmap,
        GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT, GLenum filterMin = GL_NEAREST, GLenum filterMag = GL_NEAREST);

    /*
    Creates -> no resizable, multisampled texture 2D.

    Params:
    samples - count of samples on pixel (usual 2 or 4).
    width - textures width (max 1024).
    height - textures heiht (max 1024).
    internalFormat - number of color components in texture. Most used: GL_R8, GL_RGB8, GL_RGBA8, GL_DEPTH_COMPONENT24. Table 1: Sized Internal Format -> https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexStorage2DMultisample.xhtml
    */
    static ZC_Texture TextureStorage2DMultisample(GLsizei samples, GLsizei width, GLsizei height, GLenum internalFormat);

    // //  Cube map ctr
    // ZC_Texture(int width, int height, unsigned char** data);

    ZC_Texture() = default;
    
	ZC_Texture(ZC_Texture&& tex) noexcept;
	ZC_Texture& operator = (ZC_Texture&& tex);

    ~ZC_Texture();

        //  activates texture
    void GLBindTextureUnit(GLuint DELTE_BINDING = 1000000) const;
    GLuint GetId() const noexcept;

    /*
    Params:
    - xoffset - offset X (by width)
    - yoffset - offset Y (by height). Beging (0) in bottom left corner.
    - width - width of the saved data (pData).
    - height - height of the saved data (pData).
    - format - format of CPU's data (pData).  
    - type - format of the pixel data: GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_DEPTH_COMPONENT, and GL_STENCIL_INDEX.
        Don't forget to set glPixelStorei(GL_UNPACK_ALIGNMENT, 1/2/4/8 (default 4)) if byte alignment in texture  not equal default 4.
    - pData - pointer on data.
    */
    void GLTextureSubImage2D(GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pData);

    int GetWidth() const noexcept;
    int GetHeight() const noexcept;

private:
    GLuint id = 0;
    GLuint binding = 0;
    int width = 0;
    int height = 0;

    ZC_Texture(GLenum target, GLuint _binding, int width, int height);
    static GLenum GetFormat(GLenum internalFormat);     //  unused now
};