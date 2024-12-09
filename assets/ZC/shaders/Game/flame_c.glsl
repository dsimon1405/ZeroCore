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
struct Particle    //  std 430 to avoid problems with alignment, don't use mat and vec types!
{
        //  life time
    float life_time_secs_to_start;   //  secs to start life time
    float life_time_secs_total;      //  life time total secs
    float life_time_secs_cur;        //  life time cur secs
        //  position
    float pos_start[3];
    float pos_cur[3];
        //  visibility
    float visibility_alpha;
        //  move
    float move_dir_normalized[3];
    float move_speed_secs;
        //  animaion, uv
    uint uvs_start_id;      //  updated on life time end of the particle
    uint uvs_cur_id;
};

    //  see enum G_PS_Source::Move::DirectionType
#define DT__from_particles_center   0
#define DT__variable_is_direction   1
#define DT__variable_is_destination 2

layout (std430, binding = BIND_SSBO_PARTICLE) buffer SSBO_ParticleSystem
{
            //  Update every frame on the cpu
        //  time
    float time_prev_frame_secs;
    float time_total_secs;   //  seconds from the start of particle system drawing
        //  origin pos
                //  may be changed
    float spawn_mat_model[4][4];   //  may be located in other SSBOs and one calculated system may be use in different places

            //  Update only on configuration
        //  corners rotated frace to cam
    float size_bl[3];
    float size_br[3];
    float size_tl[3];
    float size_tr[3];
        //  particle size for corners calculation
    float size_half_width;
    float size_half_height;
        //  visibility
    float visibility_appear_secs;
    float visibility_disappear_secs;
        //  move
    int move_direction_type;     //  see G_PS_Source::Move::DirectionType
    float move_variable[3];       //  see G_PS_Source::Move::DirectionType
    float move_speed_power;       //  total move speed of all particles
        //  animation
    float uv_shift_speed;   //  1 / uv_per_second

    Particle particles[];
} ssbo_ps;


    //  functions
void CalcCornersRotatedToCam();
void CalcPosAndDirOnSpawn(uint particle_id);
void Set_pos_cur(uint particle_id, vec3 pos_cur);
void Set_move_dir_normalized(uint particle_id, vec3 move_dir_normalized);

void main()
{
    uint id = gl_GlobalInvocationID.x;    //  use only glDispatchCompute(X, 1, 1). And use only layout(local_size_x, 1, 1)

    if (id == 0) CalcCornersRotatedToCam();   //  on first vertex rotate corners to cam, they are same for each particle
    if (ssbo_ps.time_prev_frame_secs == ssbo_ps.time_total_secs) CalcPosAndDirOnSpawn(id);     //  start of particles system life, calculate particles pos_cur from pos_start

    Particle p = ssbo_ps.particles[id];

    if (ssbo_ps.time_total_secs < p.life_time_secs_to_start)      //  life has not yet begun
    {
        return;
    }

        //  life time
    float life_time_secs_cur = p.life_time_secs_cur + ssbo_ps.time_prev_frame_secs;

        //  uv coord
    uint uvs_cur_id = p.uvs_start_id + uint(life_time_secs_cur / ssbo_ps.uv_shift_speed);
                                                            //  repeats amount              total amount   
    if (uvs_cur_id >= ssbo_uv.uvs_count) uvs_cur_id -= (uvs_cur_id / ssbo_uv.uvs_count) * ssbo_uv.uvs_count;   //  avoid out of range uvs[]

        //  particle spawn
    if (life_time_secs_cur > p.life_time_secs_total)  //  life ended, need restart life
    {
        life_time_secs_cur -= p.life_time_secs_total;
        CalcPosAndDirOnSpawn(id);
        ssbo_ps.particles[id].uvs_start_id = uvs_cur_id;      //  update start id
        p = ssbo_ps.particles[id];  //  get updated data from ssbo to p
    }

        //  calculate pos
    vec3 pos_cur = vec3(p.pos_cur[0], p.pos_cur[1], p.pos_cur[2]);
    vec3 move_dir_normalized = vec3(ssbo_ps.particles[id].move_dir_normalized[0], ssbo_ps.particles[id].move_dir_normalized[1], ssbo_ps.particles[id].move_dir_normalized[2]);
    pos_cur += move_dir_normalized * p.move_speed_secs * ssbo_ps.move_speed_power * ssbo_ps.time_prev_frame_secs;

        //  visibility
    float disappear_start_secs = p.life_time_secs_total - ssbo_ps.visibility_disappear_secs;
    float visibility_alpha = life_time_secs_cur < ssbo_ps.visibility_appear_secs ? life_time_secs_cur / ssbo_ps.visibility_appear_secs  //  particle appear (life start)
        : disappear_start_secs < life_time_secs_cur ? 1.f - ((life_time_secs_cur - disappear_start_secs) / ssbo_ps.visibility_disappear_secs)  //  particle dissapear (life end)
        : 1.f;  //  full seen

        //  set data
    ssbo_ps.particles[id].life_time_secs_cur = life_time_secs_cur;
    Set_pos_cur(id, pos_cur);
    ssbo_ps.particles[id].uvs_cur_id = uvs_cur_id;
    ssbo_ps.particles[id].visibility_alpha = visibility_alpha;
}

