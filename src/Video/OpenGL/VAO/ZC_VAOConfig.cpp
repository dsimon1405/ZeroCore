#include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_VAOConfig::ZC_VAOConfig(ZC_DA<Format> _formats)
    : formats(std::move(_formats))
{}

ZC_VAOConfig::ZC_VAOConfig(ZC_VAOConfig&& vaoConfig) noexcept
    : formats(std::move(vaoConfig.formats))
{}

ZC_VAOConfig& ZC_VAOConfig::operator = (ZC_VAOConfig&& vaoConfig) noexcept
{
    formats = std::move(vaoConfig.formats);
    return *this;
}

void ZC_VAOConfig::Config(GLuint startOffset, GLuint vertsCount)
{
    ZC_DA<StrideOffset> so = CalculateStrideAndOffset(startOffset, vertsCount);
    for (size_t i = 0; i < formats.size; ++i)
    {
        glEnableVertexAttribArray(formats[i].attribIndex);
        glVertexAttribPointer(formats[i].attribIndex, formats[i].size, formats[i].type, formats[i].normalized, so[i].stride, reinterpret_cast<void*>(so[i].relativeOffset));
    }
}

ZC_DA<typename ZC_VAOConfig::StrideOffset> ZC_VAOConfig::CalculateStrideAndOffset(GLuint startOffset, GLuint vertsCount)
{
    ZC_DA<StrideOffset> so(formats.size);
    if (vertsCount == 0)    //  VNCVNC
    {
        GLint localStartOffset = 0;
        for (unsigned char i = 0; i < formats.size; ++i)
        {
            so[i].relativeOffset = startOffset + localStartOffset;
            localStartOffset += TypeSize(formats[i].type) * formats[i].size;
        }
        for (unsigned char i = 0; i < formats.size; ++i) so[i].stride = localStartOffset;
    }
    else    //  VVNNCC
    {
        GLint localStartOffset = 0;
        for (unsigned char i = 0; i < formats.size; ++i)
        {
            so[i].relativeOffset = startOffset + localStartOffset;
            so[i].stride = TypeSize(formats[i].type) * formats[i].size;
            localStartOffset += so[i].stride * vertsCount;
        }
    }
    return so;
}

GLint ZC_VAOConfig::TypeSize(GLenum type) const noexcept
{
        switch (type)
        {
        case GL_BYTE: return 1;
        case GL_UNSIGNED_BYTE: return 1;
        case GL_SHORT: return 2;
        case GL_UNSIGNED_SHORT: return 2;
        case GL_FLOAT: return 4;
        case GL_HALF_FLOAT: return 2;
        case GL_INT: return 4;
        case GL_INT_2_10_10_10_REV: return 1;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_INT_2_10_10_10_REV: return 1;
        case GL_UNSIGNED_INT_10F_11F_11F_REV: return 1;
#ifdef ZC_PC
        case GL_DOUBLE: return 8;
#endif
        }

// void ZC_VAOConfig::Config(const GLuint& config) const noexcept
// {
//     for (auto& format : formats)
//     {
//         glEnableVertexAttribArray(format.attribindex);
//         glVertexAttribFormat(format.attribindex, format.size, format.type, format.normalized, format.relativeoffset);
//         glVertexAttribBinding(format.attribindex, config);
//     }
// }
}


//  Format start

ZC_VAOConfig::Format::Format(GLuint _attribindex, GLint _size, GLenum _type, GLboolean _normalized)
    : attribIndex(_attribindex),
    size(_size),
    type(_type),
    normalized(_normalized)
{}
//  Format end