#pragma once

#include <ZC_Config.h>
#include "ZC_VAO.h"
#include "ZC_VAOConfig.h"

#include <map>

//	Wrapper OpenGL vertex array object.#include <ZC/Video/ZC_AndroidNativeAppGlue_Window.h>

class ZC_VAOs
{
public:
	ZC_VAOs() = delete;

	/*If ZC_VAO does not exist (there was no previously called GetVAO with the same vaoConfig), it is created using ZC_VAOConfig with the COPY constructor.

	Params:
	vaoConfig - vao configuration.

	Return:
	On success pointer to ZC_VAO, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	static ZC_VAO* Get(const ZC_VAOConfig& vaoConfig);

	/*
	If ZC_VAO does not exist (there was no previously called GetVAO with the same vaoConfig), it is created using ZC_VAOConfig with the MOVE constructor. If previously it was called GetVAO with the same vaoConfig, nothing happens with vaoConfig.

	Params:
	vaoConfig - vao configuration.

	Return:
	On success pointer to ZC_VAO, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
	*/
	static ZC_VAO* Get(ZC_VAOConfig&& vaoConfig);

private:
	static inline std::map<ZC_VAOConfig, ZC_VAO> vaos;

	static ZC_VAO Create(const ZC_VAOConfig& vaoConfig);

#ifdef ZC_ANDROID
    static void Reload();
#endif
};