void CalcCornersRotatedToCam()
{
    vec3 cam_right = vec3(camera.view[0].x, camera.view[1].x, camera.view[2].x);     //  normalized
    vec3 cam_up = vec3(camera.view[0].y, camera.view[1].y, camera.view[2].y);        //  normalized

    float left_x    = -1.f;
    float right_x   =  1.f;
    float top_y     =  1.f;
    float bottom_y  = -1.f;

    vec3 size_bl = (cam_right * left_x * ssbo_ps.size_half_width) + (cam_up * bottom_y * ssbo_ps.size_half_height);
    ssbo_ps.size_bl[0] = size_bl.x;
    ssbo_ps.size_bl[1] = size_bl.y;
    ssbo_ps.size_bl[2] = size_bl.z;

    vec3 size_br = (cam_right * right_x * ssbo_ps.size_half_width) + (cam_up * bottom_y * ssbo_ps.size_half_height);
    ssbo_ps.size_br[0] = size_br.x;
    ssbo_ps.size_br[1] = size_br.y;
    ssbo_ps.size_br[2] = size_br.z;

    vec3 size_tl = (cam_right * left_x * ssbo_ps.size_half_width) + (cam_up * top_y * ssbo_ps.size_half_height);
    ssbo_ps.size_tl[0] = size_tl.x;
    ssbo_ps.size_tl[1] = size_tl.y;
    ssbo_ps.size_tl[2] = size_tl.z;

    vec3 size_tr = (cam_right * right_x * ssbo_ps.size_half_width) + (cam_up * top_y * ssbo_ps.size_half_height);
    ssbo_ps.size_tr[0] = size_tr.x;
    ssbo_ps.size_tr[1] = size_tr.y;
    ssbo_ps.size_tr[2] = size_tr.z;
}

void CalcPosAndDirOnSpawn(uint particle_id)
{
    Particle p = ssbo_ps.particles[particle_id];

        //  calc pos_cur
    vec4 pos_start = vec4(p.pos_start[0], p.pos_start[1], p.pos_start[2], 1.f);
    mat4 spawn_mat_model = mat4(
            ssbo_ps.spawn_mat_model[0][0], ssbo_ps.spawn_mat_model[0][1], ssbo_ps.spawn_mat_model[0][2], ssbo_ps.spawn_mat_model[0][3],
            ssbo_ps.spawn_mat_model[1][0], ssbo_ps.spawn_mat_model[1][1], ssbo_ps.spawn_mat_model[1][2], ssbo_ps.spawn_mat_model[1][3],
            ssbo_ps.spawn_mat_model[2][0], ssbo_ps.spawn_mat_model[2][1], ssbo_ps.spawn_mat_model[2][2], ssbo_ps.spawn_mat_model[2][3],
            ssbo_ps.spawn_mat_model[3][0], ssbo_ps.spawn_mat_model[3][1], ssbo_ps.spawn_mat_model[3][2], ssbo_ps.spawn_mat_model[3][3]
        );
    vec3 pos_cur = vec3(spawn_mat_model * pos_start);
    Set_pos_cur(particle_id, pos_cur);

        //  calc move_dir_normalized
    switch (ssbo_ps.move_direction_type)
    {
    case DT__from_particles_center: 
    {
        vec3 ps_origin_world_pos = vec3(spawn_mat_model * vec4(0.f, 0.f, 0.f, 1.f));
        Set_move_dir_normalized(particle_id, normalize(pos_cur - ps_origin_world_pos));
    } break;
    case DT__variable_is_direction: Set_move_dir_normalized(particle_id, vec3(ssbo_ps.move_variable[0], ssbo_ps.move_variable[1], ssbo_ps.move_variable[2])); break;     //  move_variable normalized at cpu in that case
    case DT__variable_is_destination:
    {
        vec4 dest_local = vec4(ssbo_ps.move_variable[0], ssbo_ps.move_variable[1], ssbo_ps.move_variable[2], 1.f);
        vec3 dest_world = vec3(spawn_mat_model * dest_local);
        Set_move_dir_normalized(particle_id, normalize(dest_world - pos_cur));
    } break;
    }
}

void Set_pos_cur(uint particle_id, vec3 pos_cur)
{
    ssbo_ps.particles[particle_id].pos_cur[0] = pos_cur.x;
    ssbo_ps.particles[particle_id].pos_cur[1] = pos_cur.y;
    ssbo_ps.particles[particle_id].pos_cur[2] = pos_cur.z;
}

void Set_move_dir_normalized(uint particle_id, vec3 move_dir_normalized)
{
    ssbo_ps.particles[particle_id].move_dir_normalized[0] = move_dir_normalized.x;
    ssbo_ps.particles[particle_id].move_dir_normalized[1] = move_dir_normalized.y;
    ssbo_ps.particles[particle_id].move_dir_normalized[2] = move_dir_normalized.z;
}