#include <ZC/Video/OpenGL/Shader/ZC_ShLoader.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/File/Read/ZC_FileReader.h>

ZC_ShLoader::~ZC_ShLoader()
{
    for (size_t i; i < shLoadSets.size; ++i)
        if (shLoadSets[i].type == GL_FRAGMENT_SHADER && shLoadSets[i].subData)
            delete static_cast<ZC_TexSets*>(shLoadSets[i].subData);
}

typename ZC_ShLoader::ShData ZC_ShLoader::Get(Name v, Name f, Name g)
{
    ShData shData;
    Load(v, shData.pV, shData.pVaoCon, shData.pUniformsV);
    Load(f, shData.pF, shData.pTexSets, shData.pUniformsF);
    // if (g != ZC_ShName::none)
    // {
    //     std::list<const char*>* garbage1;
    //     std::forward_list<const char*>* garbage2;
    //     Load(frags, g, shData.pG, GL_GEOMETRY_SHADER, garbage1, garbage2);
    // }
    return shData;
}

ZC_DA<char> ZC_ShLoader::ReadShaderFile(const char* path, GLenum type)
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

void ZC_ShLoader::FillShaderStart(char* shaderData, const std::string& shaderStart) noexcept
{
    for (size_t i = 0; i < shaderStart.size(); ++i)
    {
        shaderData[i] = shaderStart[i];
    }
}


//  Set start

ZC_ShLoader::Set::Set(ZC_Shader&& _shader, void* _subData, ZC_DA<ZC_uptr<ZC_Uniform>>* _pUniforms)
    : shader(std::move(_shader)),
    subData(_subData),
    pUniforms(_pUniforms)
{}

ZC_ShLoader::Set::Set(Set&& s)
    : shader(std::move(s.shader)),
    subData(s.subData),
    pUniforms(s.pUniforms)
{}