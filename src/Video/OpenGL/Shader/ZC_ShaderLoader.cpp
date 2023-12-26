#include <ZC/Video/OpenGL/Shader/ZC_ShaderLoader.h>

#include <ZC/File/Read/ZC_FileReader.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>
#include <Video/OpenGL/ZC_OpenGL.h>

ZC_ShaderCode ZC_ShaderLoader::LoadShaderCode(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{

    ZC_DynamicArray<char> vertexCode = ReadShaderFile(vertexPath, ShaderType::Vertex);
    if (!vertexCode.pHead) return {};

    ZC_DynamicArray<char> fragmentCode = ReadShaderFile(fragmentPath, ShaderType::Fragment);
    if (!fragmentCode.pHead) return {};

    ZC_DynamicArray<char> geometryCode;
    if (geometryPath)
    {
        geometryCode = ReadShaderFile(geometryPath, ShaderType::Geometry);
        if (!geometryCode.pHead) return {};
    }

    return ZC_ShaderCode(std::move(vertexCode), std::move(fragmentCode), std::move(geometryCode));
}

ZC_DynamicArray<char> ZC_ShaderLoader::ReadShaderFile(const char* path, ShaderType shaderType)
{

#ifdef ZC_PC
    static const std::string vertexStart = "#version " + std::to_string(ZC_OPEN_GL_MAJOR_VERSION)
            + std::to_string(ZC_OPEN_GL_MINOR_VERSION) + "0 core\n";
    static const std::string fragmentStart = vertexStart;
#elif defined ZC_ANDROID
    static const std::string vertexStart = "#version " + std::to_string(ZC_OPEN_GL_MAJOR_VERSION)
                                                  + std::to_string(ZC_OPEN_GL_MINOR_VERSION) + "0 es\n";
    static const std::string fragmentStart = vertexStart + "precision mediump float;";
#endif
    static const std::string geometryStart = vertexStart;

    ZC_upFileReader upFileReader = ZC_FileReader::MakeReader(path);
    if (!upFileReader) return nullptr;

    size_t shaderStartSize = 0;
    switch (shaderType)
    {
        case ShaderType::Vertex:
        {
            static const size_t size = vertexStart.size();
            shaderStartSize = size;
            break;
        }
        case ShaderType::Geometry:
        {
            static const size_t size = geometryStart.size();
            shaderStartSize = size;
            break;
        }
        case ShaderType::Fragment:
        {
            static const size_t size = fragmentStart.size();
            shaderStartSize = size;
            break;
        }
    }

    ZC_ErrorLogger::Clear();
    size_t fileSize = upFileReader->Size();
    if (fileSize == 0)
    {
        if (ZC_ErrorLogger::WasError()) return nullptr;

        ZC_ErrorLogger::Err("Empty fIle: " + std::string(path));
        return nullptr;
    }

    ZC_DynamicArray<char> fileData(shaderStartSize + fileSize + 1);
    fileData.pHead[fileSize + shaderStartSize] = '\0';
    if (upFileReader->Read(fileData.pHead + shaderStartSize, static_cast<long>(fileSize)) == 0) return nullptr;

    switch (shaderType)
    {
        case ShaderType::Vertex:
            FillShaderStart(fileData.pHead, vertexStart);
            break;
        case ShaderType::Geometry:
            FillShaderStart(fileData.pHead, geometryStart);
            break;
        case ShaderType::Fragment:
            FillShaderStart(fileData.pHead, fragmentStart);
            break;
    }

    return fileData;
}

void ZC_ShaderLoader::FillShaderStart(char* shaderData, const std::string& shaderStart) noexcept
{
    for (size_t i = 0; i < shaderStart.size(); ++i)
    {
        shaderData[i] = shaderStart[i];
    }
}