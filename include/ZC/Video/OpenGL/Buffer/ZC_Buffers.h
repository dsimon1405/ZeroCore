// #pragma once

// #include "ZC_Buffer.h"

// #include <list>

// class ZC_Buffers
// {
// public:
// 	ZC_Buffers() = delete;

// 	/*
// 	Create a vertex buffer object to store data in graphics memory.

// 	Return:
// 	On success pointer to ZC_VBO, otherwise nullptr (in second case ZC_ErrorLogger::ErrorMessage() - for more information).
// 	*/
// 	static ZC_Buffer* Create(GLenum type);

// private:
//     static inline std::list<ZC_Buffer> buffers;
// #ifdef ZC_ANDROID
// 	static void Reload();
// #endif
// };