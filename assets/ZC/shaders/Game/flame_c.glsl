#version 460 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

    //  math
#define ZC_PI       3.14159265358979323846   // pi
#define ZC_PI_RAD_COEF (ZC_PI / 180.0)

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

    //  ssbo bindings
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

    //  move, see enum G_PS_Source::Move::DirectionType
#define PS_Move_DT__from_particles_center   0
#define PS_Move_DT__variable_is_direction   1
#define PS_Move_DT__variable_is_destination 2
    //  animation, see enum G_PS_Source::Animation::Repeat 
#define PS_Animation_R_Loop         0
#define PS_Animation_R_Single_pass  1

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
        //  coners world pos
    float world_bl[3];
    float world_br[3];
    float world_tl[3];
    float world_tr[3];
        //  rotate angle
    float rotate_angle;    //  rotate particle in 2d
        //  animaion
    float animation_start_secs;     //  when in life time to start animation
    uint animation_uvs_cur_id;      //  id of ssbo_uv.uvs[]
    float animation_uvs_cur_id_secs;      //  seconds to show animation_uvs_cur_id
};
layout (std430, binding = BIND_SSBO_PARTICLE) buffer SSBO_ParticleSystem
{
        //  time
    float time_prev_frame_secs;     //  cpu update
    float time_total_secs;          //  cpu update. Seconds from the start of particle system drawing
        //  spawn mat
    float spawn_mat_model[4][4];    //  cpu update. May be located in other SSBOs and one calculated system may be use in different places
        //  texture particle size for corners calculation
    float size_half_width;
    float size_half_height;
        //  visibility
    float visibility_appear_secs;
    float visibility_disappear_secs;
        //  move
    int move_direction_type;    //  see G_PS_Source::Move::DirectionType
    float move_variable[3];     //  see G_PS_Source::Move::DirectionType
    float move_speed_power;     //  total move speed of all particles
        //  animation
    int animation_repeat;       //  enum G_PS_Source::Animation::LifeTimePass: loop or one single pass for a life time
    float animation_uv_shift_speed;     //  (1 / uv_per_second)

    Particle particles[];
} ssbo_ps;


    //  functions
void CalcCornersRotatedToCam(uint particle_id, vec3 pos_cur);
void CalcPosAndDirOnSpawn(uint particle_id);
void Set_pos_cur(uint particle_id, vec3 pos_cur);
void Set_move_dir_normalized(uint particle_id, vec3 move_dir_normalized);
mat2 GetRotateMatrix2D(float angle);
void CalcAnimation(uint particle_id);

