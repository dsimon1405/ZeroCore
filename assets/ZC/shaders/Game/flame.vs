// #version 460 core   //  flame.gs
//     //  in
// layout (location = 0) in vec4 inPosition;   //  inPositin.w is life time

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
//     data[2].w - move speed
//     */
// layout (location = 0) uniform mat4 unData;


//     //  out
// layout (location = 0) out OutV
// {
//     float tex_left_x;
//     float tex_top_y;
//     float tex_right_x;
//     float tex_bottom_y;
// } outV;


//     //  function
// vec3 MoveByLength(vec3 v, vec3 direction, float length);

// void main()
// {
//     const float tex_tile_width = 128.f;
//     const float tex_tile_height = 128.f;
//     const int tex_columns_count = 4;
//     const int tex_rows_count = 4;
//     const float tex_widht = tex_tile_width * tex_columns_count;
//     const float tex_height = tex_tile_height * tex_rows_count;
//     const int atlas_size = tex_columns_count * tex_rows_count;
    
//     const float cur_time_seconds = unData[0].x;
//     const float tiles_per_second = 1.f / unData[0].y;
//     float tiles_passed = floor(cur_time_seconds / tiles_per_second);   //  amoun of the tiles passed from the start of the time
//     float atlases_passed = floor(tiles_passed / atlas_size);    //  amount of atlases passed from the start of the time
//     int cur_tile_index = int(tiles_passed - (atlas_size * atlases_passed));
    
//     int cur_tile_tex_row_index = cur_tile_index / tex_columns_count;
//     int cur_tile_tex_column_index = cur_tile_index - (tex_columns_count * cur_tile_tex_row_index);
//         //  read atlas from top left to bottom right
//     outV.tex_left_x = cur_tile_tex_column_index * tex_tile_width;
//     outV.tex_top_y = tex_height - (cur_tile_tex_row_index * tex_tile_height);   //  opengl start count texture coords from bottom left, we are moves from top left
//     outV.tex_right_x = outV.tex_left_x + tex_tile_width;
//     outV.tex_bottom_y = outV.tex_top_y - tex_tile_height;
//         //  normalize tex coords
//     outV.tex_left_x /= tex_widht;
//     outV.tex_top_y /= tex_height;
//     outV.tex_right_x /= tex_widht;
//     outV.tex_bottom_y /= tex_height;

//         //  cur life time
//     const float particle_life_time = inPosition.w;
//     float lives_passed = floor(cur_time_seconds / particle_life_time);
//     float cur_life_time_seconds = cur_time_seconds - (particle_life_time * lives_passed);

//         //  particle alpha
//     const float appear_disappear_seconds = unData[1].x;
//     const float disappear_start_seconds = particle_life_time - appear_disappear_seconds;
//     float life_time_alpha = 1.f;
//     if (cur_life_time_seconds < appear_disappear_seconds) life_time_alpha = cur_life_time_seconds / appear_disappear_seconds;  //  particle appear (life start)
//     else if (disappear_start_seconds < cur_life_time_seconds) life_time_alpha = 1.f - (cur_life_time_seconds - disappear_start_seconds) / appear_disappear_seconds;  //  particle dissapear (life end)
    
//         //  move pos
//     const vec3 start_pos = inPosition.xyz;
//     const vec3 move_to = vec3(unData[2].x, unData[2].y, unData[2].z);
//     vec3 dir_pos_to_move_to = move_to - start_pos;
//     const float move_speed_seconds = unData[2].w;
//     float dist_pos_to_move_to = move_speed_seconds * cur_life_time_seconds;
//     vec3 pos = MoveByLength(start_pos, dir_pos_to_move_to, dist_pos_to_move_to);

//     gl_Position = vec4(pos, life_time_alpha);


//     // gl_Position = vec4(inPosition.xyz, life_time_alpha);
// }

// vec3 MoveByLength(vec3 v, vec3 direction, float length)
// {
//     return v + (direction * (length / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z)));
// }




    //      WITH SSBO
#version 460 core   //  flame.gs
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


    //  out
layout (location = 0) out OutV
{
    float life_time_alpha;
    flat uint uvs_cur_id;
} outV;


