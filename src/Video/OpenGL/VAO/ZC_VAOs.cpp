#include <ZC/Video/OpenGL/VAO/ZC_VAOs.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#ifdef ZC_ANDROID
#include <Video/OpenGL/Android/ZC_AndroidOpenGLReloder.h>
#endif

ZC_VAO* ZC_VAOs::Get(const ZC_VAOConfig& vaoConfig)
{
    auto vaosIter = vaos.find(vaoConfig);
    if (vaosIter != vaos.end()) return &vaosIter->second;

    ZC_ErrorLogger::Clear();
    ZC_VAO vao = Create(vaoConfig);
    if (ZC_ErrorLogger::WasError()) return nullptr;

    return &vaos.emplace(vaoConfig, std::move(vao)).first->second;
}

ZC_VAO* ZC_VAOs::Get(ZC_VAOConfig&& vaoConfig)
{
    auto vaosIter = vaos.find(vaoConfig);
    if (vaosIter != vaos.end()) return &vaosIter->second;

    ZC_ErrorLogger::Clear();
    ZC_VAO vao = Create(vaoConfig);
    if (ZC_ErrorLogger::WasError()) return nullptr;

    return &vaos.emplace(std::move(vaoConfig), std::move(vao)).first->second;
}

ZC_VAO ZC_VAOs::Create(const ZC_VAOConfig& vaoConfig)
{
#ifdef ZC_ANDROID
    static auto connectReload = ZC_AndroidOpenGLReloader::sReload.Connect(ZC_VAOs::Reload);
#endif
    auto configID = static_cast<GLuint>(vaos.size());

    ZC_VAO vao(configID);
    vao.BindVertexArray();
    vaoConfig.Config(configID);
    ZC_VAO::UnbindVertexArray();

    if (ZC_ErrorLogger::WasError()) return vao;

    vao.stride = vaoConfig.Stride();
    
    return vao; 
}
#ifdef ZC_ANDROID
void ZC_VAOs::Reload()
{
    for (auto& vaosPair : vaos)
    {
        glGenVertexArrays(1, &vaosPair.second.id);
        vaosPair.second.BindVertexArray();
        vaosPair.first.Config(vaosPair.second.configID);
    }
    ZC_VAO::UnbindVertexArray();
}
#endif