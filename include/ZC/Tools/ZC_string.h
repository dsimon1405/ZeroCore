#pragma once

#include <ZC_Config.h>

#if defined(_WIN32) || defined(ZC_ANDROID_NATIVE_APP_GLUE)
	#include <string>
#else
	#include <memory>
#endif