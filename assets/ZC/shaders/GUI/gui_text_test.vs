#version 460 core

layout (location = 0) in vec2 in_pos;
layout (location = 1) in vec2 in_uv;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspective;
    mat4 perspView;
    mat4 perspViewSkybox;
    mat4 view;
    vec3 camPos;
};

layout (location = 0) out outF
{
    vec2 uv;
};

void main()
{
    uv = in_uv;
    gl_Position = ortho * vec4(in_pos, 0.f, 1.f);
}