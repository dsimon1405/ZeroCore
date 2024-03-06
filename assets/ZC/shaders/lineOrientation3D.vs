#version 460 core

layout (location = 0) in vec4 inPosition;   //  consider perspective * view * model * position -> look in ZCR_Orientation3D::MouseMoveAroundObject()
layout (location = 1) in vec3 inColor;

out vec3 color;

void main()
{
    gl_Position = inPosition;
    color = inColor;
}