#version 460 core

    //  in
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

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
    int color_rgb_use;      //  see enum G_PS_Source::Color::RGBUse
    float color_appear_secs;
    float color_disappear_secs;
    uint color_rgba_start;       //  rgb interpolation start and alpha appear, packed [32]->8x8x8x8
    uint color_rgba_end;         //  rgb interpolation end and alpha disappear, packed [32]->8x8x8x8
        //  collision
    int collision_action_mask;

    Particle particles[];
} ssbo_ps;


    //  out
layout (location = 0) out OutG
{
    int color_rgb_use;
    vec3 color_rgb;
    float alpha;
    vec2 uv;
} outG;

    //  functions
void SetVertexData(vec4 particle_corner_pos_world, vec2 uv);
vec4 Unpack_UInt_8x8x8x8_To_vec4(uint rgba);

void main()
{
    Particle particle = ssbo_ps.particles[gl_PrimitiveIDIn];

    outG.color_rgb_use = ssbo_ps.color_rgb_use;
    vec4 color_rgba = Unpack_UInt_8x8x8x8_To_vec4(particle.color_rgba);
    outG.color_rgb = color_rgba.rgb;
    outG.alpha = color_rgba.a;
    
    if (outG.alpha == 0.f)    //  particle isn't visible, will be discard in the fragment shader
    {
        EmitVertex();
        EndPrimitive();
        return;
    }
        //  particle pos
    UV uv = ssbo_uv.uvs[particle.animation_uvs_cur_id];
    SetVertexData(vec4(particle.world_bl[0], particle.world_bl[1], particle.world_bl[2], 1.f), vec2(uv.left_x, uv.bottom_y));
    SetVertexData(vec4(particle.world_br[0], particle.world_br[1], particle.world_br[2], 1.f), vec2(uv.right_x, uv.bottom_y));
    SetVertexData(vec4(particle.world_tl[0], particle.world_tl[1], particle.world_tl[2], 1.f), vec2(uv.left_x, uv.top_y));
    SetVertexData(vec4(particle.world_tr[0], particle.world_tr[1], particle.world_tr[2], 1.f), vec2(uv.right_x, uv.top_y));

    EndPrimitive();
}

void SetVertexData(vec4 particle_corner_pos_world, vec2 uv)
{
    outG.uv = uv;
    gl_Position = camera.perspView * particle_corner_pos_world;
    EmitVertex();
}

vec4 Unpack_UInt_8x8x8x8_To_vec4(uint rgba)
{
    return vec4(((rgba >> 24) & 255u) / 255.f, ((rgba >> 16) & 255u) / 255.f, ((rgba >> 8) & 255u) / 255.f, (rgba & 255u) / 255.f);
}