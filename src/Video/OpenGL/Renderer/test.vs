
// // //                                      instancing + sso st430
// #version 460 core
// //  in
// layout (location = 0) in vec2 inPosition;

// struct InData
// {
//     vec4 color;
//     mat4 model;
// };

// layout (std430, binding = 0) buffer InDataAll
// {
//     InData inData[];
// } inDataAll;

// //  out
// layout (location = 0) out Out
// {
//     uint drawID;
// };

// void main()
// {
//     gl_Position = inDataAll.inData[gl_InstanceID].model * vec4(inPosition, 0, 1);
//     drawID = gl_InstanceID;
// }




// //                         //  shader pipeline + (tes evaluation shader + tes control from cpu (need make tes con shader))
// #version 460 core

// layout (location = 0) in vec2 inPosition;
// layout (location = 1) in vec3 inColor;

// layout (location = 0) out Out { vec3 color; } outD;

// void main()
// {
//     gl_Position = vec4(inPosition, 0, 1);
//     outD.color = inColor;
// }




// //                               	texture array
// #version 460 core
// layout (location = 0) in vec2 inPosition;
// layout (location = 1) in vec2 inTexCoord;

// layout (location = 0) uniform uint layer;

// layout (location = 0) out OutV
// {
//     vec2 texCoord;
//     uint layer;
// } outV;

// void main()
// {
//     gl_Position = vec4(inPosition, 0, 1);
//     outV.texCoord = inTexCoord;
//     outV.layer = layer;
// }





//                    //  uses -> VertexAtribFormat, no metter call glVertexArrayVertexBuffers(...) or glVertexArrayVertexBuffer(...)
// #version 460 core
// layout (location = 0) in vec2 inPosition;
// layout (location = 1) in float r;
// layout (location = 2) in float g;
// layout (location = 3) in float b;

// out vec4 vColor;

// void main()
// {
//     gl_Position = vec4(inPosition, 0, 1);
//     vColor = vec4(r, g, b, 1);
// };



//     //  uses -> std430 buffer
// #version 460 core
// layout (location = 0) in vec2 inPosition;

// out vec4 vColor;

// layout (location = 11) uniform uint Index;
// struct MyData
// {
//     float posX, posY;
//     float r, g, b, a;
// };
// layout (std430, binding = 5) buffer MyUniform
// {
//     MyData myData[];
// };

// void main()
// {
//         //  uses -> std430 buffer
//     gl_Position = vec4(inPosition.x + myData[Index].posX, inPosition.y + myData[Index].posY, 0.f, 1.f);
//     vColor = vec4(myData[Index].r, myData[Index].g, myData[Index].b, myData[Index].a);
//     if (Index == 1)
//     {
//         myData[Index].r = 1;
//         myData[Index].g = 0;
//         myData[Index].b = 0;
//     }
// };





// //                                 //  uses -> uniform block
// #version 460 core
// layout (location = 0) in vec2 inPosition;

// out vec4 vColor;

//     //  uses -> uniform block
// layout (binding = 1) uniform Data
// {
//     vec2 pos;
//     float r, g, b;
// } data;

// void main()
// {
//         //  uses -> uniform block
//     gl_Position = vec4(inPosition + data.pos, 0, 1);
//     vColor = vec4(data.r, data.g, data.b, 1);
// };



// //                          ssbo withought vao
// #version 460 core
//     //  in
// struct Vertex
// {
//     float[2] pos;
//     float[2] uv;
// };

// layout (std430, binding = 0) readonly buffer InVertex
// {
//     Vertex verts[];
// };

// // layout (std430, binding = 1) readonly buffer InIndex
// // {
// //     uint indexes[];
// // };

//     //  out
// layout (location = 0) out Out
// {
//     vec2 texCoords;
// };


// void main()
// {
//     // uint index = indexes[gl_VertexID];   //  for glDraw*Elements operation is elemnts(indexes) counter, for glDraw*Arrays is vertex counter
//     gl_Position = vec4(verts[gl_VertexID].pos[0], verts[gl_VertexID].pos[1], 0, 1);
//     texCoords = vec2(verts[gl_VertexID].uv[0], verts[gl_VertexID].uv[1]);
// }




//                          //      glMultiDrawElementsIndirect
#version 460 core

//  in
struct Vertex
{
    float x, y;
    float u, v;
};
layout (std430, binding = 0) buffer InVertex { Vertex vert[]; } inV;

struct Data
{
    float posX, posY;
};
layout (std430, binding = 1) buffer InData { Data data[]; } inD;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};

//  out
layout (location = 0) out OutV
{
    vec2 uv;
} outV;


void main()
{
    Vertex vert = inV.vert[gl_VertexID];
    Data d = inD.data[abs(gl_VertexID / 6)];
    // gl_Position = ortho * vec4(vert.x, vert.y, 0, 1);
    gl_Position = ortho * vec4(vert.x + d.posX, vert.y + d.posY, 0, 1);
    outV.uv = vec2(vert.u, vert.v);
}