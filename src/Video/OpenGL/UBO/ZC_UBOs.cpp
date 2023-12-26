#include <ZC/Video/OpenGL/UBO/ZC_UBOs.h>

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#ifdef ZC_ANDROID
#include <Video/OpenGL/ZC_OpenGL.h>
#include <Video/OpenGL/Android/ZC_AndroidOpenGLReloder.h>
#endif

ZC_UBO* ZC_UBOs::Create(GLuint bindingPoint)
{
#ifdef ZC_ANDROID
    static auto connectReload = ZC_AndroidOpenGLReloader::sReload.Connect(ZC_UBOs::Reload);
#endif
    for (auto& ubo : ubos)
    {
        if (ubo.bindingPoint == bindingPoint)
        {
		    ZC_ErrorLogger::Err("ZC_UBO allready exists with bindingPoint = " + std::to_string(bindingPoint), __FILE__, __LINE__);
            return nullptr;
        }
    }
    return &ubos.emplace_back(ZC_UBO(GL_UNIFORM_BUFFER, bindingPoint));
}
#ifdef ZC_ANDROID
void ZC_UBOs::Reload()
{
    int ubosSize = static_cast<int>(ubos.size());
    GLuint ids[ubosSize];
    glGenBuffers(ubosSize, ids);
    int idsIndex = 0;
    for (ZC_UBO& ubo : ubos)
    {
        ubo.Reload(ids[idsIndex++]);
	    glBindBufferRange(GL_UNIFORM_BUFFER, ubo.bindingPoint, ubo.id, 0, static_cast<GLsizeiptr>(ubo.Size()));
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
#endif