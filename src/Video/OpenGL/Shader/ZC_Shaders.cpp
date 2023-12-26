#include <ZC/Video/OpenGL/Shader/ZC_Shaders.h>

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#ifdef ZC_ANDROID
#include <Video/OpenGL/Android/ZC_AndroidOpenGLReloder.h>
#endif

ZC_Shader* ZC_Shaders::Create(const std::string& name, ZC_ShaderCode&& shaderCode)
{
#ifdef ZC_ANDROID
    static auto connectReload = ZC_AndroidOpenGLReloader::sReload.Connect(ZC_Shaders::Reload);
#endif
	if (shaders.find(name) != shaders.end())
	{
		ZC_ErrorLogger::Err("Already exists shader: " + name, __FILE__, __LINE__);
		return nullptr;
	}
	ZC_ErrorLogger::Clear();
	ZC_Shader shader(shaderCode);
	if (ZC_ErrorLogger::WasError()) return nullptr;
#ifdef ZC_PC
	return &shaders.emplace(name, std::move(shader)).first->second;
#elif defined ZC_ANDROID
	return &shaders.emplace(name, std::make_pair<ZC_Shader, ZC_ShaderCode>(std::move(shader), std::move(shaderCode))).first->second.first;
#endif
}

ZC_Shader* ZC_Shaders::Get(const std::string& name)
{
	auto shadersIter = shaders.find(name);
	if (shadersIter == shaders.end()) return nullptr;
#ifdef ZC_PC
	return &shadersIter->second;
#elif defined ZC_ANDROID
	return &shadersIter->second.first;
#endif
}
#ifdef ZC_ANDROID
void ZC_Shaders::Reload()
{
	for (auto& shadersPair : shaders)
	{
		shadersPair.second.first.CreateShaderProgram(shadersPair.second.second);
	}
}
#endif