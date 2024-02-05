#include <ZC/Video/OpenGL/VAO/ZC_VAOs.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#ifdef ZC_ANDROID
#include <Video/OpenGL/Android/ZC_AndroidOpenGLReloder.h>
#endif

ZC_VAO* ZC_VAOs::Create()
{
    return &vaos.emplace_back();
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