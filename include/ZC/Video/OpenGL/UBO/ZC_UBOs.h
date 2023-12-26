#pragma once

#include "ZC_UBO.h"

#include <list>

class ZC_UBOs
{
public:
	ZC_UBOs() = delete;

	/*
	Create a uniform buffer object to store data in graphics memory.

	Return:
	On success pointer to ZC_UBO, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	static ZC_UBO* Create(GLuint bindingPoint);

private:
    static inline std::list<ZC_UBO> ubos;
#ifdef ZC_ANDROID
	static void Reload();
#endif
};