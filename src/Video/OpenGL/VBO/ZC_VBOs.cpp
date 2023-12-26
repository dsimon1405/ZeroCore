#include <ZC/Video/OpenGL/VBO/ZC_VBOs.h>

#ifdef ZC_ANDROID
#include <Video/OpenGL/ZC_OpenGL.h>
#include <Video/OpenGL/Android/ZC_AndroidOpenGLReloder.h>
#endif

ZC_VBO* ZC_VBOs::Create()
{
#ifdef ZC_ANDROID
    static auto connectReload = ZC_AndroidOpenGLReloader::sReload.Connect(ZC_VBOs::Reload);
#endif
    return &vbos.emplace_back(ZC_VBO(GL_ARRAY_BUFFER));
}
#ifdef ZC_ANDROID
void ZC_VBOs::Reload()
{
    int vbosSize = static_cast<int>(vbos.size());
    GLuint ids[vbosSize];
    glGenBuffers(vbosSize, ids);
    int idsIndex = 0;
    for (ZC_VBO& vbo : vbos)
    {
        vbo.Reload(ids[idsIndex++]);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
#endif