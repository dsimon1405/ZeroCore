#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC/Tools/Math/ZC_Math.h>
#include <ZC/Tools/ZC_uptr.h>
#include <ZC/Tools/Container/ZC_DA.h>

//  Class for configuring ZC_VAO.
class ZC_VAOConfig
{
public:
    /*
    Example => F_3_0:
    F - type float
    3 - count
    0 - location in vertex shader
    */
    enum FormatShVLayout
    {
        None,
        F_3_0,
        F_3_0__F_3_1,
        F_3_0__F_2_1,
        F_3_0__F_2_3,
        F_3_0__UB_3_1,  //  UB_3_1 -> vec3 of normalized (255 -> 1.f) bytes
        F_4_0__UB_3_1,  //  UB_3_1 -> vec3 of normalized (255 -> 1.f) bytes
        F_3_0__UB_3_1__I_2_10_10_10_REV_1_2,     //  I_2_10_10_10_REV_1 one int32 in code and vec4 of floats in GLSL
        F_4_0,
    };

    /*
    Pack in format uint
    uchar[32] => [0 - 7]{ZC_VAOConfig.useCount - count of using formats}, [8 - 31]{in each bit true or false for using format or not}
    */
    struct UsingFormatsPacker
    {
        uint value = 0;

        //  index - that should be true in Format.isUsing array
        UsingFormatsPacker& Pack(uchar index);
    };

    struct FormatShVLayoutAndUsingFormatsPacker
    {
        FormatShVLayout formatShVLayout;
        UsingFormatsPacker usingFormatsPacker;

        bool operator == (FormatShVLayout _formatShVLayout);
    };
    
    ZC_VAOConfig() = default;
    //  isUsingIndexes - Format.isUsing - true indexes in ZC_DA<Format> formats
    ZC_VAOConfig(FormatShVLayoutAndUsingFormatsPacker fsvlAufp);

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
    ZC_DA<Format> GetFormats(FormatShVLayout shPFormat);
};