void main()
{
    gl_Position = vec4(0,0,0,1);

    outV.life_time_alpha = ssbo_ps.particles[gl_VertexID].life_time_alpha;
    outV.uvs_cur_id = ssbo_ps.particles[gl_VertexID].uvs_cur_id;

    if (gl_VertexID == 0) ssbo_ps.prev_frame_secs = 0.f;     //  updater can be stoped on cpu for particles, so need to stop update on gpu like that





    // gl_Position = vec4(0,0,0,1);

    // Particle p = ssbo_particles.particles[gl_VertexID];

    // if (ssbo_particles.total_secs < p.secs_to_start)      //  life has not yet begun
    // {
    //     outV.life_time_alpha = 0.f;
    //     return;
    // }
    //     //  get pos
    // vec3 pos_cur = vec3(p.pos_cur[0], p.pos_cur[1], p.pos_cur[2]);

    //     //  life time
    // float life_secs_cur = p.life_secs_cur + ssbo_particles.prev_frame_secs;

    //     //  uv coord
    // uint uvs_cur_id = p.uvs_start_id + uint(life_secs_cur / ssbo_particles.uv_shift_speed);
    //                                                 //  repeats amount           total amount   
    // if (uvs_cur_id >= ssbo_uv.uvs_count) uvs_cur_id -= (uvs_cur_id / ssbo_uv.uvs_count) * ssbo_uv.uvs_count;   //  avoid out of range uvs[]

    //     //  particle reborn
    // if (life_secs_cur > p.life_secs_total)  //  life ended
    // {
    //     life_secs_cur -= p.life_secs_total;
    //     pos_cur = vec3(p.pos_start[0], p.pos_start[1], p.pos_start[2]);    //  return particle to the start

    //     ssbo_particles.particles[gl_VertexID].uvs_start_id = uvs_cur_id;      //  update start id
    // }

    //     //  calculate pos
    // vec3 dir_move_normalized = vec3(ssbo_particles.particles[gl_VertexID].dir_move_normalized[0], ssbo_particles.particles[gl_VertexID].dir_move_normalized[1],
    //     ssbo_particles.particles[gl_VertexID].dir_move_normalized[2]);
    // pos_cur += dir_move_normalized * p.move_speed_secs * ssbo_particles.prev_frame_secs;

    //     //  alpha
    // float disappear_start_secs = p.life_secs_total - ssbo_particles.disappear_secs;
    // outV.life_time_alpha = life_secs_cur < ssbo_particles.appear_secs ? life_secs_cur / ssbo_particles.appear_secs  //  particle appear (life start)
    //     : disappear_start_secs < life_secs_cur ? 1.f - ((life_secs_cur - disappear_start_secs) / ssbo_particles.disappear_secs)  //  particle dissapear (life end)
    //     : 1.f;  //  full seen
    // // outV.life_time_alpha = 1.f;

    //     //  set data
    // ssbo_particles.particles[gl_VertexID].life_secs_cur = life_secs_cur;
    // ssbo_particles.particles[gl_VertexID].pos_cur[0] = pos_cur.x;
    // ssbo_particles.particles[gl_VertexID].pos_cur[1] = pos_cur.y;
    // ssbo_particles.particles[gl_VertexID].pos_cur[2] = pos_cur.z;

    // outV.uvs_cur_id = uvs_cur_id;
    // // outV.pos_cur = pos_cur;

    // if (gl_VertexID == 0)   //  on first vertex rotate corners to cam, they are same for each particle
    // {
    //     vec3 cam_right = vec3(camera.view[0].x, camera.view[1].x, camera.view[2].x);     //  normalized
    //     vec3 cam_up = vec3(camera.view[0].y, camera.view[1].y, camera.view[2].y);        //  normalized

    //     float left_x    = -1.f;
    //     float right_x   =  1.f;
    //     float top_y     =  1.f;
    //     float bottom_y  = -1.f;

    //     vec3 bl = (cam_right * left_x * ssbo_particles.half_width) + (cam_up * bottom_y * ssbo_particles.half_height);
    //     ssbo_particles.bl[0] = bl.x;
    //     ssbo_particles.bl[1] = bl.y;
    //     ssbo_particles.bl[2] = bl.z;

    //     vec3 br = (cam_right * right_x * ssbo_particles.half_width) + (cam_up * bottom_y * ssbo_particles.half_height);
    //     ssbo_particles.br[0] = br.x;
    //     ssbo_particles.br[1] = br.y;
    //     ssbo_particles.br[2] = br.z;

    //     vec3 tl = (cam_right * left_x * ssbo_particles.half_width) + (cam_up * top_y * ssbo_particles.half_height);
    //     ssbo_particles.tl[0] = tl.x;
    //     ssbo_particles.tl[1] = tl.y;
    //     ssbo_particles.tl[2] = tl.z;

    //     vec3 tr = (cam_right * right_x * ssbo_particles.half_width) + (cam_up * top_y * ssbo_particles.half_height);
    //     ssbo_particles.tr[0] = tr.x;
    //     ssbo_particles.tr[1] = tr.y;
    //     ssbo_particles.tr[2] = tr.z;
    // }

    // // outV.bl = vec3(ssbo_particles.bl[0], ssbo_particles.bl[1], ssbo_particles.bl[2]);
    // // outV.br = vec3(ssbo_particles.br[0], ssbo_particles.br[1], ssbo_particles.br[2]);
    // // outV.tl = vec3(ssbo_particles.tl[0], ssbo_particles.tl[1], ssbo_particles.tl[2]);
    // // outV.tr = vec3(ssbo_particles.tr[0], ssbo_particles.tr[1], ssbo_particles.tr[2]);















    // const float tex_tile_width = 128.f;     //  325.f;
    // const float tex_tile_height = 128.f;    //  325.f;
    // const int tex_columns_count = 4;
    // const int tex_rows_count = 4;
    // const float tex_widht = tex_tile_width * tex_columns_count;
    // const float tex_height = tex_tile_height * tex_rows_count;
    // const int atlas_size = tex_columns_count * tex_rows_count;
    
    // const float cur_time_seconds = unData[0].x;
    // const float tiles_per_second = 1.f / unData[0].y;
    // float tiles_passed = floor(cur_time_seconds / tiles_per_second);   //  amoun of the tiles passed from the start of the time
    // float atlases_passed = floor(tiles_passed / atlas_size);    //  amount of atlases passed from the start of the time
    // int cur_tile_index = int(tiles_passed - (atlas_size * atlases_passed));
    
    // int cur_tile_tex_row_index = cur_tile_index / tex_columns_count;
    // int cur_tile_tex_column_index = cur_tile_index - (tex_columns_count * cur_tile_tex_row_index);
    //     //  read atlas from top left to bottom right
    // outV.tex_left_x = cur_tile_tex_column_index * tex_tile_width;
    // outV.tex_top_y = tex_height - (cur_tile_tex_row_index * tex_tile_height);   //  opengl start count texture coords from bottom left, we are moves from top left
    // outV.tex_right_x = outV.tex_left_x + tex_tile_width;
    // outV.tex_bottom_y = outV.tex_top_y - tex_tile_height;
    //     //  normalize tex coords
    // outV.tex_left_x /= tex_widht;
    // outV.tex_top_y /= tex_height;
    // outV.tex_right_x /= tex_widht;
    // outV.tex_bottom_y /= tex_height;

    //     //  cur life time
    // const float particle_life_time = inPosition.w;
    // float lives_passed = floor(cur_time_seconds / particle_life_time);
    // float cur_life_time_seconds = cur_time_seconds - (particle_life_time * lives_passed);

    //     //  particle alpha
    // const float appear_disappear_seconds = unData[1].x;
    // const float disappear_start_seconds = particle_life_time - appear_disappear_seconds;
    // float life_time_alpha = 1.f;
    // if (cur_life_time_seconds < appear_disappear_seconds) life_time_alpha = cur_life_time_seconds / appear_disappear_seconds;  //  particle appear (life start)
    // else if (disappear_start_seconds < cur_life_time_seconds) life_time_alpha = 1.f - (cur_life_time_seconds - disappear_start_seconds) / appear_disappear_seconds;  //  particle dissapear (life end)
    
    //     //  move pos
    // const vec3 start_pos = inPosition.xyz;
    // const vec3 move_to = vec3(unData[2].x, unData[2].y, unData[2].z);
    // vec3 dir_pos_to_move_to = move_to - start_pos;
    // const float move_speed_seconds = unData[2].w;
    // float dist_pos_to_move_to = move_speed_seconds * cur_life_time_seconds;
    // vec3 pos = MoveByLength(start_pos, dir_pos_to_move_to, dist_pos_to_move_to);

    // gl_Position = vec4(pos, life_time_alpha);


    // gl_Position = vec4(inPosition.xyz, life_time_alpha);
}