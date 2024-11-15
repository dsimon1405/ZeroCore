#version 460 core   //  flame.gs
    //  in
layout (location = 0) in vec4 inPosition;   //  inPositin.w is life time

/*
data[0].x - cur time in seconds
data[0].y - tiles per second
data[0].z - particle widht
data[0].w - particle height

data[1].x - appear/disappear seconds. other time particle have alpha 1.f
*/
layout (location = 0) uniform mat4 unData;


    //  out
layout (location = 0) out OutV
{
    vec3 pos;
    float tex_left_x;
    float tex_top_y;
    float tex_right_x;
    float tex_bottom_y;
    float life_time_alpha;
} outV;

void main()
{
    const float tex_tile_width = 325.f;
    const float tex_tile_height = 325.f;
    const int tex_columns_count = 4;
    const int tex_rows_count = 4;
    const int atlas_size = tex_columns_count * tex_rows_count;
    
    const float cur_time_seconds = unData[0].x;
    const float tiles_per_second = 1.f / unData[0].y;
    float tiles_passed = floor(cur_time_seconds / tiles_per_second);   //  amoun of the tiles passed from the start of the time
    float atlases_passed = floor(tiles_passed / atlas_size);    //  amount of atlases passed from the start of the time
    int cur_tile_index = int(tiles_passed - (atlas_size * atlases_passed));
    int cur_tile_tex_row_index = cur_tile_index / tex_columns_count;
    int cur_tile_tex_column_index = cur_tile_index - (tex_columns_count * cur_tile_tex_row_index);
        //  read atlas from top left to bottom right
    const float tex_height = tex_tile_height * tex_rows_count;
    outV.tex_left_x = cur_tile_tex_column_index * tex_tile_width;
    outV.tex_top_y = tex_height - (cur_tile_tex_row_index * tex_tile_height);   //  opengl start count texture coords from bottom left, we are moves from top left
    outV.tex_right_x = outV.tex_left_x + tex_tile_width;
    outV.tex_bottom_y = outV.tex_top_y - tex_tile_height;

        //  particle alpha
    const float particle_life_time = inPosition.w;
    float lives_passed = floor(cur_time_seconds / particle_life_time);
    float cur_life_time_seconds = cur_time_seconds - (particle_life_time * lives_passed);
    const float appear_disappear_seconds = unData[1].x;
    const float disappear_start_seconds = particle_life_time - appear_disappear_seconds;
    if (cur_life_time_seconds < appear_disappear_seconds) outV.life_time_alpha = cur_life_time_seconds / appear_disappear_seconds;  //  particle appear (life start)
    else if (disappear_start_seconds < cur_life_time_seconds) outV.life_time_alpha = 1.f - (cur_life_time_seconds - disappear_start_seconds) / appear_disappear_seconds;  //  particle dissapear (life end)
    else outV.life_time_alpha = 1.f;    //  particle lives

    outV.pos = inPosition.xyz;

    gl_Position = vec4(0.f, 0.f, 0.f, 0.f);
}