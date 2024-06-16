#version 460 core

//                 //      vert + frag
// //  in
// layout (std430, binding = 10) buffer InVertex { vec2 pos[]; } inV;

// struct Data
// {
//     float width, height;
//     //  uv[0] is u, uv[1] is v -> for one vertex, total 4 vertices is uv[2 * 4]
//     float uv[8];
// };
// layout (std430, binding = 11) buffer InData { Data data[]; } inD;

// layout (std140, binding = 0) uniform Camera
// {
//     mat4 ortho;
//     mat4 perspView;
//     vec3 camPos;
// };

// layout (location = 0) out OutV
// {
//     vec2 uv;
// } outV;

// void main()
// {
//     Data data = inD.data[0];
//     vec2 pos = inV.pos[0];     //  bl
//     switch (gl_VertexID % 4)
//     {
//         case 0: outV.uv = vec2(data.uv[0], data.uv[1]); break;     //  bl
//         case 1:     //  br
//         {
//             pos.x += data.width;
//             outV.uv = vec2(data.uv[2], data.uv[3]);
//         } break;
//         case 2:     //  tl
//         {
//             pos.y += data.height;
//             outV.uv = vec2(data.uv[4], data.uv[5]);
//         } break;
//         case 3:     //  tr
//         {
//             pos.x += data.width;
//             pos.y += data.height;
//             outV.uv = vec2(data.uv[6], data.uv[7]);
//         } break;
//     }
//     gl_Position = ortho * vec4(pos, 0, 1);
//     // Data data = inD.data[gl_InstanceID];
//     // vec2 pos = inV.pos[gl_InstanceID];     //  bl
//     // switch (gl_VertexID)
//     // {
//     //     case 0: outV.uv = vec2(data.uv[0], data.uv[1]); break;     //  bl
//     //     case 1:     //  br
//     //     {
//     //         pos.x += data.width;
//     //         outV.uv = vec2(data.uv[2], data.uv[3]);
//     //     } break;
//     //     case 2:     //  tl
//     //     {
//     //         pos.y += data.height;
//     //         outV.uv = vec2(data.uv[4], data.uv[5]);
//     //     } break;
//     //     case 3:     //  tr
//     //     {
//     //         pos.x += data.width;
//     //         pos.y += data.height;
//     //         outV.uv = vec2(data.uv[6], data.uv[7]);
//     //     } break;
//     // }
//     // gl_Position = ortho * vec4(pos, 0, 1);
// }



//                 //      vert + geon + frag
// //  in
// layout (std430, binding = 10) buffer InVertex { vec2 pos[]; } inV;

// struct ObjData
// {
//     float width, height;
//     //  uv[0] is u, uv[1] is v -> for one vertex, total 4 vertices is uv[2 * 4]
//     float uv[8];
// };
// layout (std430, binding = 11) buffer InObjData { ObjData data[]; } inOD;

// layout (std140, binding = 0) uniform Camera
// {
//     mat4 ortho;
//     mat4 perspView;
//     vec3 camPos;
// };

// layout (location = 0) out OutV
// {
//     int index;
// } outV;

// void main()
// {
//         //  use instace rendering
//     // outV.index = gl_InstanceID;
//     // gl_Position = vec4(inV.pos[outV.index], 0, 1);

//     //     //  use vertex count for rendering
//     // outV.index = gl_VertexID;
//     // gl_Position = vec4(inV.pos[outV.index], 0, 1);

//         //  use vertex count for rendering (now gl_BaseInstance store start index for drawing command (and is data of window,
//         //  first object to draw in drawing command is window). Sets in comand)
//     vec2 posWin = inV.pos[gl_BaseInstance];
//     outV.index = gl_BaseInstance + gl_VertexID;     //  index of current object
//     vec2 posObj = inV.pos[outV.index];
//     gl_Position = vec4(posWin + posObj, 0, 1);
// }


    //  in
struct ZC_GUI_Border
{
    vec2 bl;     //  coord of frame border, to avoid drawing objects out of window
    vec2 tr;
};
layout (std430, binding = 0) readonly buffer InBorder { ZC_GUI_Border borders[]; } inBorder;

layout (std430, binding = 1) readonly buffer InVertex { vec2 bls[]; } inBL;

struct ZC_GUI_ObjData 
{
    float width;
    float height;
    float depth;    //  first element is window and it's start window depth, each other object must calculate own depth from that depth
    float uv[4];    //  bl, tr
    int borderIndex;
    uint textureIndex;
};
layout (std430, binding = 2) readonly buffer InObjData { ZC_GUI_ObjData objDatas[]; } inObjData;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};
    //  out
layout (location = 0) out OutV
{
    int baseInstance;   //  total offset in: bls, objDatas (uses in glMutiDrawArraysIndirect, sets in it's command struct, by default is 0 for glDrawArrays)
    int vertexID;       //  relative offset in: bls, objDatas
    
} outV;

void main()
{
    // ZC_GUI_CommandSet cs = inCS.cs[gl_DrawID];  //  data of the drawing command, may be window into the window
    // vec2 commandPos = vec2(cs.bl_x, cs.bl_y);   //  may be window into the window
    // if (gl_DrawID != 0 || gl_VertexID != 0) commandPos += inV.pos[0];   //  if now drawing not window suface, add start window position
    
    outV.baseInstance = gl_BaseInstance;
    outV.vertexID = gl_VertexID;
    // outV.drawID = gl_DrawID;
    // commandPos += inV.pos[outV.index];    //  finaly get element position
    gl_Position = vec4(0,0,0,0);
    // gl_Position = vec4(commandPos, , 1);

    // vec2 posWin = inV.pos[gl_BaseInstance];
    // outV.index = gl_BaseInstance + gl_VertexID;     //  index of current object
    // vec2 posObj = inV.pos[outV.index];
    // gl_Position = vec4(posWin + posObj, 0, 1);
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


