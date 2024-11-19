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
};

layout (location = 0) in InG    //  ALL VARIABLES MUST BE INVOLVED INTO THE SOME FUCNTION OR GONNA BE PROBLEM WITH ALIGNMENT (I THOUGHT THAT WITH ALIGNMENT. DATA AFTER NOT INVOLVED FIELD IS NOT CORRECT)
{
    float tex_left_x;
    float tex_top_y;
    float tex_right_x;
    float tex_bottom_y;
} inG[];

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
    data[3].w - move speed min secs

    data[3].x - move speed max secs
    */
layout (location = 0) uniform mat4 unData;


    //  out
layout (location = 0) out OutG
{
    float life_time_alpha;
    vec2 tex_coords;
} outG;


    //  functions
void SetVertexData(vec3 quad_corner, vec2 tex_coords)
{
    outG.tex_coords = tex_coords;
    vec3 particles_origin_pos = unData[1].yzw;
    gl_Position = perspView * vec4(particles_origin_pos + gl_in[0].gl_Position.xyz + quad_corner, 1.f);
    EmitVertex();
}

    //  drawing from points to quads need to take ID = 0, NOT gl_PrimitiveIDIn! For gl_in[0] and in block inG[0]
void main()
{
    outG.life_time_alpha = gl_in[0].gl_Position.w;   //  in gs put alpha to .w

        //  calculate particle's corners positions
    float particle_half_width = unData[0].z / 2.f;
    float particle_half_height = unData[0].w / 2.f;

    vec3 cam_right = vec3(view[0].x, view[1].x, view[2].x);     //  normalized
    vec3 cam_up = vec3(view[0].y, view[1].y, view[2].y);        //  normalized

    const vec2 corner_bl = vec2(-1.f, -1.f);
    const vec2 corner_br = vec2( 1.f, -1.f);
    const vec2 corner_tl = vec2(-1.f,  1.f);
    const vec2 corner_tr = vec2( 1.f,  1.f);
    
    vec3 bl = (cam_right * corner_bl.x * particle_half_width) + (cam_up * corner_bl.y * particle_half_height);    //  rotate corner in origin face to cam
    SetVertexData(bl, vec2(inG[0].tex_left_x, inG[0].tex_bottom_y));

    vec3 br = (cam_right * corner_br.x * particle_half_width) + (cam_up * corner_br.y * particle_half_height);    //  rotate corner in origin face to cam
    SetVertexData(br, vec2(inG[0].tex_right_x, inG[0].tex_bottom_y));

    vec3 tl = (cam_right * corner_tl.x * particle_half_width) + (cam_up * corner_tl.y * particle_half_height);    //  rotate corner in origin face to cam
    SetVertexData(tl, vec2(inG[0].tex_left_x, inG[0].tex_top_y));

    vec3 tr = (cam_right * corner_tr.x * particle_half_width) + (cam_up * corner_tr.y * particle_half_height);    //  rotate corner in origin face to cam
    SetVertexData(tr, vec2(inG[0].tex_right_x, inG[0].tex_top_y));

    EndPrimitive();
}