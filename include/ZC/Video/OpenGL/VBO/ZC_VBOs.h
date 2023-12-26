#pragma once

#include "ZC_VBO.h"

#include <list>

class ZC_VBOs
{
public:
	ZC_VBOs() = delete;

	/*
	Create a vertex buffer object to store data in graphics memory.

	Return:
	On success pointer to ZC_VBO, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	static ZC_VBO* Create();

private:
    static inline std::list<ZC_VBO> vbos;
#ifdef ZC_ANDROID
	static void Reload();
#endif
};