#pragma once

#include <ZC_Config.h>
#include "ZC_Shader.h"
#include <ZC/Tools/ZC_string.h>

#include <map>

//	OpenGL shader program class.
class ZC_Shaders
{
public:
	ZC_Shaders() = delete;

	/*
    Create shader program.

    Params:
    name - shader name.
    shaderCode - code for creating shader program.

    Return:
    On success pointer to ZC_Shader, otherwise nullptr.
    */
	static ZC_Shader* Create(const std::string& name, ZC_ShaderCode&& shaderCode);

	/*
    Help to get shader.

    Params:
    name - shader name.

    Return:
    If shader exists pointer to ZC_Shader, otherwise nullptr.
    */
	static ZC_Shader* Get(const std::string& name);
	
private:
#ifdef ZC_PC
	static inline std::map<std::string, ZC_Shader> shaders;
#elif defined ZC_ANDROID
	static inline std::map<std::string, std::pair<ZC_Shader, ZC_ShaderCode>> shaders;

	static void Reload();
#endif
};