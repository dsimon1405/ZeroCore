#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

layout (std140, binding = 0) uniform Persp { mat4 perspView; };
uniform mat4 unModel;

out vec3 vColor;

void main()
{
    gl_PointSize = 6;
    gl_Position = perspView * unModel * vec4(inPosition, 1.0);
    vColor = inColor;
}