#include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_VAOConfig::ZC_VAOConfig(FormatShVLayoutAndUsingFormatsPacker fsvlAufp)
    : formats(GetFormats(fsvlAufp.formatShVLayout)),
    useCount(fsvlAufp.usingFormatsPacker.value)
{
    uint isUsingArr = fsvlAufp.usingFormatsPacker.value >> 8;
    uchar useCounter = useCount;
    for (size_t formatsI = 0; formatsI < formats.size ; ++formatsI)
    {
        formats[formatsI].isUsing = (isUsingArr >> formatsI) & 1;
        if (formats[formatsI].isUsing && --useCounter == 0) break; 
    }
}

ZC_VAOConfig::ZC_VAOConfig(ZC_VAOConfig&& vaoConfig) noexcept
    : formats(std::move(vaoConfig.formats)),
    useCount(vaoConfig.useCount)
{}

void ZC_VAOConfig::Config(GLuint startOffset, GLuint verticesCount)
{
    ZC_DA<StrideOffset> so = CalculateStrideAndOffset(startOffset, verticesCount);
    uchar useCounter = useCount;
    for (size_t i = 0; i < formats.size && useCounter != 0; ++i)
    {
        if (formats[i].isUsing)
        {
            glEnableVertexAttribArray(formats[i].attribIndex);
            glVertexAttribPointer(formats[i].attribIndex, formats[i].size, formats[i].type, formats[i].normalized, so[i].stride, reinterpret_cast<void*>(so[i].relativeOffset));
            --useCounter;
        }
    }
}

ZC_DA<typename ZC_VAOConfig::StrideOffset> ZC_VAOConfig::CalculateStrideAndOffset(GLuint startOffset, GLuint verticesCount)
{
    ZC_DA<StrideOffset> so(formats.size);
    if (verticesCount == 0)    //  VNCVNC
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
        uchar useCounter = useCount;
        for (size_t i = 0; i < formats.size && useCounter != 0; ++i)
        {
            so[i].relativeOffset = startOffset + localStartOffset;
            so[i].stride = TypeSize(formats[i].type) * formats[i].size;
            localStartOffset += so[i].stride * verticesCount;
            if (formats[i].isUsing) --useCounter;
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
    return 0;
}

ZC_DA<typename ZC_VAOConfig::Format> ZC_VAOConfig::GetFormats(FormatShVLayout spf)
{
    switch (spf)
    {
    case F_3_0: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE } }, 1 };
    case F_3_0__F_3_1: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 1, 3, GL_FLOAT, GL_FALSE } }, 2 };
    case F_3_0__F_2_1: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 1, 2, GL_FLOAT, GL_FALSE } }, 2 };
    case F_3_0__F_2_3: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 3, 2, GL_FLOAT, GL_FALSE } }, 2 };
    case F_3_0__UB_3_1: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 1, 3, GL_UNSIGNED_BYTE, GL_TRUE } }, 2 };
    case F_4_0__UB_3_1: return { new Format[]{ { 0, 4, GL_FLOAT, GL_FALSE }, { 1, 3, GL_UNSIGNED_BYTE, GL_TRUE } }, 2 };
    case F_3_0__UB_3_1__I_2_10_10_10_REV_1_2: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 1, 3, GL_UNSIGNED_BYTE, GL_TRUE }, { 2, 4, GL_INT_2_10_10_10_REV, GL_TRUE } }, 3 };
    case F_4_0: return { new Format[]{ { 0, 4, GL_FLOAT, GL_FALSE } }, 1 };
    default: return {};
    }
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


//  UsingFormatsPacker

typename ZC_VAOConfig::UsingFormatsPacker& ZC_VAOConfig::UsingFormatsPacker::Pack(uchar index)
{
    value = (1 << (8 + index)) | (value & 0xFFFFFF00) | ((value & 0xFF) + 1);
    return *this;
}


//  Format

ZC_VAOConfig::Format::Format(GLuint _attribindex, GLint _size, GLenum _type, GLboolean _normalized)
    : attribIndex(_attribindex),
    size(_size),
    type(_type),
    normalized(_normalized)
{}


//  ShPFormatAndPacker

bool ZC_VAOConfig::FormatShVLayoutAndUsingFormatsPacker::operator == (FormatShVLayout _formatShVLayout)
{
    return formatShVLayout == _formatShVLayout;
}