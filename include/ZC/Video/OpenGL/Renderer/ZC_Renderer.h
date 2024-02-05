#pragma once

#include "ZC_RendererSet.h"

#include <map>

struct ZC_RMode
{
    virtual ~ZC_RMode() = default;
    virtual void Start() const {};
    virtual void End() const {};
};
// struct ZC_RMDepthLEQUAL : ZC_RMode
// {
//     ~ZC_RMDepthLEQUAL() override = default;

//     void Start() const override
//     {
//         glDepthFunc(GL_LESS);
//     }

//     void End() const override
//     {
//         glDepthFunc(GL_LESS);
//     }
// };

struct ZC_Renderer
{
    struct ModeSet
    {
        unsigned char level;
        ZC_uptr<ZC_RMode> upRMode;

        ModeSet(unsigned char _level, ZC_uptr<ZC_RMode> _upMode = nullptr);

        ModeSet(ModeSet&& ms);

        bool operator < (const ModeSet& ms) const noexcept;
    };

    static inline std::map<ModeSet, std::forward_list<ZC_RendererSet*>> rendSets;

    ZC_Renderer() = delete;

    static void DrawAll();
    static void Add(ZC_RendererSet* pRS);
    static void Erase(ZC_RendererSet* pRS);
    static void EnablePointSize();
};