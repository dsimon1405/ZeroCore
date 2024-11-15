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

/*
data[0].x - cur time in seconds
data[0].y - change tiles in second (like frames in second fps)
data[0].z - particle widht
data[0].w - particle height

data[1].x - appear/disappear seconds. other time particle have alpha 1.f
*/
layout (location = 0) uniform mat4 unData;

layout (location = 0) in InG
{
    vec3 pos;
    float tex_left_x;
    float tex_top_y;
    float tex_right_x;
    float tex_bottom_y;
    float life_time_alpha;
} inG[];


    //  out
layout (location = 0) out OutG
{
    float life_time_alpha;
    vec2 tex_coords;
} outG;


    //  functions
void SetVertexData(vec3 pos, vec2 tex_coords)
{
    gl_Position = perspView * vec4(pos, 1.f);
    outG.tex_coords = tex_coords;
    EmitVertex();
}

void main()
{
    outG.life_time_alpha = 1.f;
    // outG.life_time_alpha = inG[gl_PrimitiveIDIn].life_time_alpha;

    vec3 cam_right = vec3(view[0].x, view[1].x, view[2].x);     //  normalized
    vec3 cam_up = vec3(view[0].y, view[1].y, view[2].y);        //  normalized
        //  calculate particle's corners positions
    float particle_half_width = unData[0].z / 2.f;
    float particle_half_height = unData[0].w / 2.f;
    const vec2 corner_bl = vec2(-1.f, -1.f);
    const vec2 corner_br = vec2( 1.f, -1.f);
    const vec2 corner_tl = vec2(-1.f,  1.f);
    const vec2 corner_tr = vec2( 1.f,  1.f);
    vec3 pos_center = inG[gl_PrimitiveIDIn].pos;

    vec3 bl = pos_center + ((cam_right * corner_bl.x * particle_half_width) + (cam_up * corner_bl.y * particle_half_height));
    SetVertexData(bl, vec2(inG[gl_PrimitiveIDIn].tex_left_x, inG[gl_PrimitiveIDIn].tex_bottom_y));

    vec3 br = pos_center + ((cam_right * corner_br.x * particle_half_width) + (cam_up * corner_br.y * particle_half_height));
    SetVertexData(br, vec2(inG[gl_PrimitiveIDIn].tex_right_x, inG[gl_PrimitiveIDIn].tex_bottom_y));

    vec3 tl = pos_center + ((cam_right * corner_tl.x * particle_half_width) + (cam_up * corner_tl.y * particle_half_height));
    SetVertexData(tl, vec2(inG[gl_PrimitiveIDIn].tex_left_x, inG[gl_PrimitiveIDIn].tex_top_y));

    vec3 tr = pos_center + ((cam_right * corner_tr.x * particle_half_width) + (cam_up * corner_tr.y * particle_half_height));
    SetVertexData(tr, vec2(inG[gl_PrimitiveIDIn].tex_right_x, inG[gl_PrimitiveIDIn].tex_top_y));

    EndPrimitive();
}