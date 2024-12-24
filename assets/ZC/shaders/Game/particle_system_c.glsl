#version 460 core   //  flame.vs

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

    //  math
#define ZC_PI 3.14159265358979323846   // pi
#define ZC_PI_RAD_COEF (ZC_PI / 180.0)

    //  SSBO_ParticleSystem
    //  move, see enum G_PS_Source::Move::DirectionType
#define PS_Move_DT_from_particles_center   0
#define PS_Move_DT_variable_is_direction   1
#define PS_Move_DT_variable_is_destination 2
    //  animation, see enum G_PS_Source::Animation::Repeat 
#define PS_Animation_R_Loop        0
#define PS_Animation_R_Single_pass 1
    //  external influence, see enum G_PS_Source::ExternalInfluence
#define PS_EI_None      0
#define PS_EI_Collision 1

    //  SSBO_Collision
    //  see enum G_ParticleSystem::Collision::SetLifeTime
#define PS_Collision_SLF_None            0
#define PS_Collision_SLT_Start_animation 1

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
#define G_BIND_SSBO_PARTICLE 0
#define G_BIND_SSBO_TEX_DATA 1
#define G_Bind_SSBO_COLLISION 2

    //  collision
struct CollisionObject
{
    float radius;   //  if radius = 0 it is free space
    float[3] world_pos;
};
layout (std430, binding = G_Bind_SSBO_COLLISION) readonly buffer SSBO_Collision
{
    uint collision_objects_count;    //  filles with system, may be missed. Count of object in collision_objects.
    float particle_radius;    //  radius ofthe particle (may be less then drawing particle size, help to avoid alpha channels border around the particle).

    int set_life_time;     //  enum G_ParticleSystem::Collision::SetLifeTime
    float move_speed;      //  percent value to change speed value. Range [-1.f, 1.f].
    
    CollisionObject collision_objects[];
} ssbo_collision;

    //  texture data
struct UV
{
    float left_x;
    float top_y;
    float right_x;
    float bottom_y;
};
layout (std430, binding = G_BIND_SSBO_TEX_DATA) readonly buffer SSBO_UV
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
        //  move
    float move_dir_normalized[3];
    float move_speed_secs_start;  //  sets to move_speed_secs_cur at respawn
    float move_speed_secs_cur;    //  cur speed
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
        //  color
    uint color_rgba;     //  rgb to add and alpha, packed [32]->8x8x8x8
};
layout (std430, binding = G_BIND_SSBO_PARTICLE) buffer SSBO_ParticleSystem
{
        //  time
    float time_prev_frame_secs;     //  cpu update
    float time_total_secs;          //  cpu update. Seconds from the start of particle system drawing
        //  spawn mat
    float spawn_mat_model[4][4];    //  cpu update. May be located in other SSBOs and one calculated system may be use in different places
        //  texture particle size for corners calculation
    float size_half_width;
    float size_half_height;
        //  move
    int move_direction_type;    //  see G_PS_Source::Move::DirectionType
    float move_variable[3];     //  see G_PS_Source::Move::DirectionType
    float move_speed_power;     //  total move speed of all particles
        //  animation
    int animation_repeat;       //  enum G_PS_Source::Animation::LifeTimePass: loop or one single pass for a life time
    float animation_uv_shift_speed;     //  (1 / uv_per_second)
        //  color
    float system_alpha;   //  visibility of all system
    int color_rgb_use;      //  see enum G_PS_Source::Color::RGBUse
    float color_appear_secs;
    float color_disappear_secs;
    uint color_rgba_start;       //  rgb interpolation start and alpha appear, packed [32]->8x8x8x8
    uint color_rgba_end;         //  rgb interpolation end and alpha disappear, packed [32]->8x8x8x8
        //  external influence
    int external_influence_mask;    //  G_PS_Source::ExternalInfluence

    Particle particles[];
} ssbo_ps;


    //  functions
