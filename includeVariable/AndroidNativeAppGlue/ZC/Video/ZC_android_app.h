#pragma once

#include <ZC_Config.h>

#include <android_native_app_glue.h>

class ZC_android_app
{
public:
    ZC_android_app(android_app* _pAndroidApp);

    virtual ~ZC_android_app() = default;

protected:
    static inline android_app* pAndroidApp = nullptr;

    ZC_android_app() = default;
};