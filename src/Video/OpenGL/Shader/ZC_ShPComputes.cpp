#include <ZC/Video/OpenGL/Shader/ZC_ShPComputes.h>

#include <ZC/Video/OpenGL/Shader/ZC_Shader.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>

bool ZC_ShPComputes::Load(i_zc id_user, const char* path)
{
    ZC_DA<char> compute_code = ZC_Shader::ReadShaderFile(path, 0);  //  second param is legacy
    if (compute_code.pHead == nullptr) return false;
    
    ZC_ShProg shProg(compute_code.pHead);
    if (shProg.id == 0) return false;

    sh_progs.emplace_front(Id_ShP{ id_user, std::move(shProg) });
    return true;
}

ZC_ShProg* ZC_ShPComputes::Get(i_zc id_user)
{
    Id_ShP* pId_shP = ZC_Find(sh_progs, id_user);
    assert(pId_shP);    //  can't find compute shader
    return pId_shP ? &(pId_shP->pShP) : nullptr;
}


    //  ZC_ShPComputes::Id_ShP

bool ZC_ShPComputes::Id_ShP::operator == (i_zc _id_user) const noexcept
{
    return id_user == _id_user;
}