void Respawn(uint particle_id);
void CalcCornersRotatedToCam(uint particle_id, vec3 pos_cur);
void CalcPosAndDirOnSpawn(uint particle_id);
void Set_pos_cur(uint particle_id, vec3 pos_cur);
void Set_move_dir_normalized(uint particle_id, vec3 move_dir_normalized);
mat2 GetRotateMatrix2D(float angle);
uint GetNextUV(uint particle_id);
void CalcAnimation(uint particle_id);
vec4 Unpack_UInt_8x8x8x8_To_vec4(uint rgba);
uint Pack_vec4_To_UInt_8x8x8x8(vec4 val);
vec3 InterpolateColor(vec3 start_color, vec3 end_color, float growing_coef);
void CalaColor(uint particle_id);
void MakeCollision(uint particle_id);

void main()
{
    uint id = gl_GlobalInvocationID.x;    //  use only glDispatchCompute(X, 1, 1). And use only layout(local_size_x, 1, 1)

    if (ssbo_ps.time_prev_frame_secs == ssbo_ps.time_total_secs)    //  start drwing system, set particle to default state
    {
        ssbo_ps.particles[id].life_time_secs_cur = 0.f;
        ssbo_ps.particles[id].color_rgba = 0;   //  if system were drawing before, particle must be invisible wile -> ssbo_ps.time_total_secs < p.life_time_secs_to_start
        Respawn(id);     //  start of particles system life, calculate particles pos_cur from pos_start
    }

    Particle p = ssbo_ps.particles[id];

    if (ssbo_ps.time_total_secs < p.life_time_secs_to_start) return;     //  life has not yet begun

        //  life time
    float life_time_secs_cur = p.life_time_secs_cur + ssbo_ps.time_prev_frame_secs;

        //  particle spawn
    if (life_time_secs_cur > p.life_time_secs_total)  //  life ended, need restart life
    {
        life_time_secs_cur -= p.life_time_secs_total;
        Respawn(id);
        p = ssbo_ps.particles[id];  //  get updated data from ssbo to p
    }

        //  move
    vec3 pos_cur = vec3(p.pos_cur[0], p.pos_cur[1], p.pos_cur[2]);
    vec3 move_dir_normalized = vec3(ssbo_ps.particles[id].move_dir_normalized[0], ssbo_ps.particles[id].move_dir_normalized[1], ssbo_ps.particles[id].move_dir_normalized[2]);
    pos_cur += move_dir_normalized * p.move_speed_secs_cur * ssbo_ps.move_speed_power * ssbo_ps.time_prev_frame_secs;

        //  ssbo update
    ssbo_ps.particles[id].life_time_secs_cur = life_time_secs_cur;
    Set_pos_cur(id, pos_cur);

        //  next functions must be called after ssbo update
    CalcCornersRotatedToCam(id, pos_cur);   //  Rotate particle in 2d, and calculate corners
    CalcAnimation(id);
    CalaColor(id);
    MakeCollision(id);
}

