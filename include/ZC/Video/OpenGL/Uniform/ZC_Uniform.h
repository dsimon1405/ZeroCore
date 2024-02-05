#pragma once

#include <ZC/Tools/ZC_uptr.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
#include <ZC/Tools/Math/ZC_Mat4.h>

struct ZC_Uniform
{
    enum Name
    {
        model,
        color
    };

    Name name;
    GLint location;

    ZC_Uniform(Name _name, GLint _location);
    virtual ~ZC_Uniform() = default;

    virtual void Activate() = 0;
    virtual void Set(void* _value) = 0;
    virtual void* Get() = 0;
    virtual ZC_uptr<ZC_Uniform> GetCopy() = 0;

    void GetUniformLocation(ZC_ShProg& shP);
};

struct ZC_UCount : public ZC_Uniform
{
    GLsizei count;

    ZC_UCount(Name _name, GLsizei _count, GLint _location);
    ~ZC_UCount() override = default;
};

struct ZC_U3fv : public ZC_UCount
{
    ZC_Vec3<float> value;

    ZC_U3fv(Name _name, GLsizei _count, GLint _location = -1);
    ~ZC_U3fv() override = default;

    void Activate() override;
    void Set(void* _value) override;
    void* Get() override;
    ZC_uptr<ZC_Uniform> GetCopy() override;
};

struct ZC_UTranspose : public ZC_UCount
{
    GLboolean transpose;

    ZC_UTranspose(Name _name, GLsizei _count, GLboolean _transpose, GLint _location);
    ~ZC_UTranspose() override = default;
};

struct ZC_UMatrix4fv : public ZC_UTranspose
{
    ZC_Mat4<float> value;

    ZC_UMatrix4fv(Name _name, GLsizei _count, GLboolean _transpose, GLint _location = -1);
    ~ZC_UMatrix4fv() override = default;

    void Activate() override;
    void Set(void* _value) override;
    void* Get() override;
    ZC_uptr<ZC_Uniform> GetCopy() override;
};