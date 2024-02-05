#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>

class ZC_Textures;

class ZC_Texture
{
    friend class ZC_Textures;
public:
	ZC_Texture(const ZC_Texture&) = delete;
	ZC_Texture& operator = (const ZC_Texture&) = delete;

	ZC_Texture(ZC_Texture&& tex) noexcept;
	ZC_Texture& operator = (ZC_Texture&& tex);

    ~ZC_Texture();

    void ActiveTexture(GLuint num = 0);

// private:
    GLuint id = 0;
    GLenum type;

    ZC_Texture(GLenum _type, GLenum format, int width, int height, void* data, GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag);
    //  Cube map ctr
    ZC_Texture(GLenum _type, int width, int height, unsigned char** data);
};