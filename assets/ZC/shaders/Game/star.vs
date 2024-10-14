#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    mat4 perspViewSkybox;
    vec3 camPos;
};

uniform mat4 unModel;

out vec2 vTexCoords;

void main()
{
    vTexCoords = tex;
    gl_Position =  perspView * unModel * vec4(pos, 1);
}