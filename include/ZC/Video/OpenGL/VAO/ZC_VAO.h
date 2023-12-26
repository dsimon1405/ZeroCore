#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC/Video/OpenGL/VBO/ZC_VBO.h>

//	Wrapper OpenGL vertex array object.
class ZC_VAO
{
    friend class ZC_VAOs;
public:
	ZC_VAO(const ZC_VAO&) = delete;
	ZC_VAO& operator = (const ZC_VAO&) = delete;

	ZC_VAO(ZC_VAO&& vao) noexcept;
	ZC_VAO& operator = (ZC_VAO&& vao);

	~ZC_VAO();

	/*
	Unbind vertex array object.
	*/
	static void UnbindVertexArray();

	/*
	Bind vertex array object.
	*/
	void BindVertexArray() const;

	/*
	Drawings graphic objects.

	Params:
	vbo - buffer containing data.
	bufferOffset - starting offset in bytes in vbo.
	mode - OpenGL drawing mode (depends on the location of the data in vbo) : GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY GL_PATCHES.
	count - count of drawing elements (depends on mode).
	*/
	void DrawArrays(const ZC_VBO* vbo, long bufferOffset, GLenum mode, int count) const;

private:
	GLuint id = 0;
	GLuint configID;
	GLint stride = 0;

	ZC_VAO(const GLuint& _configID);
};