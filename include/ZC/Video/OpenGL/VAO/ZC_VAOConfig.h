#pragma once

#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Tools/ZC_uptr.h>
#include <ZC/Tools/Container/ZC_DA.h>

#include <glad/glad.h>

/*
Example => S_3_0_N:
S - type short
3 - count
0 - location in vertex shader
N - normalizes, S is signed short [-1,1], if US unsigned short [0,1] (if don't used normalization - N missing!)
*/
enum ZC_VAOLayout
{
    ZC_VAOL__None,
    ZC_VAOL__F_2_0__UB_2_1_N,
    ZC_VAOL__F_2_0__US_2_1_N,   //  US_2 in unsigned short packed texture coords 
    ZC_VAOL__F_3_0,
    ZC_VAOL__F_3_0__F_3_1,
    ZC_VAOL__F_3_0__F_2_1,
    ZC_VAOL__F_3_0__F_2_3,
    ZC_VAOL__F_3_0__UB_3_1_N,  //  UB_3_1 -> vec3 of normalized (255 -> 1.f) bytes
    ZC_VAOL__F_4_0__UB_3_1_N,  //  UB_3_1 -> vec3 of normalized (255 -> 1.f) bytes
    ZC_VAOL__F_3_0__UB_3_1_N__I_2_10_10_10_REV_1_2_N,     //  I_2_10_10_10_REV_1 one int32 in code and vec4 of floats in GLSL
    ZC_VAOL__F_4_0,
    ZC_VAOL__F_3_0__I_2_10_10_10_REV_1_1_N__US_2_2_N
};

//  Class for configuring ZC_VAO.
class ZC_VAOConfig
{
public:

    /*
    Pack in format uint
    uchar[32] => [0 - 7]{ZC_VAOConfig.useCount - count of using formats}, [8 - 31]{in each bit true or false for using format or not}
    */
    struct LayoutPacker
    {
        uint value = 0;

        LayoutPacker(uchar index = -1);

        //  index - that should be true in Format.isUsing array
        LayoutPacker& Pack(uchar index);
    };

    struct ConfigData
    {
        ZC_VAOLayout formatShVLayout;
        LayoutPacker usingFormatsPacker;

        bool operator == (ZC_VAOLayout _formatShVLayout);
    };

    static ConfigData CreateConfig(ZC_VAOLayout formatShVLayout, uchar* pIndexUsed, ulong indexUsedCount);
    
    ZC_VAOConfig() = default;
    //  isUsingIndexes - Format.isUsing - true indexes in ZC_DA<Format> formats
    ZC_VAOConfig(ConfigData configData);

    ZC_VAOConfig(ZC_VAOConfig&& vaoConfig) noexcept;
    
    void Config(GLuint startOffset, GLuint vertsCount);


private:
    //  Struct contain glVertexAttribFormat parameter data.
    struct Format
    {
        GLuint attribIndex;
        GLint size;
        GLenum type;
        GLboolean normalized;
        bool isUsing = false;

        /*
        Create Format object.

        Params:
        Listed at the link https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
        */  
        Format(GLuint _attribIndex, GLint _size, GLenum _type, GLboolean _normalized);
    };

    ZC_DA<Format> formats;
    uchar useCount;

    struct StrideOffset
    {
        GLint stride = 0;
        GLuint relativeOffset = 0;
    };

    ZC_DA<StrideOffset> CalculateStrideAndOffset(GLuint startOffset, GLuint vertsCount);
    GLint TypeSize(GLenum type) const noexcept;
    ZC_DA<Format> GetFormats(ZC_VAOLayout shPFormat);
};