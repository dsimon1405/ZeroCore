#version 460 core
    //  in
layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 norm;  //  packed in GL_INT_REV_2_10_10_10, last 2 bytes store object id : -1(sphere playable), 0(platform), 1(sphere map). Packed in G_ModelLoader::CreateDrawerSet()
layout(location = 2) in vec2 tex;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspective;
    mat4 perspView;
    mat4 perspViewSkybox;
    mat4 view;
    vec3 camPos;
} camera;

uniform mat4 unModel;

    //  out
layout (location = 0) out OutV
{
    float frag_z;
} outV;

void main()
{
    vec4 pos = unModel * vec4(pos, 1.f);;
    outV.frag_z = pos.z;

    gl_Position = camera.perspView * pos;
}