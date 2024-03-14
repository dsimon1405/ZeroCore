#include <ZC/Video/OpenGL/Uniform/ZC_Uniform.h>

#include <Video/OpenGL/ZC_OpenGL.h>
#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

//  ZC_Uniform start

ZC_Uniform::ZC_Uniform(Name _name)
    : name(_name)
{}

void ZC_Uniform::GetUniformLocation(ZC_ShProg& shP)
{
    switch (name)
    {
        case Name::unModel: location = shP.GetUniformLocation("unModel"); break;
        case Name::unColor: location = shP.GetUniformLocation("unColor"); break;
        case Name::unPosition: location = shP.GetUniformLocation("unPosition"); break;
    }
    if (location == -1) ZC_ErrorLogger::Err("GetUniformLocation fail!", __FILE__,__LINE__);
}

//  ZC_U1uiValue start

ZC_U1uiValue::ZC_U1uiValue(Name _name)
    : ZC_Uniform(_name)
{}

void ZC_U1uiValue::Activate()
{
    glUniform1ui(location, value);
}

void ZC_U1uiValue::Set(void* _value)
{
    value = *static_cast<GLuint*>(_value);
}

const void* ZC_U1uiValue::Get() const
{
    return &value;
}

ZC_uptr<ZC_Uniform> ZC_U1uiValue::GetCopy() const
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_U1uiValue>(*this);
}

//  ZC_UCount start

ZC_UCount::ZC_UCount(Name _name, GLsizei _count)
    : ZC_Uniform(_name),
    count(_count)
{}

//  ZC_U2fvPointer start

ZC_U2fvPointer::ZC_U2fvPointer(Name _name, GLsizei _count)
    : ZC_UCount(_name, _count)
{}

void ZC_U2fvPointer::Activate()
{
    glUniform2fv(location, count, pointer->Begin());
}

void ZC_U2fvPointer::Set(void* _value)
{
    pointer = static_cast<ZC_Vec2<float>*>(_value);
}

const void* ZC_U2fvPointer::Get() const
{
    return pointer;
}

ZC_uptr<ZC_Uniform> ZC_U2fvPointer::GetCopy() const
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_U2fvPointer>(*this);
}

//  ZC_U3fvValue start

ZC_U3fvValue::ZC_U3fvValue(Name _name, GLsizei _count)
    : ZC_UCount(_name, _count)
{}

void ZC_U3fvValue::Activate()
{
    glUniform3fv(location, count, &value[0]);
}

void ZC_U3fvValue::Set(void* _value)
{
    value = *static_cast<ZC_Vec3<float>*>(_value);
}

const void* ZC_U3fvValue::Get() const
{
    return &value;
}

ZC_uptr<ZC_Uniform> ZC_U3fvValue::GetCopy() const
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_U3fvValue>(*this);
}

//  ZC_U4fvValue start

ZC_U4fvValue::ZC_U4fvValue(Name _name, GLsizei _count)
    : ZC_UCount(_name, _count)
{}

void ZC_U4fvValue::Activate()
{
    glUniform4fv(location, count, &value[0]);
}

void ZC_U4fvValue::Set(void* _value)
{
    value = *static_cast<ZC_Vec3<float>*>(_value);
}

const void* ZC_U4fvValue::Get() const
{
    return &value;
}

ZC_uptr<ZC_Uniform> ZC_U4fvValue::GetCopy() const
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_U4fvValue>(*this);
}

//  ZC_UTranspose start

ZC_UTranspose::ZC_UTranspose(Name _name, GLsizei _count, GLboolean _transpose)
    : ZC_UCount(_name, _count),
    transpose(_transpose)
{}

//  ZC_UMatrix4fvValue start

ZC_UMatrix4fvValue::ZC_UMatrix4fvValue(Name _name, GLsizei _count, GLboolean _transpose)
    : ZC_UTranspose(_name, _count, _transpose)
{}

void ZC_UMatrix4fvValue::Activate()
{
    glUniformMatrix4fv(location, count, transpose, value.Begin());
}

void ZC_UMatrix4fvValue::Set(void* _value)
{
    value = *static_cast<ZC_Mat4<float>*>(_value);
}

const void* ZC_UMatrix4fvValue::Get() const
{
    return &value;
}

ZC_uptr<ZC_Uniform> ZC_UMatrix4fvValue::GetCopy() const
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_UMatrix4fvValue>(*this);
}

//  ZC_UMatrix4fvPointer start

ZC_UMatrix4fvPointer::ZC_UMatrix4fvPointer(Name _name, GLsizei _count, GLboolean _transpose)
    : ZC_UTranspose(_name, _count, _transpose)
{}

void ZC_UMatrix4fvPointer::Activate()
{
    glUniformMatrix4fv(location, count, transpose, pointer->Begin());
}

void ZC_UMatrix4fvPointer::Set(void* _value)
{
    pointer = static_cast<ZC_Mat4<float>*>(_value);
}

const void* ZC_UMatrix4fvPointer::Get() const
{
    return pointer;
}

ZC_uptr<ZC_Uniform> ZC_UMatrix4fvPointer::GetCopy() const
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_UMatrix4fvPointer>(*this);
}