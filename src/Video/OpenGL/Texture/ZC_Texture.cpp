#include <ZC/Video/OpenGL/Texture/ZC_Texture.h>

#include <Video/OpenGL/ZC_OpenGL.h>

ZC_Texture::ZC_Texture(ZC_Texture&& tex) noexcept
    : id(tex.id),
    type(tex.type)
{
    tex.id = 0;
}

ZC_Texture& ZC_Texture::operator = (ZC_Texture&& tex)
{
    id = tex.id;
    type = tex.type;
    tex.id = 0;
    return *this;
}

ZC_Texture::~ZC_Texture()
{
    glDeleteTextures(1, &id);
}

void ZC_Texture::Bind()
{
    glBindTexture(type, id);
}

void ZC_Texture::Unbind()
{
    glBindTexture(type, 0);
}

void ZC_Texture::ActiveTexture(GLuint num)
{
    glActiveTexture(GL_TEXTURE0 + num);
    glBindTexture(type, id);
}

ZC_Texture::ZC_Texture(GLenum _type, GLenum format, int width, int height, void* data, GLenum wrapS, GLenum wrapT, GLenum filterMin, GLenum filterMag)
    : type(_type)
{
    glGenTextures(1, &id);
    glBindTexture(type, id);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, filterMag);
    glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(type);
}

ZC_Texture::ZC_Texture(GLenum _type, int width, int height, unsigned char** data)
{
    glGenTextures(1, &id);
    glBindTexture(type, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (int i = 0; i < 6; ++i) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
}

GLuint ZC_Texture::GetId() const noexcept
{
    return id;
}