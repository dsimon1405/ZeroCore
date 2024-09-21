#pragma once

#include <glad/glad.h>

struct ZC_ShProg
{
    GLuint id;

    ZC_ShProg(GLuint idV, GLuint idF, GLuint idG);

	ZC_ShProg(const ZC_ShProg&) = delete;
	ZC_ShProg& operator = (const ZC_ShProg&) = delete;
	
	ZC_ShProg(ZC_ShProg&& shader) noexcept;
	ZC_ShProg& operator = (ZC_ShProg&& shader);

    ~ZC_ShProg();

		//	Activate shader program.
    void ActivateOpenGL() const;
		//	Unset activeId in the end of the draw cycle
	static void SetDefault();

	int GetUniformLocation(const char* name);

private:
    static inline GLuint activeId = 0;
};