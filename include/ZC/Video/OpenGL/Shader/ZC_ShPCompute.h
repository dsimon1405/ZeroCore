#pragma once

#include <ZC/ZC_Types.h>
#include <ZC/Video/OpenGL/Shader/ZC_ShProg.h>

struct ZC_ShPCompute
{
    ZC_ShPCompute() = default;
    /*
    Params:
    - id_user - id on load to ZC_ShPComputes.
    - _barriers - combination of: GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT, GL_ELEMENT_ARRAY_BARRIER_BIT, GL_UNIFORM_BARRIER_BIT, GL_TEXTURE_FETCH_BARRIER_BIT, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,
        GL_COMMAND_BARRIER_BIT, GL_PIXEL_BUFFER_BARRIER_BIT, GL_TEXTURE_UPDATE_BARRIER_BIT, GL_BUFFER_UPDATE_BARRIER_BIT, GL_FRAMEBUFFER_BARRIER_BIT, GL_TRANSFORM_FEEDBACK_BARRIER_BIT,
        GL_ATOMIC_COUNTER_BARRIER_BIT, GL_SHADER_STORAGE_BARRIER_BIT. Or GL_ALL_BARRIER_BITS.
    - _num_groups_x - number of work groups to be launched in the X dimension.
    - _num_groups_y - number of work groups to be launched in the Y dimension.
    - _num_groups_z - number of work groups to be launched in the Z dimension.
    */
    ZC_ShPCompute(ui_zc id_user, ui_zc _barriers, ui_zc _num_groups_x, ui_zc _num_groups_y, ui_zc _num_groups_z);
        //  ALWAYS RETURN FALSE!!! The compute shader behaves differently than the pipeline and must be called every time.
    // bool operator == (const ZC_ShPCompute& shP_c) const noexcept;

    void ActivateOpenGL() const;

    ZC_ShProg* pShP = nullptr;
    ui_zc barriers = 0;
    ui_zc num_groups_x = 0;
    ui_zc num_groups_y = 0;
    ui_zc num_groups_z = 0;
};