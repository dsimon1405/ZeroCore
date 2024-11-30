#version 460 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;


    //  in
layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspective;
    mat4 perspView;
    mat4 perspViewSkybox;
    mat4 view;
    vec3 camPos;
} camera;

#define BIND_SSBO_PARTICLE 0
#define BIND_SSBO_TEX_DATA 1
    //  texture data
struct UV
{
    float left_x;
    float top_y;
    float right_x;
    float bottom_y;
};
layout (std430, binding = BIND_SSBO_TEX_DATA) readonly buffer SSBO_UV
{
    uint uvs_count;     //  tiles count
    UV uvs[];
} ssbo_uv;

    //  particle
        //  move space, look enum G_Particles::G_ParticleMoveSpace
#define G_PLS__particles_space 0
#define G_PLS__world_space 1

struct Particle    //  std 430 to avoid problems with alignment, don't use mat and vec types!
{
    float life_secs_total;
    float life_secs_cur;

    float life_time_alpha;

    float secs_to_start;
    
        //  position
    float pos_start[3];
    float pos_cur[3];
    
    float dir_move_normalized[3];
    float move_speed_secs;

        //  animaion, uv
    uint uvs_start_id;      //  updated on life time end of the particle
    uint uvs_cur_id;
};
layout (std430, binding = BIND_SSBO_PARTICLE) buffer SSBO_ParticleSystem
{
            //  Update every frame on the cpu
        //  time
    float prev_frame_secs;
    float total_secs;   //  seconds from the start of particle system drawing
        //  origin pos
                //  may be changed
    float mat_model[4][4];   //  may be located in other SSBOs and one calculated system may be use in different places

            //  Update only on configuration
        //  corners rotated frace to cam
    float bl[3];
    float br[3];
    float tl[3];
    float tr[3];
        //  particle size for corners calculation
    float half_width;
    float half_height;
        //  alpha data
    float appear_secs;
    float disappear_secs;

        //  position
    int life_space;     //  look enum G_ParticleLifeSpace
            //  move
    float move_speed_power;     //  total move speed of all particles
    
        //  Animation
    float uv_shift_speed;   //  1 / uv_per_second

    Particle particles[];
} ssbo_ps;

void main()
{
    uint id = gl_GlobalInvocationID.x;    //  use only glDispatchCompute(X, 1, 1). And use only layout(local_size_x, 1, 1)

    if (id == 0)   //  on first vertex rotate corners to cam, they are same for each particle
    {
        vec3 cam_right = vec3(camera.view[0].x, camera.view[1].x, camera.view[2].x);     //  normalized
        vec3 cam_up = vec3(camera.view[0].y, camera.view[1].y, camera.view[2].y);        //  normalized

        float left_x    = -1.f;
        float right_x   =  1.f;
        float top_y     =  1.f;
        float bottom_y  = -1.f;

        vec3 bl = (cam_right * left_x * ssbo_ps.half_width) + (cam_up * bottom_y * ssbo_ps.half_height);
        ssbo_ps.bl[0] = bl.x;
        ssbo_ps.bl[1] = bl.y;
        ssbo_ps.bl[2] = bl.z;

        vec3 br = (cam_right * right_x * ssbo_ps.half_width) + (cam_up * bottom_y * ssbo_ps.half_height);
        ssbo_ps.br[0] = br.x;
        ssbo_ps.br[1] = br.y;
        ssbo_ps.br[2] = br.z;

        vec3 tl = (cam_right * left_x * ssbo_ps.half_width) + (cam_up * top_y * ssbo_ps.half_height);
        ssbo_ps.tl[0] = tl.x;
        ssbo_ps.tl[1] = tl.y;
        ssbo_ps.tl[2] = tl.z;

        vec3 tr = (cam_right * right_x * ssbo_ps.half_width) + (cam_up * top_y * ssbo_ps.half_height);
        ssbo_ps.tr[0] = tr.x;
        ssbo_ps.tr[1] = tr.y;
        ssbo_ps.tr[2] = tr.z;
    }

    Particle p = ssbo_ps.particles[id];

    if (ssbo_ps.total_secs < p.secs_to_start)      //  life has not yet begun
    {
        return;
    }
        //  get pos
    vec3 pos_cur = vec3(p.pos_cur[0], p.pos_cur[1], p.pos_cur[2]);

        //  life time
    float life_secs_cur = p.life_secs_cur + ssbo_ps.prev_frame_secs;

        //  uv coord
    uint uvs_cur_id = p.uvs_start_id + uint(life_secs_cur / ssbo_ps.uv_shift_speed);
                                                            //  repeats amount              total amount   
    if (uvs_cur_id >= ssbo_uv.uvs_count) uvs_cur_id -= (uvs_cur_id / ssbo_uv.uvs_count) * ssbo_uv.uvs_count;   //  avoid out of range uvs[]

        //  particle spawn
    if (life_secs_cur > p.life_secs_total)  //  life ended
    {
        life_secs_cur -= p.life_secs_total;

        pos_cur = vec3(p.pos_start[0], p.pos_start[1], p.pos_start[2]);
        if (ssbo_ps.life_space == G_PLS__world_space)     //  transfer partical from particles local start pos to world pos
        {
            mat4 mat_model = mat4(
                    ssbo_ps.mat_model[0][0], ssbo_ps.mat_model[0][1], ssbo_ps.mat_model[0][2], ssbo_ps.mat_model[0][3],
                    ssbo_ps.mat_model[1][0], ssbo_ps.mat_model[1][1], ssbo_ps.mat_model[1][2], ssbo_ps.mat_model[1][3],
                    ssbo_ps.mat_model[2][0], ssbo_ps.mat_model[2][1], ssbo_ps.mat_model[2][2], ssbo_ps.mat_model[2][3],
                    ssbo_ps.mat_model[3][0], ssbo_ps.mat_model[3][1], ssbo_ps.mat_model[3][2], ssbo_ps.mat_model[3][3]
                );
            pos_cur = vec3(mat_model * vec4(pos_cur, 1.f));
        }

        ssbo_ps.particles[id].uvs_start_id = uvs_cur_id;      //  update start id
    }

        //  calculate pos
    vec3 dir_move_normalized = vec3(ssbo_ps.particles[id].dir_move_normalized[0], ssbo_ps.particles[id].dir_move_normalized[1],
        ssbo_ps.particles[id].dir_move_normalized[2]);
    pos_cur += dir_move_normalized * p.move_speed_secs * ssbo_ps.move_speed_power * ssbo_ps.prev_frame_secs;

        //  alpha
    float disappear_start_secs = p.life_secs_total - ssbo_ps.disappear_secs;
    float life_time_alpha = life_secs_cur < ssbo_ps.appear_secs ? life_secs_cur / ssbo_ps.appear_secs  //  particle appear (life start)
        : disappear_start_secs < life_secs_cur ? 1.f - ((life_secs_cur - disappear_start_secs) / ssbo_ps.disappear_secs)  //  particle dissapear (life end)
        : 1.f;  //  full seen

        //  set data
    ssbo_ps.particles[id].life_secs_cur = life_secs_cur;
    ssbo_ps.particles[id].pos_cur[0] = pos_cur.x;
    ssbo_ps.particles[id].pos_cur[1] = pos_cur.y;
    ssbo_ps.particles[id].pos_cur[2] = pos_cur.z;
    ssbo_ps.particles[id].uvs_cur_id = uvs_cur_id;
    ssbo_ps.particles[id].life_time_alpha = life_time_alpha;
}