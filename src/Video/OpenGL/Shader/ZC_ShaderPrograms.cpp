#include <ZC/Video/OpenGL/Shader/ZC_ShaderPrograms.h>

#include <ZC/File/Read/ZC_FileReader.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>

void ZC_ShaderPrograms::Clear()
{
    if (!(vertex.empty())) vertex.clear();
    if (!(tessControl.empty())) tessControl.clear();
    if (!(tessEvaluation.empty())) tessEvaluation.clear();
    if (!(geometry.empty())) geometry.clear();
    if (!(fragment.empty())) fragment.clear();
}

ZC_ShaderProgram* ZC_ShaderPrograms::GetShader(GLenum type, int customID)
{
    switch (type)
    {
    case GL_VERTEX_SHADER: return ZC_Find(vertex, customID);
    case GL_TESS_CONTROL_SHADER: return ZC_Find(tessControl, customID);
    case GL_TESS_EVALUATION_SHADER: return ZC_Find(tessEvaluation, customID);
    case GL_GEOMETRY_SHADER: return ZC_Find(geometry, customID);
    case GL_FRAGMENT_SHADER: return ZC_Find(fragment, customID);
    default: assert(false);
    }
    return nullptr;
}

bool ZC_ShaderPrograms::AddShaderProgram(GLenum type, const ZC_ShaderInput& shaderInput)
{
    ZC_DA<char> code = ReadShaderFile(shaderInput.path.c_str());
    if (code.pHead == nullptr) return false;
    
    switch (type)
    {
    case GL_VERTEX_SHADER:
    {
        assert(!ZC_Find(vertex, shaderInput.customID));   //  double adding
        vertex.emplace_front(ZC_ShaderProgram{GL_VERTEX_SHADER, code.pHead, shaderInput.customID});
    } break;
    case GL_TESS_CONTROL_SHADER:
    {
        assert(!ZC_Find(tessControl, shaderInput.customID));   //  double adding
        tessControl.emplace_front(ZC_ShaderProgram{GL_TESS_CONTROL_SHADER, code.pHead, shaderInput.customID});
    } break;
    case GL_TESS_EVALUATION_SHADER:
    {
        assert(!ZC_Find(tessEvaluation, shaderInput.customID));   //  double adding
        tessEvaluation.emplace_front(ZC_ShaderProgram{GL_TESS_EVALUATION_SHADER, code.pHead, shaderInput.customID});
    } break;
    case GL_GEOMETRY_SHADER:
    {
        assert(!ZC_Find(geometry, shaderInput.customID));   //  double adding
        geometry.emplace_front(ZC_ShaderProgram{GL_GEOMETRY_SHADER, code.pHead, shaderInput.customID});
    } break;
    case GL_FRAGMENT_SHADER:
    {
        assert(!ZC_Find(fragment, shaderInput.customID));   //  double adding
        fragment.emplace_front(ZC_ShaderProgram{GL_FRAGMENT_SHADER, code.pHead, shaderInput.customID});
    } break;
    default: assert(false);
    }
    return true;
}

bool ZC_ShaderPrograms::CompilationTest(const char* v, const char* tc, const char* te, const char* g, const char* f)
{
    GLuint vert = 0,
        tessCon = 0,
        tessEv = 0,
        geom = 0,
        frag = 0;
    
    auto lambDeleteShaders = [vert, tessCon, tessEv, geom, frag]()
    {
        if (vert != 0) glDeleteShader(vert);
        if (tessCon != 0) glDeleteShader(tessCon);
        if (tessEv != 0) glDeleteShader(tessEv);
        if (geom != 0) glDeleteShader(geom);
        if (frag != 0) glDeleteShader(frag);
        return false;
    };

    if (v && !CompileShader(vert, GL_VERTEX_SHADER, ReadShaderFile(v).pHead)) return lambDeleteShaders();
    if (tc && !CompileShader(tessCon, GL_TESS_CONTROL_SHADER, ReadShaderFile(tc).pHead)) return lambDeleteShaders();
    if (te && !CompileShader(tessEv, GL_TESS_EVALUATION_SHADER, ReadShaderFile(te).pHead)) return lambDeleteShaders();
    if (g && !CompileShader(geom, GL_GEOMETRY_SHADER, ReadShaderFile(g).pHead)) return lambDeleteShaders();
    if (f && !CompileShader(frag, GL_FRAGMENT_SHADER, ReadShaderFile(f).pHead)) return lambDeleteShaders();

    GLuint prog = glCreateProgram();
    if (vert != 0) glAttachShader(prog, vert);
    if (tessCon != 0) glAttachShader(prog, tessCon);
    if (tessEv != 0) glAttachShader(prog, tessEv);
    if (geom != 0) glAttachShader(prog, geom);
    if (frag != 0) glAttachShader(prog, frag);
    
    glLinkProgram(prog);
    //	link check
    int success = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        char infoLog[1024];
        glGetProgramInfoLog(prog, 1024, NULL, infoLog);
        ZC_ErrorLogger::Err("glLinkProgram() fail! => " + std::string(infoLog));
    }
    // glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniforms);

    lambDeleteShaders();
    return success;
}

ZC_DA<char> ZC_ShaderPrograms::ReadShaderFile(const char* path)
{
    ZC_upFileReader upFileReader = ZC_FileReader::MakeReader(path);
    if (!upFileReader)
    {
        assert(false);  //  cant create ZC_FileReader
        return nullptr;
    }

    unsigned long fileSize = upFileReader->Size();
    if (fileSize == 0)
    {
        assert(false);  //  empty file
        return nullptr;
    }

    ZC_DA<char> fileData(fileSize + 1);
    fileData.pHead[fileSize] = '\0';
    if (upFileReader->Read(fileData.pHead , static_cast<long>(fileSize)) == 0) return nullptr;

    return fileData;
}

bool ZC_ShaderPrograms::CompileShader(GLuint& id, GLenum type, const char* code)
{
    if (!code) return false;

    id = glCreateShader(type);
    glShaderSource(id, 1, &code, nullptr);
    glCompileShader(id);
    int success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        char infoLog[1024];
        glGetShaderInfoLog(id, 1024, NULL, infoLog);
        ZC_ErrorLogger::Err("glCompileShader() fail! => " + std::string(infoLog));
        return false;
    }
    return true;
}