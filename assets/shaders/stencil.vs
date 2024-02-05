#version 460 core

layout (location = 0) in vec3 inPosition;
layout (std140, binding = 0) uniform Persp { mat4 perspView; };
uniform mat4 model;
uniform vec3 color;

out vec3 vColor;

void main()
{
    gl_Position = perspView * model * vec4(inPosition, 1.f);
    vColor = color;
}