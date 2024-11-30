#include <ZC/Video/OpenGL/Shader/ZC_ShPCompute.h>

#include <ZC/Video/OpenGL/Shader/ZC_ShPComputes.h>

ZC_ShPCompute::ZC_ShPCompute(ui_zc id_user, ui_zc _barriers, ui_zc _num_groups_x, ui_zc _num_groups_y, ui_zc _num_groups_z)
    : pShP(ZC_ShPComputes::Get(id_user)),
    barriers(_barriers),
    num_groups_x(_num_groups_x == 0 ? 1 : _num_groups_x),
    num_groups_y(_num_groups_y == 0 ? 1 : _num_groups_y),
    num_groups_z(_num_groups_z == 0 ? 1 : _num_groups_z)
{
    // int maxX, maxY, maxZ, maxItemsPerGroup;
    // glGetIntegeri_v ( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxX );
    // glGetIntegeri_v ( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxY );
    // glGetIntegeri_v ( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &maxZ );
    // glGetIntegerv   ( GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maxItemsPerGroup );

    // int maxGroupX, maxGroupY, maxGroupZ;
    // glGetIntegeri_v ( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &maxGroupX );
    // glGetIntegeri_v ( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &maxGroupY );
    // glGetIntegeri_v ( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &maxGroupZ );
    // int q = 3;
}

// bool ZC_ShPCompute::operator == (const ZC_ShPCompute& shP_c) const noexcept
// {
//     return false;    //  ALWAYS RETURN FALSE!!! The compute shader behaves differently than the pipeline and must be called every time.
// }

void ZC_ShPCompute::ActivateOpenGL() const
{
    if (!pShP) return;

    pShP->ActivateOpenGL();
    glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
    glMemoryBarrier(barriers);
}