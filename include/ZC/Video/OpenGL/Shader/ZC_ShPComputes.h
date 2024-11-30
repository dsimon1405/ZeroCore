#pragma once

#include <ZC/ZC_Types.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>

#include <forward_list>

class ZC_ShPComputes
{
public:
    ZC_ShPComputes() = delete;

    static bool Load(i_zc id_user, const char* path);
    static ZC_ShProg* Get(i_zc id_user);

private:
    struct Id_ShP
    {
        i_zc id_user;
        ZC_ShProg pShP;

        bool operator == (i_zc _id_user) const noexcept;
    };

    static inline std::forward_list<Id_ShP> sh_progs;
};