// #version 460 core

//     //  in
// layout (points) in;
// layout (triangle_strip, max_vertices = 4) out;

// layout (std140, binding = 0) uniform Camera
// {
//     mat4 ortho;
//     mat4 perspective;
//     mat4 perspView;
//     mat4 perspViewSkybox;
//     mat4 view;
//     vec3 camPos;
// };

// layout (location = 0) in InG    //  ALL VARIABLES MUST BE INVOLVED INTO THE SOME FUCNTION OR GONNA BE PROBLEM WITH ALIGNMENT (I THOUGHT THAT WITH ALIGNMENT. DATA AFTER NOT INVOLVED FIELD IS NOT CORRECT)
// {
//     float tex_left_x;
//     float tex_top_y;
//     float tex_right_x;
//     float tex_bottom_y;
// } inG[];

//     /*
//     data[0].x - cur time in seconds
//     data[0].y - tiles per second
//     data[0].z - particle widht
//     data[0].w - particle height

//     data[1].x - appear/disappear seconds. other time particle have alpha 1.f
//     data[1].y - pos x
//     data[1].z - pos y
//     data[1].y - pos z

//     data[2].x - move to x
//     data[2].y - move to y
//     data[2].z - move to z
//     data[3].w - move speed min secs

//     data[3].x - move speed max secs
//     */
// layout (location = 0) uniform mat4 unData;


//     //  out
// layout (location = 0) out OutG
// {
//     float life_time_alpha;
//     vec2 tex_coords;
// } outG;


//     //  functions
// void SetVertexData(vec3 quad_corner, vec2 tex_coords)
// {
//     outG.tex_coords = tex_coords;
//     vec3 particles_origin_pos = unData[1].yzw;
//     gl_Position = perspView * vec4(particles_origin_pos + gl_in[0].gl_Position.xyz + quad_corner, 1.f);
//     EmitVertex();
// }

//     //  drawing from points to quads need to take ID = 0, NOT gl_PrimitiveIDIn! For gl_in[0] and in block inG[0]
// void main()
// {
//     outG.life_time_alpha = gl_in[0].gl_Position.w;   //  in gs put alpha to .w

//         //  calculate particle's corners positions
//     float particle_half_width = unData[0].z / 2.f;
//     float particle_half_height = unData[0].w / 2.f;

//     vec3 cam_right = vec3(view[0].x, view[1].x, view[2].x);     //  normalized
//     vec3 cam_up = vec3(view[0].y, view[1].y, view[2].y);        //  normalized

//     const vec2 corner_bl = vec2(-1.f, -1.f);
//     const vec2 corner_br = vec2( 1.f, -1.f);
//     const vec2 corner_tl = vec2(-1.f,  1.f);
//     const vec2 corner_tr = vec2( 1.f,  1.f);
    
//     vec3 bl = (cam_right * corner_bl.x * particle_half_width) + (cam_up * corner_bl.y * particle_half_height);    //  rotate corner in origin face to cam
//     SetVertexData(bl, vec2(inG[0].tex_left_x, inG[0].tex_bottom_y));

//     vec3 br = (cam_right * corner_br.x * particle_half_width) + (cam_up * corner_br.y * particle_half_height);    //  rotate corner in origin face to cam
//     SetVertexData(br, vec2(inG[0].tex_right_x, inG[0].tex_bottom_y));

//     vec3 tl = (cam_right * corner_tl.x * particle_half_width) + (cam_up * corner_tl.y * particle_half_height);    //  rotate corner in origin face to cam
//     SetVertexData(tl, vec2(inG[0].tex_left_x, inG[0].tex_top_y));

//     vec3 tr = (cam_right * corner_tr.x * particle_half_width) + (cam_up * corner_tr.y * particle_half_height);    //  rotate corner in origin face to cam
//     SetVertexData(tr, vec2(inG[0].tex_right_x, inG[0].tex_top_y));

//     EndPrimitive();
// }






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

layout (location = 0) in InG    //  ALL VARIABLES MUST BE INVOLVED INTO THE SOME FUCNTION OR GONNA BE PROBLEM WITH ALIGNMENT (I THOUGHT THAT WITH ALIGNMENT. DATA AFTER NOT INVOLVED FIELD IS NOT CORRECT)
{
    float life_time_alpha;
    flat uint uvs_cur_id;
} inG[];


    //  out
layout (location = 0) out OutG
{
    float life_time_alpha;
    vec2 uv;
    float add_color;
} outG;


    //  functions