void Respawn(uint particle_id)
{
        //  pos and dir
    CalcPosAndDirOnSpawn(particle_id);
        //  move speed
    ssbo_ps.particles[particle_id].move_speed_secs_cur = ssbo_ps.particles[particle_id].move_speed_secs_start;
        //  animation
    if (ssbo_ps.animation_repeat == PS_Animation_R_Single_pass)   //  set default state for single pass particle
    {
        ssbo_ps.particles[particle_id].animation_uvs_cur_id_secs = 0;
        ssbo_ps.particles[particle_id].animation_uvs_cur_id = 0;
    }
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
    case PS_Move_DT_from_particles_center: 
    {
        vec3 ps_origin_world_pos = vec3(spawn_mat_model * vec4(0.f, 0.f, 0.f, 1.f));
        Set_move_dir_normalized(particle_id, normalize(pos_cur - ps_origin_world_pos));
    } break;
    case PS_Move_DT_variable_is_direction: Set_move_dir_normalized(particle_id, vec3(ssbo_ps.move_variable[0], ssbo_ps.move_variable[1], ssbo_ps.move_variable[2])); break;     //  move_variable normalized at cpu in that case
    case PS_Move_DT_variable_is_destination:
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

uint GetNextUV(uint particle_id)
{
    uint animation_uvs_cur_id = ssbo_ps.particles[particle_id].animation_uvs_cur_id + 1;
    if (animation_uvs_cur_id == ssbo_uv.uvs_count)
        animation_uvs_cur_id = ssbo_ps.animation_repeat == PS_Animation_R_Loop ? 0 : animation_uvs_cur_id - 1;     //  overflow uvs array, so restart loop. But for PS_Animation_R_Single_pass restart only on respawn
    return animation_uvs_cur_id;
}

void CalcAnimation(uint particle_id)
{
    if (ssbo_ps.particles[particle_id].life_time_secs_cur < ssbo_ps.particles[particle_id].animation_start_secs   //  animation don't started
        || (ssbo_ps.animation_repeat == PS_Animation_R_Single_pass && ssbo_ps.particles[particle_id].animation_uvs_cur_id == ssbo_uv.uvs_count - 1)) return;    //  PS_Animation_R_Single_pass reached end, wait for respawn

    if (ssbo_ps.particles[particle_id].animation_start_secs != 0.f && ssbo_ps.particles[particle_id].animation_uvs_cur_id == 0)    //  if the animation has a time offset, then when the animation starts there is no need to show the first tile of the animation, we immediately move on to the next
        ssbo_ps.particles[particle_id].animation_uvs_cur_id = GetNextUV(particle_id);

    float animation_uvs_cur_id_secs = ssbo_ps.particles[particle_id].animation_uvs_cur_id_secs + ssbo_ps.time_prev_frame_secs;
    if (animation_uvs_cur_id_secs > ssbo_ps.animation_uv_shift_speed)   //  switch to next uv
    {
        uint animation_uvs_cur_id = GetNextUV(particle_id);

        animation_uvs_cur_id_secs -= ssbo_ps.animation_uv_shift_speed;  //  restart time

            //  update ssbo
        ssbo_ps.particles[particle_id].animation_uvs_cur_id = animation_uvs_cur_id;
    }
            //  update ssbo
    ssbo_ps.particles[particle_id].animation_uvs_cur_id_secs = animation_uvs_cur_id_secs;
}

vec4 Unpack_UInt_8x8x8x8_To_vec4(uint rgba)
{
    return vec4(((rgba >> 24) & 255u) / 255.f, ((rgba >> 16) & 255u) / 255.f, ((rgba >> 8) & 255u) / 255.f, (rgba & 255u) / 255.f);
}

uint Pack_vec4_To_UInt_8x8x8x8(vec3 rgb, float alpha)
{
    return ((uint(rgb.r * 255.f) << 8 | uint(rgb.g * 255.f)) << 8 | uint(rgb.b * 255.f)) << 8 | uint(alpha * 255.f);
}

vec3 InterpolateColor(vec3 start_color, vec3 end_color, float growing_coef)
{
    return vec3(
        start_color.r > end_color.r ? start_color.r - ((start_color.r - end_color.r) * growing_coef) : start_color.r + ((end_color.r - start_color.r) * growing_coef),
        start_color.g > end_color.g ? start_color.g - ((start_color.g - end_color.g) * growing_coef) : start_color.g + ((end_color.g - start_color.g) * growing_coef),
        start_color.b > end_color.b ? start_color.b - ((start_color.b - end_color.b) * growing_coef) : start_color.b + ((end_color.b - start_color.b) * growing_coef)
        );
}

void CalaColor(uint particle_id)
{
    vec4 rgba_start = Unpack_UInt_8x8x8x8_To_vec4(ssbo_ps.color_rgba_start);
    vec4 rgba_end = Unpack_UInt_8x8x8x8_To_vec4(ssbo_ps.color_rgba_end);
    float life_time_secs_cur = ssbo_ps.particles[particle_id].life_time_secs_cur;
    float life_time_secs_total = ssbo_ps.particles[particle_id].life_time_secs_total;

        //  rgb
    vec3 rgb_start = vec3(rgba_start);
    vec3 rgb_end = vec3(rgba_end);
    vec3 rgb = rgb_start == rgb_end ? vec3(0.f, 0.f, 0.f) : InterpolateColor(rgb_start, rgb_end, life_time_secs_cur / life_time_secs_total);  //  interpolate color over life time

        //  alpha
    const float max_alpha = 1.f;
    float alpha = max_alpha;    //  default full seen alpha 1.f
    float disappear_start_secs = life_time_secs_total - ssbo_ps.color_disappear_secs;
    float visibility_alpha;
    if (life_time_secs_cur < ssbo_ps.color_appear_secs)
    {
        float start_alpha = rgba_start.a;
        float start_life_time_coef = life_time_secs_cur / ssbo_ps.color_appear_secs;  //  particle appear (life start)
        float start_life_alpha_range = max_alpha - start_alpha;
        alpha = start_alpha + (start_life_alpha_range * start_life_time_coef);
    }
    else if (disappear_start_secs < life_time_secs_cur)
    {
        float end_alpha = rgba_end.a;
        float end_life_time_coef = (life_time_secs_cur - disappear_start_secs) / ssbo_ps.color_disappear_secs;
        float end_life_alpha_range = max_alpha - end_alpha;
        alpha = max_alpha - (end_life_alpha_range * end_life_time_coef);
    }
        //  update ssbo
    ssbo_ps.particles[particle_id].color_rgba = Pack_vec4_To_UInt_8x8x8x8(rgb, alpha);
}

void MakeCollision(uint particle_id)
{
    if ((ssbo_ps.external_influence_mask & PS_EI_Collision) != PS_EI_Collision) return;   //  these's no collision objects

    vec3 p_pos_world = vec3(ssbo_ps.particles[particle_id].pos_cur[0], ssbo_ps.particles[particle_id].pos_cur[1], ssbo_ps.particles[particle_id].pos_cur[2]);

    for (uint i = 0; i < ssbo_collision.collision_objects_count; ++i)
    {
        float obj_radius = ssbo_collision.collision_objects[i].radius;
        if (obj_radius == 0) continue;    //  empty space

        float max_dist = ssbo_collision.particle_radius + obj_radius;
        vec3 obj_pos_world = vec3(ssbo_collision.collision_objects[i].world_pos[0], ssbo_collision.collision_objects[i].world_pos[1], ssbo_collision.collision_objects[i].world_pos[2]);
        float centers_dist = length(p_pos_world - obj_pos_world);
        if (centers_dist <= max_dist)   //  had collision
        {
            if ((ssbo_collision.set_life_time & PS_Collision_SLT_Start_animation) == PS_Collision_SLT_Start_animation && ssbo_ps.particles[particle_id].life_time_secs_cur < ssbo_ps.particles[particle_id].animation_start_secs)     //  starts the animation time if it has not already started
                ssbo_ps.particles[particle_id].life_time_secs_cur = ssbo_ps.particles[particle_id].animation_start_secs;     //  collision may happend many times at the life time, life time changes sets only at first collision
            if (ssbo_collision.move_speed != 0.f && ssbo_ps.particles[particle_id].move_speed_secs_cur == ssbo_ps.particles[particle_id].move_speed_secs_start)      //  change speed if it wasn't shaged yet
                ssbo_ps.particles[particle_id].move_speed_secs_cur += ssbo_ps.particles[particle_id].move_speed_secs_cur * ssbo_collision.move_speed;      //  stop move

            return;  //  one collision with one object at a time is enough. DON'T USE break; !!!
        }
    }
}