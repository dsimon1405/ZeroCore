#pragma once

#include <ZC/Video/OpenGL/GL/glcorearb.h>
#include <ZC/Video/OpenGL/Buffer/ZC_Buffer.h>
#include "ZC_VAOConfig.h"

//	Wrapper OpenGL vertex array object.
struct ZC_VAO
{
	GLuint id = 0;
#ifdef ZC_ANDROID
	//	ZC_VAOConfig* config, ZC_Buffer* vbo, ZC_Buffer* ebo
#endif
	ZC_VAO();

	ZC_VAO(const ZC_VAO&) = delete;
	ZC_VAO& operator = (const ZC_VAO&) = delete;

	ZC_VAO(ZC_VAO&& vao) noexcept;
	ZC_VAO& operator = (ZC_VAO&& vao);

	~ZC_VAO();

    /*
    Configuring VAO.
	
	ZC_VAOConfig* config - pointer to vao for configuration.
	ZC_Buffer* vbo - pointer to vbo for binding.
	ZC_Buffer* ebo - pointer to ebo for bindig.
    startOffset - start offset in buffer in bytes.
    verticesCount — number of vertices. The value should be 0 if the data in vbo is stored in VNCVNC style. If the data stored in vbo is in VVNNCC, the value cannot be 0.
    */
	void Config(ZC_VAOConfig* config, ZC_Buffer& vbo, ZC_Buffer& ebo, GLuint startOffset, GLuint verticesCount);
	void BindVertexArray();
};