#version 460 core

layout (location = 0) in vec3 inPosition;
layout (std140, binding = 0) uniform PerspView { mat4 perspView; };

out mat4 vPerspView;

void main()
{
    gl_Position = vec4(inPosition,1.f);
    vPerspView = perspView;
}