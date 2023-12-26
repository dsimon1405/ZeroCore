#include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>

#include <Video/OpenGL/ZC_OpenGL.h>

ZC_VAOConfig::ZC_VAOConfig(ZC_VAOConfig&& vaoConfig) noexcept
        : formats(std::move(vaoConfig.formats))
{}

ZC_VAOConfig& ZC_VAOConfig::operator = (ZC_VAOConfig&& vaoConfig) noexcept
{
    formats = std::move(vaoConfig.formats);
    return *this;
}

bool ZC_VAOConfig::operator < (const ZC_VAOConfig& vaoConfig) const noexcept
{
    return formats < vaoConfig.formats;
}

ZC_VAOConfig* ZC_VAOConfig::AddFormat(const ZC_VAOConfig::Format& format) noexcept
{
    formats.emplace_back(format);
    return this;
}

GLint ZC_VAOConfig::MaxCount() noexcept
{
    static GLint maxCount = GetMaxCount();
    return maxCount;
}

void ZC_VAOConfig::Config(const GLuint& config) const noexcept
{
    for (auto& format : formats)
    {
        glEnableVertexAttribArray(format.attribindex);
        glVertexAttribFormat(format.attribindex, format.size, format.type, format.normalized, format.relativeoffset);
        glVertexAttribBinding(format.attribindex, config);
    }
}

GLint ZC_VAOConfig::Stride() const noexcept
{
    GLint stride = 0;
    for (auto& format : formats)
    {
        GLint typeSize = 0;
        switch (format.type)
        {
        case GL_BYTE: typeSize = sizeof(GLbyte); break;
        case GL_UNSIGNED_BYTE: typeSize = sizeof(GLbyte); break;
        case GL_SHORT: typeSize = sizeof(GLshort); break;
        case GL_HALF_FLOAT: typeSize = sizeof(GLshort); break;
        case GL_UNSIGNED_SHORT: typeSize = sizeof(GLshort); break;
        case GL_INT: typeSize = sizeof(GLint); break;
        case GL_FLOAT: typeSize = sizeof(GLint); break;
        case GL_UNSIGNED_INT: typeSize = sizeof(GLint); break;
        case GL_INT_2_10_10_10_REV: typeSize = sizeof(GLint); break;
        case GL_UNSIGNED_INT_2_10_10_10_REV: typeSize = sizeof(GLint); break;
        case GL_UNSIGNED_INT_10F_11F_11F_REV: typeSize = sizeof(GLint); break;
#ifdef ZC_PC
        case GL_DOUBLE: typeSize = sizeof(GLdouble); break;
#endif
        }
//         if (format.type == GL_BYTE || format.type == GL_UNSIGNED_BYTE) typeSize = sizeof(GLbyte);
//         else if (format.type == GL_SHORT || format.type == GL_HALF_FLOAT || format.type == GL_UNSIGNED_SHORT) typeSize = sizeof(GLshort);
//         else if (format.type == GL_INT || format.type == GL_FLOAT || format.type == GL_UNSIGNED_INT || format.type == GL_INT_2_10_10_10_REV
//                  || format.type == GL_UNSIGNED_INT_2_10_10_10_REV || format.type == GL_UNSIGNED_INT_10F_11F_11F_REV) typeSize = sizeof(GLint);
// #ifdef ZC_PC
//         else if (format.type == GL_DOUBLE) typeSize = sizeof(GLdouble);
// #endif

        stride += (format.size * typeSize);
    }
    return stride;
}

GLint ZC_VAOConfig::GetMaxCount() noexcept
{
    GLint maxCount = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &maxCount);
    return maxCount;
}

//  Format start

ZC_VAOConfig::Format::Format(GLuint _attribindex, GLint _size, GLenum _type, GLboolean _normalized, GLuint _relativeoffset) noexcept
        : attribindex(_attribindex),
          size(_size),
          type(_type),
          normalized(_normalized),
          relativeoffset(_relativeoffset)
{}

bool ZC_VAOConfig::Format::operator < (const Format& format) const noexcept
{
    if (attribindex < format.attribindex
        || size < format.size
        || type < format.type
        || normalized < format.normalized
        || relativeoffset < format.relativeoffset) return true;
    return false;
}
//  Format end