#version 460 core   //  flame.gs
    //  in
layout (location = 0) in vec4 inPosition;   //  inPositin.w is life time

    /*
    data[0].x - cur time in seconds
    data[0].y - tiles per second
    data[0].z - particle widht
    data[0].w - particle height

    data[1].x - appear/disappear seconds. other time particle have alpha 1.f
    data[1].y - pos x
    data[1].z - pos y
    data[1].y - pos z

    data[2].x - move to x
    data[2].y - move to y
    data[2].z - move to z
    data[2].w - move speed
    */
layout (location = 0) uniform mat4 unData;


    //  out
layout (location = 0) out OutV
{
    float tex_left_x;
    float tex_top_y;
    float tex_right_x;
    float tex_bottom_y;
} outV;


    //  function
vec3 MoveByLength(vec3 v, vec3 direction, float length);

void main()
{
    const float tex_tile_width = 128.f;     //  325.f;
    const float tex_tile_height = 128.f;    //  325.f;
    const int tex_columns_count = 4;
    const int tex_rows_count = 4;
    const float tex_widht = tex_tile_width * tex_columns_count;
    const float tex_height = tex_tile_height * tex_rows_count;
    const int atlas_size = tex_columns_count * tex_rows_count;
    
    const float cur_time_seconds = unData[0].x;
    const float tiles_per_second = 1.f / unData[0].y;
    float tiles_passed = floor(cur_time_seconds / tiles_per_second);   //  amoun of the tiles passed from the start of the time
    float atlases_passed = floor(tiles_passed / atlas_size);    //  amount of atlases passed from the start of the time
    int cur_tile_index = int(tiles_passed - (atlas_size * atlases_passed));
    
    int cur_tile_tex_row_index = cur_tile_index / tex_columns_count;
    int cur_tile_tex_column_index = cur_tile_index - (tex_columns_count * cur_tile_tex_row_index);
        //  read atlas from top left to bottom right
    outV.tex_left_x = cur_tile_tex_column_index * tex_tile_width;
    outV.tex_top_y = tex_height - (cur_tile_tex_row_index * tex_tile_height);   //  opengl start count texture coords from bottom left, we are moves from top left
    outV.tex_right_x = outV.tex_left_x + tex_tile_width;
    outV.tex_bottom_y = outV.tex_top_y - tex_tile_height;
        //  normalize tex coords
    outV.tex_left_x /= tex_widht;
    outV.tex_top_y /= tex_height;
    outV.tex_right_x /= tex_widht;
    outV.tex_bottom_y /= tex_height;

        //  cur life time
    const float particle_life_time = inPosition.w;
    float lives_passed = floor(cur_time_seconds / particle_life_time);
    float cur_life_time_seconds = cur_time_seconds - (particle_life_time * lives_passed);

        //  particle alpha
    const float appear_disappear_seconds = unData[1].x;
    const float disappear_start_seconds = particle_life_time - appear_disappear_seconds;
    float life_time_alpha = 1.f;
    if (cur_life_time_seconds < appear_disappear_seconds) life_time_alpha = cur_life_time_seconds / appear_disappear_seconds;  //  particle appear (life start)
    else if (disappear_start_seconds < cur_life_time_seconds) life_time_alpha = 1.f - (cur_life_time_seconds - disappear_start_seconds) / appear_disappear_seconds;  //  particle dissapear (life end)
    
        //  move pos
    const vec3 start_pos = inPosition.xyz;
    const vec3 move_to = vec3(unData[2].x, unData[2].y, unData[2].z);
    vec3 dir_pos_to_move_to = move_to - start_pos;
    const float move_speed_seconds = unData[2].w;
    float dist_pos_to_move_to = move_speed_seconds * cur_life_time_seconds;
    vec3 pos = MoveByLength(start_pos, dir_pos_to_move_to, dist_pos_to_move_to);

    gl_Position = vec4(pos, life_time_alpha);


    // gl_Position = vec4(inPosition.xyz, life_time_alpha);
}

vec3 MoveByLength(vec3 v, vec3 direction, float length)
{
    return v + (direction * (length / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z)));
}