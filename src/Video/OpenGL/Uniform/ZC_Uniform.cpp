#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

//  ZC_Uniform start

ZC_Uniform::ZC_Uniform(Name _name, GLint _location)
    : name(_name),
    location(_location)
{}

void ZC_Uniform::GetUniformLocation(ZC_ShProg& shP)
{
    switch (name)
    {
        case Name::unModel: location = shP.GetUniformLocation("unModel"); break;
        case Name::unColor: location = shP.GetUniformLocation("unColor"); break;
    }
    if (location == -1) ZC_ErrorLogger::Err("GetUniformLocation fail!", __FILE__,__LINE__);
}

//  ZC_U1ui start

ZC_U1ui::ZC_U1ui(Name _name, GLint _location)
    : ZC_Uniform(_name, _location)
{}

void ZC_U1ui::Activate()
{
    glUniform1ui(location, value);
}

void ZC_U1ui::Set(void* _value)
{
    value = *static_cast<GLuint*>(_value);
}

void* ZC_U1ui::Get()
{
    return &value;
}

ZC_uptr<ZC_Uniform> ZC_U1ui::GetCopy()
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_U1ui>(name, location);
}

//  ZC_UCount start

ZC_UCount::ZC_UCount(Name _name, GLsizei _count, GLint _location)
    : ZC_Uniform(_name, _location),
    count(_count)
{}

//  ZC_UCount start

ZC_U3fv::ZC_U3fv(Name _name, GLsizei _count, GLint _location)
    : ZC_UCount(_name, _count, _location)
{}

//  ZC_U3fv start

void ZC_U3fv::Activate()
{
    glUniform3fv(location, count, &value[0]);
}

void ZC_U3fv::Set(void* _value)
{
    value = *static_cast<ZC_Vec3<float>*>(_value);
}

void* ZC_U3fv::Get()
{
    return &value;
}

ZC_uptr<ZC_Uniform> ZC_U3fv::GetCopy()
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_U3fv>(name, count, location);
}

//  ZC_UTranspose start

ZC_UTranspose::ZC_UTranspose(Name _name, GLsizei _count, GLboolean _transpose, GLint _location)
    : ZC_UCount(_name, _count, _location),
    transpose(_transpose)
{}

//  ZC_UMatrix4fv start

ZC_UMatrix4fv::ZC_UMatrix4fv(Name _name, GLsizei _count, GLboolean _transpose, GLint _location)
    : ZC_UTranspose(_name, _count, _transpose, _location)
{}

void ZC_UMatrix4fv::Activate()
{
    glUniformMatrix4fv(location, count, transpose, value.Begin());
}

void ZC_UMatrix4fv::Set(void* _value)
{
    value = *static_cast<ZC_Mat4<float>*>(_value);
}

void* ZC_UMatrix4fv::Get()
{
    return &value;
}

ZC_uptr<ZC_Uniform> ZC_UMatrix4fv::GetCopy()
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_UMatrix4fv>(name, count, transpose, location);
}

//  ZC_UMatrix4fvPointer start

ZC_UMatrix4fvPointer::ZC_UMatrix4fvPointer(Name _name, GLsizei _count, GLboolean _transpose, GLint _location)
    : ZC_UTranspose(_name, _count, _transpose, _location)
{}

void ZC_UMatrix4fvPointer::Activate()
{
    glUniformMatrix4fv(location, count, transpose, value->Begin());
}

void ZC_UMatrix4fvPointer::Set(void* _value)
{
    value = static_cast<ZC_Mat4<float>*>(_value);
}

void* ZC_UMatrix4fvPointer::Get()
{
    return value;
}

ZC_uptr<ZC_Uniform> ZC_UMatrix4fvPointer::GetCopy()
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_UMatrix4fvPointer>(name, count, transpose, location);
}