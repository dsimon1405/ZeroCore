#version 460 core

//  in
layout (std430, binding = 10) buffer InVertex { vec2 pos[]; } inV;

struct Data
{
    float width, height;
    //  uv[0] is u, uv[1] is v -> for one vertex, total 4 vertices is uv[2 * 4]
    float uv[8];
};
layout (std430, binding = 11) buffer InData { Data data[]; } inD;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};

layout (location = 0) out OutV
{
    int index;
} outV;

void main()
{
    outV.index = gl_DrawID * 1;
    // gl_Position = vec4(inV.pos[0], 0, 1);
    gl_Position = vec4(inV.pos[outV.index + gl_VertexID], 0, 1);
    // gl_Position = vec4(inV.pos[gl_VertexID], 0, 1);
}

//      
// di = gl_DrawIDi
// vi = gl_VertexIDi
// 
// [v0][v1][v2][v3]     [v0][v1][v2][v3]    [v0][v1][v2][v3]
//       [d0]                 [d1]                [d2]            
// 
// vc = 4 (GL_TRIAGNELS_STROP, 4 vertices in one quad)
// gl_Position = (di * vc) + vi
// data = di * vc