void SetVertexData(vec4 particle_corner_pos_world, vec2 uv)
{
    outG.uv = uv;
                                        //  spread position to corners rotated to cam 
    gl_Position = camera.perspView * particle_corner_pos_world;
    // gl_Position = camera.perspView * mat_model * vec4(particle_pos_cur + quad_corner, 1.f);
    
    // vec3 particles_origin_pos = vec3(ssbo_ps.particles_origin_pos[0], ssbo_ps.particles_origin_pos[1], ssbo_ps.particles_origin_pos[2]);
    // gl_Position = camera.perspView * vec4(particles_origin_pos + particle_pos_cur + quad_corner, 1.f);

    // gl_Position = camera.perspView * vec4(particles_origin_pos + inG[0].pos_cur + quad_corner, 1.f);
    EmitVertex();
}

void main()
{
    // outG.p = inParticle.particles[gl_PrimitiveIDIn];

    Particle particle = ssbo_ps.particles[gl_PrimitiveIDIn];

    outG.life_time_alpha = inG[0].life_time_alpha;
    outG.add_color = 1.f - (particle.life_secs_cur / particle.life_secs_total);
    
    if (outG.life_time_alpha == 0.f)    //  particle isn't visible, will be discard in the fragment shader
    {
        EmitVertex();
        EndPrimitive();
        return;
    }
        //  particle pos
    vec4 particle_pos_world = vec4(particle.pos_cur[0], particle.pos_cur[1], particle.pos_cur[2], 1.f);
    if (ssbo_ps.life_space == G_PLS__particles_space)     //  if G_PLS__world_space, particle allready at the world space look enum G_Particles::G_ParticleMoveSpace
    {
        mat4 mat_model = mat4(
                ssbo_ps.mat_model[0][0], ssbo_ps.mat_model[0][1], ssbo_ps.mat_model[0][2], ssbo_ps.mat_model[0][3],
                ssbo_ps.mat_model[1][0], ssbo_ps.mat_model[1][1], ssbo_ps.mat_model[1][2], ssbo_ps.mat_model[1][3],
                ssbo_ps.mat_model[2][0], ssbo_ps.mat_model[2][1], ssbo_ps.mat_model[2][2], ssbo_ps.mat_model[2][3],
                ssbo_ps.mat_model[3][0], ssbo_ps.mat_model[3][1], ssbo_ps.mat_model[3][2], ssbo_ps.mat_model[3][3]
            );
        particle_pos_world = mat_model * particle_pos_world;
    }

    UV uv = ssbo_uv.uvs[inG[0].uvs_cur_id];
    SetVertexData(particle_pos_world + vec4(ssbo_ps.bl[0], ssbo_ps.bl[1], ssbo_ps.bl[2], 0.f), vec2(uv.left_x, uv.bottom_y));
    SetVertexData(particle_pos_world + vec4(ssbo_ps.br[0], ssbo_ps.br[1], ssbo_ps.br[2], 0.f), vec2(uv.right_x, uv.bottom_y));
    SetVertexData(particle_pos_world + vec4(ssbo_ps.tl[0], ssbo_ps.tl[1], ssbo_ps.tl[2], 0.f), vec2(uv.left_x, uv.top_y));
    SetVertexData(particle_pos_world + vec4(ssbo_ps.tr[0], ssbo_ps.tr[1], ssbo_ps.tr[2], 0.f), vec2(uv.right_x, uv.top_y));


        //  calculates in the vertex shader
    //     //  calculate particle's corners positions
    // float particle_half_width = ssbo_ps.half_width;
    // float particle_half_height = ssbo_ps.half_height;

    // vec3 cam_right = vec3(camera.view[0].x, camera.view[1].x, camera.view[2].x);     //  normalized
    // vec3 cam_up = vec3(camera.view[0].y, camera.view[1].y, camera.view[2].y);        //  normalized

    // float left_x    = -1.f;
    // float right_x   =  1.f;
    // float top_y     =  1.f;
    // float bottom_y  = -1.f;
    
    // UV uv = ssbo_uv.uvs[inG[0].uvs_cur_id];

    // vec3 bl = (cam_right * left_x * ssbo_ps.half_width) + (cam_up * bottom_y * ssbo_ps.half_height);
    // SetVertexData(bl, vec2(uv.left_x, uv.bottom_y));

    // vec3 br = (cam_right * right_x * ssbo_ps.half_width) + (cam_up * bottom_y * ssbo_ps.half_height);
    // SetVertexData(br, vec2(uv.right_x, uv.bottom_y));

    // vec3 tl = (cam_right * left_x * ssbo_ps.half_width) + (cam_up * top_y * ssbo_ps.half_height);
    // SetVertexData(tl, vec2(uv.left_x, uv.top_y));

    // vec3 tr = (cam_right * right_x * ssbo_ps.half_width) + (cam_up * top_y * ssbo_ps.half_height);
    // SetVertexData(tr, vec2(uv.right_x, uv.top_y));

    EndPrimitive();
}