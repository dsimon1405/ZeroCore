#pragma once

#include "ZC_UBO.h"
#include <ZC/Tools/Function/ZC_Function.h>

#include <forward_list>

class ZC_UBOs
{
public:
	ZC_UBOs() = delete;

	/*
	Create a uniform buffer object to store data in graphics memory.

	bindingPoint - binding point in shader.
	fUpdate - function to update data in the GPU buffer (the purpose of its existence is to change data on the GPU
		only once per frame, since data can be changed on the CPU several times in one frame)

	Return:
	On success pointer to ZC_UBO, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	static ZC_UBO* Create(typename ZC_UBO::BindingPoint bindingPoint, ZC_Function<void()> fUpdate);
	static void Update();

private:
    static inline std::forward_list<std::pair<ZC_UBO, ZC_Function<void()>>> ubos;
#ifdef ZC_ANDROID
	static void Reload();
#endif
};