void main()
{
    uint id = gl_GlobalInvocationID.x;    //  use only glDispatchCompute(X, 1, 1). And use only layout(local_size_x, 1, 1)

    if (ssbo_ps.time_prev_frame_secs == ssbo_ps.time_total_secs) CalcPosAndDirOnSpawn(id);     //  start of particles system life, calculate particles pos_cur from pos_start

    Particle p = ssbo_ps.particles[id];

    if (ssbo_ps.time_total_secs < p.life_time_secs_to_start) return;     //  life has not yet begun

        //  life time
    float life_time_secs_cur = p.life_time_secs_cur + ssbo_ps.time_prev_frame_secs;

        //  particle spawn
    if (life_time_secs_cur > p.life_time_secs_total)  //  life ended, need restart life
    {
        life_time_secs_cur -= p.life_time_secs_total;
        CalcPosAndDirOnSpawn(id);
        if (ssbo_ps.animation_repeat == PS_Animation_R_Single_pass)   //  set default state for single pass particle
        {
            ssbo_ps.particles[id].animation_uvs_cur_id_secs = 0;
            ssbo_ps.particles[id].animation_uvs_cur_id = 0;
        }
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

        //  ssbo update
    ssbo_ps.particles[id].life_time_secs_cur = life_time_secs_cur;
    Set_pos_cur(id, pos_cur);
    ssbo_ps.particles[id].visibility_alpha = visibility_alpha;

    
    CalcCornersRotatedToCam(id, pos_cur);  //  must be called after ssbo update. Rotate particle in 2d, and calculate corners
    CalcAnimation(id);  //  must be called after ssbo update
}

void CalcCornersRotatedToCam(uint particle_id, vec3 pos_cur)
{
    vec3 cam_right = vec3(camera.view[0].x, camera.view[1].x, camera.view[2].x);     //  normalized
    vec3 cam_up = vec3(camera.view[0].y, camera.view[1].y, camera.view[2].y);        //  normalized

    vec2 bl = vec2(-1.f, -1.f);
    vec2 br = vec2(1.f, -1.f);
    vec2 tl = vec2(-1.f, 1.f);
    vec2 tr = vec2(1.f, 1.f);

    float rotate_angle_end = ssbo_ps.particles[particle_id].rotate_angle;
    if (rotate_angle_end != 0.f)
    {
        float rotate_angle_start = 0.f;
            //  rotate texture with life time
        float life_time_coef = ssbo_ps.particles[particle_id].life_time_secs_cur / ssbo_ps.particles[particle_id].life_time_secs_total;
        float rotate_angle_cur = rotate_angle_start + ((rotate_angle_end - rotate_angle_start) * life_time_coef);
        mat2 rotate_mat = GetRotateMatrix2D(rotate_angle_cur * ZC_PI_RAD_COEF);      //  to radians

        bl = rotate_mat * bl;
        br = rotate_mat * br;
        tl = rotate_mat * tl;
        tr = rotate_mat * tr;
    }

    vec3 rotated_to_cam_bl = (cam_right * bl.x * ssbo_ps.size_half_width) + (cam_up * bl.y * ssbo_ps.size_half_height);
    vec3 world_bl = rotated_to_cam_bl + pos_cur;
    ssbo_ps.particles[particle_id].world_bl[0] = world_bl.x;
    ssbo_ps.particles[particle_id].world_bl[1] = world_bl.y;
    ssbo_ps.particles[particle_id].world_bl[2] = world_bl.z;

    vec3 rotated_to_cam_br = (cam_right * br.x * ssbo_ps.size_half_width) + (cam_up * br.y * ssbo_ps.size_half_height);
    vec3 world_br = rotated_to_cam_br + pos_cur;
    ssbo_ps.particles[particle_id].world_br[0] = world_br.x;
    ssbo_ps.particles[particle_id].world_br[1] = world_br.y;
    ssbo_ps.particles[particle_id].world_br[2] = world_br.z;

    vec3 rotated_to_cam_tl = (cam_right * tl.x * ssbo_ps.size_half_width) + (cam_up * tl.y * ssbo_ps.size_half_height);
    vec3 world_tl = rotated_to_cam_tl + pos_cur;
    ssbo_ps.particles[particle_id].world_tl[0] = world_tl.x;
    ssbo_ps.particles[particle_id].world_tl[1] = world_tl.y;
    ssbo_ps.particles[particle_id].world_tl[2] = world_tl.z;

    vec3 rotated_to_cam_tr = (cam_right * tr.x * ssbo_ps.size_half_width) + (cam_up * tr.y * ssbo_ps.size_half_height);
    vec3 world_tr = rotated_to_cam_tr + pos_cur;
    ssbo_ps.particles[particle_id].world_tr[0] = world_tr.x;
    ssbo_ps.particles[particle_id].world_tr[1] = world_tr.y;
    ssbo_ps.particles[particle_id].world_tr[2] = world_tr.z;
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
    case PS_Move_DT__from_particles_center: 
    {
        vec3 ps_origin_world_pos = vec3(spawn_mat_model * vec4(0.f, 0.f, 0.f, 1.f));
        Set_move_dir_normalized(particle_id, normalize(pos_cur - ps_origin_world_pos));
    } break;
    case PS_Move_DT__variable_is_direction: Set_move_dir_normalized(particle_id, vec3(ssbo_ps.move_variable[0], ssbo_ps.move_variable[1], ssbo_ps.move_variable[2])); break;     //  move_variable normalized at cpu in that case
    case PS_Move_DT__variable_is_destination:
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

mat2 GetRotateMatrix2D(float angle)
{
    return mat2(
        cos(angle), -sin(angle),
        sin(angle), cos(angle)
    );
}

void CalcAnimation(uint particle_id)
{
    if (ssbo_ps.particles[particle_id].life_time_secs_cur < ssbo_ps.particles[particle_id].animation_start_secs   //  animation don't started
        || (ssbo_ps.animation_repeat == PS_Animation_R_Single_pass && ssbo_ps.particles[particle_id].animation_uvs_cur_id == ssbo_uv.uvs_count - 1)) return;    //  PS_Animation_R_Single_pass reached end, wait for respawn

    float animation_uvs_cur_id_secs = ssbo_ps.particles[particle_id].animation_uvs_cur_id_secs + ssbo_ps.time_prev_frame_secs;
    if (animation_uvs_cur_id_secs > ssbo_ps.animation_uv_shift_speed)   //  switch to next uv
    {
        uint animation_uvs_cur_id = ssbo_ps.particles[particle_id].animation_uvs_cur_id + 1;
        if (animation_uvs_cur_id == ssbo_uv.uvs_count)
            animation_uvs_cur_id = ssbo_ps.animation_repeat == PS_Animation_R_Loop ? 0 : animation_uvs_cur_id - 1;     //  overflow uvs array, so restart loop. But for PS_Animation_R_Single_pass restart only on respawn

        animation_uvs_cur_id_secs -= ssbo_ps.animation_uv_shift_speed;  //  restart time

            //  update ssbo
        ssbo_ps.particles[particle_id].animation_uvs_cur_id = animation_uvs_cur_id;
    }
            //  update ssbo
    ssbo_ps.particles[particle_id].animation_uvs_cur_id_secs = animation_uvs_cur_id_secs;
}