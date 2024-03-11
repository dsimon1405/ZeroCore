#version 460 core

layout (location = 0) in vec3 inPosition;
layout (std140, binding = 0) uniform Persp { mat4 perspView; };
uniform mat4 unModel;
uniform uint unColor;

out vec3 vColor;

void main()
{
    gl_Position = perspView * unModel * vec4(inPosition, 1.f);
    float r = (unColor >> 20) / 255.f;
    float g = (unColor >> 10 & uint(1023)) / 255.f;
    float b = (unColor & uint(1023)) / 255.f;
    vColor = vec3(r, g, b);
}