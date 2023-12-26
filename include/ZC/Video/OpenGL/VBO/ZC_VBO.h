#pragma once

#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>

#include <utility>

class ZC_VBOs;

//	Wrapper OpenGL vertex buffer object.
class ZC_VBO : public ZC_Buffer
{
	friend class ZC_VBOs;
public:
	ZC_VBO(ZC_VBO&& vbo) noexcept;
	ZC_VBO& operator = (ZC_VBO&& vbo);

    ~ZC_VBO() override = default;

	/*
	Maybe baby
	*/
	void BindVertexBuffer(GLuint vaoConfig, long offset, int stride) const;

private:
	ZC_VBO(GLenum type);
};