#pragma once

#include <ZC/Tools/ZC_uptr.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>
#include <ZC/Tools/Math/ZC_Math.h>

#include <vector>

enum ZC_UniformName
{
    ZC_UN_unModel,
    ZC_UN_unColor,
    ZC_UN_unPositionWindow,
    ZC_UN_unPositionScene,
    ZC_UN_unAlpha,
    ZCR_UN_unUseLight,
};

class ZC_Uniform
{
public:
    struct NameType
    {
        ZC_UniformName name;
        bool isPointer;
        int count = 1;
        bool transponse = false;
    };

    ZC_UniformName name;

    virtual ~ZC_Uniform() = default;

    virtual void Activate() const = 0;
    virtual void Set(void* _value) = 0;
    virtual const void* Get() const = 0;
    virtual ZC_uptr<ZC_Uniform> GetCopy() const = 0;

    void GetUniformLocation(ZC_ShProg& shP);
    static std::vector<ZC_uptr<ZC_Uniform>> GetUniformVector(NameType pNameType);
    static std::vector<ZC_uptr<ZC_Uniform>> GetUniformVector(NameType* pNameType, ulong nameTypeCount);

protected:
    enum FunctionType
    {
        FT_glUniform1f,
        FT_glUniform1i,
        FT_glUniform1ui,
        FT_glUniform1fv,   //  start count
        FT_glUniform2fv,
        FT_glUniform3fv,
        FT_glUniform4fv,
        FT_glUniform1iv,
        FT_glUniform2iv,
        FT_glUniform3iv,
        FT_glUniform4iv,
        FT_glUniform1uiv,
        FT_glUniform2uiv,
        FT_glUniform3uiv,
        FT_glUniform4uiv,
        FT_glUniformMatrix2fv, //  start tansponse
        FT_glUniformMatrix3fv,
        FT_glUniformMatrix4fv,
        FT_glUniformMatrix2x3fv,
        FT_glUniformMatrix3x2fv,
        FT_glUniformMatrix2x4fv,
        FT_glUniformMatrix4x2fv,
        FT_glUniformMatrix3x4fv,
        FT_glUniformMatrix4x3fv
    };

    ZC_Uniform(ZC_UniformName _name, FunctionType _functionType);

    void GLUniform(const void* pData, int count, bool transponse) const;

private:

    int location = -1;
    FunctionType functionType;

    static ZC_uptr<ZC_Uniform> GetUpUniform(FunctionType functionType, NameType& nameType);
};

template<typename T>
struct ZC_UniformData : public ZC_Uniform
{
    ZC_UniformData(ZC_UniformName _name, FunctionType _functionType);

    void Activate() const override;
    void Set(void* _value) override;
    const void* Get() const override;
    ZC_uptr<ZC_Uniform> GetCopy() const override;

protected:
    T data;
};

template<typename T>
struct ZC_UDCount : public ZC_UniformData<T>
{
    ZC_UDCount(ZC_UniformName _name, typename ZC_Uniform::FunctionType _functionType, int _count);

    void Activate() const override;
    ZC_uptr<ZC_Uniform> GetCopy() const override;

protected:
    int count;
};

template<typename T>
struct ZC_UDCTransponse : public ZC_UDCount<T>
{
    ZC_UDCTransponse(ZC_UniformName _name, typename ZC_Uniform::FunctionType _functionType, int _count, bool _transponse);

    void Activate() const override;
    ZC_uptr<ZC_Uniform> GetCopy() const override;

private:
    bool transponse;
};


//  ZC_UniformData

template<typename T>
ZC_UniformData<T>::ZC_UniformData(ZC_UniformName _name, FunctionType _functionType)
    : ZC_Uniform(_name, _functionType)
{}

template<typename T>
void ZC_UniformData<T>::Activate() const
{
    if constexpr (std::is_pointer<T>()) this->GLUniform(data, 0, false);
    else this->GLUniform(&data, 0, false);
}

template<typename T>
void ZC_UniformData<T>::Set(void* pData)
{
    if constexpr (std::is_pointer<T>()) data = static_cast<T>(pData);
    else data = *(static_cast<T*>(pData));
}

template<typename T>
const void* ZC_UniformData<T>::Get() const
{
    if constexpr (std::is_pointer<T>()) return data;
    else return &data;
}

template<typename T>
ZC_uptr<ZC_Uniform> ZC_UniformData<T>::GetCopy() const
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_UniformData<T>>(*this);
}

//  ZC_UDCount

template<typename T>
ZC_UDCount<T>::ZC_UDCount(ZC_UniformName _name, typename ZC_Uniform::FunctionType _functionType, int _count)
    : ZC_UniformData<T>(_name, _functionType),
    count(_count)
{}

template<typename T>
void ZC_UDCount<T>::Activate() const
{
    if constexpr (std::is_pointer<T>()) this->GLUniform(this->data->Begin(), count, false);
    else this->GLUniform(this->data.Begin(), count, false);
}

template<typename T>
ZC_uptr<ZC_Uniform> ZC_UDCount<T>::GetCopy() const
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCount<T>>(*this);
}

//  ZC_UDCTransponse

template<typename T>
ZC_UDCTransponse<T>::ZC_UDCTransponse(ZC_UniformName _name, typename ZC_Uniform::FunctionType _functionType, int _count, bool _transponse)
    : ZC_UDCount<T>(_name, _functionType, _count),
    transponse(_transponse)
{}

template<typename T>
void ZC_UDCTransponse<T>::Activate() const
{
    if constexpr (std::is_pointer<T>()) this->GLUniform(this->data->Begin(), this->count, transponse);
    else this->GLUniform(this->data.Begin(), this->count, transponse);
}

template<typename T>
ZC_uptr<ZC_Uniform> ZC_UDCTransponse<T>::GetCopy() const
{
    return ZC_uptrMakeFromChild<ZC_Uniform, ZC_UDCTransponse<T>>(*this);
}