#pragma once

#include "ZC_VAOConfig.h"

struct ZC_VAOConfigs
{
    enum Index
    {
        F_3 = 0,
        F_3__F_3,
        F_3__F_2,
        F_3__UB_3,  //  vec3 of normalized (255 -> 1.f) bytes
        F_3__UB_3__I_2_10_10_10_REV_1
    };

    static inline ZC_VAOConfig* configs = nullptr;

    bool freeMemory;

    ~ZC_VAOConfigs()
    {
        if (freeMemory) delete[] configs;
    }

    void FreeMemory()
    {
        delete[] configs;
    }

    ZC_VAOConfigs(bool _freeMemory = true)
        : freeMemory(_freeMemory)
    {
        typedef typename ZC_VAOConfig::Format Fmt;
        if (configs) return;
        configs = new ZC_VAOConfig[]
        {
            ZC_VAOConfig(ZC_DA<Fmt>(new Fmt[]{ {0, 3, GL_FLOAT, GL_FALSE} }, 1)),
            ZC_VAOConfig({new Fmt[]{ {0, 3, GL_FLOAT, GL_FALSE}, {1, 3, GL_FLOAT, GL_FALSE} }, 2}),
            ZC_VAOConfig({new Fmt[]{ {0, 3, GL_FLOAT, GL_FALSE}, {1, 2, GL_FLOAT, GL_FALSE} }, 2}),
            ZC_VAOConfig({new Fmt[]{ {0, 3, GL_FLOAT, GL_FALSE}, {1, 3, GL_UNSIGNED_BYTE, GL_TRUE} }, 2}),
            ZC_VAOConfig({new Fmt[]{ {0, 3, GL_FLOAT, GL_FALSE}, {1, 3, GL_UNSIGNED_BYTE, GL_TRUE}, {2, 4, GL_INT_2_10_10_10_REV, GL_TRUE} }, 3 }),
        };
    }
};