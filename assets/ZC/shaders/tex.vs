#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 3) in vec2 inTexCoords;

layout (std140, binding = 0) uniform Persp { mat4 perspView; };

uniform mat4 unModel;

out vec2 vTexCoords;

void main()
{
    gl_Position = perspView * unModel * vec4(inPosition, 1.0);
    vTexCoords = inTexCoords;
}