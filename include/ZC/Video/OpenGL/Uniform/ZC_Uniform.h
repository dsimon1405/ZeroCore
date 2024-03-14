#pragma once

#include <ZC/Tools/ZC_uptr.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
#include <ZC/Tools/Math/ZC_Mat4.h>
#include <ZC/Tools/Math/ZC_Vec2.h>

struct ZC_Uniform
{
    enum Name
    {
        unModel,
        unColor,
        unPosition,
    };

    Name name;
    GLint location = -1;

    ZC_Uniform(Name _name);
    virtual ~ZC_Uniform() = default;

    virtual void Activate() = 0;
    virtual void Set(void* _value) = 0;
    virtual const void* Get() const = 0;
    virtual ZC_uptr<ZC_Uniform> GetCopy()  const = 0;

    void GetUniformLocation(ZC_ShProg& shP);
};

struct ZC_U1uiValue : public ZC_Uniform
{
    GLuint value;

    ZC_U1uiValue(Name _name);

    void Activate() override;
    void Set(void* _value) override;
    const void* Get() const override;
    ZC_uptr<ZC_Uniform> GetCopy()  const override;
};

struct ZC_UCount : public ZC_Uniform
{
    GLsizei count;

    ZC_UCount(Name _name, GLsizei _count);
};

struct ZC_U2fvPointer : public ZC_UCount
{
    ZC_Vec2<float>* pointer;

    ZC_U2fvPointer(Name _name, GLsizei _count);

    void Activate() override;
    void Set(void* _value) override;
    const void* Get() const override;
    ZC_uptr<ZC_Uniform> GetCopy() const override;
};

struct ZC_U3fvValue : public ZC_UCount
{
    ZC_Vec3<float> value;

    ZC_U3fvValue(Name _name, GLsizei _count);

    void Activate() override;
    void Set(void* _value) override;
    const void* Get() const override;
    ZC_uptr<ZC_Uniform> GetCopy() const override;
};

struct ZC_U4fvValue : public ZC_UCount
{
    ZC_Vec4<float> value;

    ZC_U4fvValue(Name _name, GLsizei _count);

    void Activate() override;
    void Set(void* _value) override;
    const void* Get() const override;
    ZC_uptr<ZC_Uniform> GetCopy() const override;
};

struct ZC_UTranspose : public ZC_UCount
{
    GLboolean transpose;

    ZC_UTranspose(Name _name, GLsizei _count, GLboolean _transpose);
};

struct ZC_UMatrix4fvValue : public ZC_UTranspose
{
    ZC_Mat4<float> value;

    ZC_UMatrix4fvValue(Name _name, GLsizei _count, GLboolean _transpose);

    void Activate() override;
    void Set(void* _value) override;
    const void* Get() const override;
    ZC_uptr<ZC_Uniform> GetCopy() const override;
};

struct ZC_UMatrix4fvPointer : public ZC_UTranspose
{
    ZC_Mat4<float>* pointer;

    ZC_UMatrix4fvPointer(Name _name, GLsizei _count, GLboolean _transpose);

    void Activate() override;
    void Set(void* _value) override;
    const void* Get() const override;
    ZC_uptr<ZC_Uniform> GetCopy() const override;
};