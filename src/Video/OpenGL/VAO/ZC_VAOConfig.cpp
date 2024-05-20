#include <ZC/Video/OpenGL/VAO/ZC_VAOConfig.h>

typename ZC_VAOConfig::ConfigData ZC_VAOConfig::CreateConfig(ZC_VAOLayout layout, uchar* pLayoutUsed, ulong layoutUsedCount)
{
    LayoutPacker layoutPacker;
    for (ulong i = 0; i < layoutUsedCount; ++i) layoutPacker.Pack(pLayoutUsed[i]);
    return { layout, layoutPacker };    
}

ZC_VAOConfig::ZC_VAOConfig(ConfigData configData)
    : formats(GetFormats(configData.formatShVLayout)),
    useCount(configData.usingFormatsPacker.value)
{
    uint isUsingArr = configData.usingFormatsPacker.value >> 8;
    uchar useCounter = useCount;
    for (ulong formatsI = 0; formatsI < formats.size; ++formatsI)
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
    for (ulong i = 0; i < formats.size && useCounter != 0; ++i)
    {
        if (formats[i].isUsing)
        {
            glEnableVertexAttribArray(formats[i].attribIndex);
            glVertexAttribPointer(formats[i].attribIndex, formats[i].size, formats[i].type, formats[i].normalized, so[i].stride, (const void*)(unsigned long long)(so[i].relativeOffset));
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
        for (ulong i = 0; i < formats.size && useCounter != 0; ++i)
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

ZC_DA<typename ZC_VAOConfig::Format> ZC_VAOConfig::GetFormats(ZC_VAOLayout spf)
{
    switch (spf)
    {
    case ZC_VAOL_F_2_0__UB_2_1_N: return { new Format[]{ { 0, 2, GL_FLOAT, GL_FALSE }, { 1, 2, GL_UNSIGNED_BYTE, GL_TRUE } }, 2 };
    case ZC_VAOL_F_2_0__US_2_1_N: return { new Format[]{ { 0, 2, GL_FLOAT, GL_FALSE }, { 1, 2, GL_UNSIGNED_SHORT, GL_TRUE } }, 2 };
    case ZC_VAOL_F_3_0: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE } }, 1 };
    case ZC_VAOL_F_3_0__F_3_1: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 1, 3, GL_FLOAT, GL_FALSE } }, 2 };
    case ZC_VAOL_F_3_0__F_2_1: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 1, 2, GL_FLOAT, GL_FALSE } }, 2 };
    case ZC_VAOL_F_3_0__F_2_3: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 3, 2, GL_FLOAT, GL_FALSE } }, 2 };
    case ZC_VAOL_F_3_0__UB_3_1_N: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 1, 3, GL_UNSIGNED_BYTE, GL_TRUE } }, 2 };
    case ZC_VAOL_F_4_0__UB_3_1_N: return { new Format[]{ { 0, 4, GL_FLOAT, GL_FALSE }, { 1, 3, GL_UNSIGNED_BYTE, GL_TRUE } }, 2 };
    case ZC_VAOL_F_3_0__UB_3_1_N__I_2_10_10_10_REV_1_2_N: return { new Format[]{ { 0, 3, GL_FLOAT, GL_FALSE }, { 1, 3, GL_UNSIGNED_BYTE, GL_TRUE }, { 2, 4, GL_INT_2_10_10_10_REV, GL_TRUE } }, 3 };
    case ZC_VAOL_F_4_0: return { new Format[]{ { 0, 4, GL_FLOAT, GL_FALSE } }, 1 };
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

ZC_VAOConfig::LayoutPacker::LayoutPacker(uchar index)
{
    if (index != -1) Pack(index);
}

typename ZC_VAOConfig::LayoutPacker& ZC_VAOConfig::LayoutPacker::Pack(uchar index)
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

bool ZC_VAOConfig::ConfigData::operator == (ZC_VAOLayout _formatShVLayout)
{
    return formatShVLayout == _formatShVLayout;
}