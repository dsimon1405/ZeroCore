#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC/Tools/Container/ZC_DynamicArray.h>

#include <forward_list>

//  Class for configuring ZC_VAO.
class ZC_VAOConfig
{
public:
    //  Struct contain glVertexAttribFormat parameter data.
    struct Format
    {
        GLuint attribIndex;
        GLint size;
        GLenum type;
        GLboolean normalized;

        /*
        Create Format object.

        Params:
        Listed at the link https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
        */  
        Format(GLuint _attribIndex, GLint _size, GLenum _type, GLboolean _normalized);
    };

    ZC_DA<Format> formats;
    
    ZC_VAOConfig(ZC_DA<Format> _formats);

    ZC_VAOConfig(ZC_VAOConfig&& vaoConfig) noexcept;
    ZC_VAOConfig& operator = (ZC_VAOConfig&& vaoConfig) noexcept;
    
    void Config(GLuint startOffset, GLuint vertsCount);

private:
    struct StrideOffset
    {
        GLint stride = 0;
        GLuint relativeOffset = 0;
    };

    ZC_DA<StrideOffset> CalculateStrideAndOffset(GLuint startOffset, GLuint vertsCount);
    GLint TypeSize(GLenum type) const noexcept;
};