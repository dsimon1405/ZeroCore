// #pragma once

// #include <ZC_Config.h>
// #include "ZC_VAO.h"
// #include "ZC_VAOConfig.h"

// #include <list>

// //	Wrapper OpenGL vertex array object.#include <ZC/Video/ZC_AndroidNativeAppGlue_Window.h>

// class ZC_VAOs
// {
// public:
// 	ZC_VAOs() = delete;

// 	/*If ZC_VAO does not exist (there was no previously called GetVAO with the same vaoConfig), it is created using ZC_VAOConfig with the COPY constructor.

// 	Params:
// 	vaoConfig - vao configuration.

// 	Return:
// 	On success pointer to ZC_VAO, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
// 	*/
// 	static ZC_VAO* Create();

// private:
// 	static inline std::list<ZC_VAO> vaos;

// #ifdef ZC_ANDROID
//     static void Reload();
// #endif
// };