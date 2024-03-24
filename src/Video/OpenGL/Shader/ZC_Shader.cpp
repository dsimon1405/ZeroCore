#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>

#include <ZC/Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <ZC/File/Read/ZC_FileReader.h>
    
ZC_Shader::ZC_Shader(const char* code, GLenum type)
{
    id = glCreateShader(type);
    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);
    int success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(id, 1024, NULL, infoLog);
        ZC_ErrorLogger::Err("glCompileShader() fail! => " + std::string(infoLog));
    }
}

ZC_Shader::ZC_Shader(ZC_Shader&& sh)
    : id(sh.id)
{
    sh.id = 0;
}

ZC_Shader::~ZC_Shader()
{
    glDeleteShader(id);
}

ZC_DA<char> ZC_Shader::ReadShaderFile(const char* path, GLenum type)
{
// #ifdef ZC_PC
//     static const std::string vertexStart = "#version " + std::to_string(ZC_OPEN_GL_MAJOR_VERSION)
//             + std::to_string(ZC_OPEN_GL_MINOR_VERSION) + "0 core\n";
//     static const std::string fragmentStart = vertexStart;
// #elif defined ZC_ANDROID
//     static const std::string vertexStart = "#version " + std::to_string(ZC_OPEN_GL_MAJOR_VERSION)
//                                                   + std::to_string(ZC_OPEN_GL_MINOR_VERSION) + "0 es\n";
//     static const std::string fragmentStart = vertexStart + "precision mediump float;";
// #endif
//     static const std::string geometryStart = vertexStart;

//     size_t shaderStartSize = 0;
//     switch (type)
//     {
//         case GL_VERTEX_SHADER:
//         {
//             static const size_t size = vertexStart.size();
//             shaderStartSize = size;
//             break;
//         }
//         case GL_FRAGMENT_SHADER:
//         {
//             static const size_t size = fragmentStart.size();
//             shaderStartSize = size;
//             break;
//         }
//         case GL_GEOMETRY_SHADER:
//         {
//             static const size_t size = geometryStart.size();
//             shaderStartSize = size;
//             break;
//         }
//     }

    ZC_upFileReader upFileReader = ZC_FileReader::MakeReader(path);
    if (!upFileReader) return nullptr;

    ZC_ErrorLogger::Clear();
    size_t fileSize = upFileReader->Size();
    if (fileSize == 0)
    {
        if (ZC_ErrorLogger::WasError()) return nullptr;

        ZC_ErrorLogger::Err("Empty fIle: " + std::string(path));
        return nullptr;
    }

    ZC_DA<char> fileData(
        // shaderStartSize + 
        fileSize + 1);
    fileData.pHead[fileSize
    //  + shaderStartSize
    ] = '\0';
    if (upFileReader->Read(fileData.pHead
    //  + shaderStartSize
    , static_cast<long>(fileSize)) == 0) return nullptr;

    // switch (type)
    // {
    //     case GL_VERTEX_SHADER:
    //         FillShaderStart(fileData.pHead, vertexStart);
    //         break;
    //     case GL_FRAGMENT_SHADER:
    //         FillShaderStart(fileData.pHead, fragmentStart);
    //         break;
    //     case GL_GEOMETRY_SHADER:
    //         FillShaderStart(fileData.pHead, geometryStart);
    //         break;
    // }

    return fileData;
}

// void ZC_Shader::FillShaderStart(char* shaderData, const std::string& shaderStart) noexcept
// {
//     for (size_t i = 0; i < shaderStart.size(); ++i)
//     {
//         shaderData[i] = shaderStart[i];
//     }
// }