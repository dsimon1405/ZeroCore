// #include <ZC/Video/OpenGL/Buffer/ZC_Buffers.h>

// #ifdef ZC_ANDROID
// #include <Video/OpenGL/ZC_OpenGL.h>
// #include <Video/OpenGL/Android/ZC_AndroidOpenGLReloder.h>
// #endif

// ZC_Buffer* ZC_Buffers::Create(GLenum type)
// {
// #ifdef ZC_ANDROID
//     static auto connectReload = ZC_AndroidOpenGLReloader::sReload.Connect(ZC_Buffers::Reload);
// #endif
//     return &buffers.emplace_back(ZC_Buffer(type));
// }
// #ifdef ZC_ANDROID
// void ZC_Buffers::Reload()
// {
//     int vbosSize = static_cast<int>(buffers.size());
//     GLuint ids[vbosSize];
//     glGenBuffers(vbosSize, ids);
//     int idsIndex = 0;
//     for (ZC_VBO& vbo : buffers)
//     {
//         buffers.Reload(ids[idsIndex++]);
//     }
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
// }
// #endif