#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>

#include <list>

class ZC_VAOs;

//  Class for configuring ZC_VAO.
class ZC_VAOConfig
{
    friend class ZC_VAOs;
public:
    //  Struct contain glVertexAttribFormat parameter data.
    struct Format
    {
        GLuint attribindex;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLuint relativeoffset;

        /*
        Create Format object.

        Params:
        Listed at the link https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
        */  
        Format(GLuint _attribindex, GLint _size, GLenum _type, GLboolean _normalized, GLuint _relativeoffset) noexcept;

        bool operator < (const Format& format) const noexcept;
    };

    ZC_VAOConfig() = default;

    ZC_VAOConfig(const ZC_VAOConfig& vaoConfig) = default;
    ZC_VAOConfig& operator = (const ZC_VAOConfig& vaoConfig) = default;

    ZC_VAOConfig(ZC_VAOConfig&& vaoConfig) noexcept;
    ZC_VAOConfig& operator = (ZC_VAOConfig&& vaoConfig) noexcept;

    ~ZC_VAOConfig() = default;

    bool operator < (const ZC_VAOConfig& vaoConfig) const noexcept;

    /*
    Add new format to configuration.

    Params:
    format - format to add.

    Return:
    Pointer to itself.
    */
    ZC_VAOConfig* AddFormat(const ZC_VAOConfig::Format& format) noexcept;

	/*
	The maximum number of different vertex array object configurations.

	Return:
	Max maximum number of vao configurations.
	*/
	static GLint MaxCount() noexcept;

private:
    std::list<Format> formats;

    static GLint GetMaxCount() noexcept;

    void Config(const GLuint& config) const noexcept;
    GLint Stride() const noexcept;
};