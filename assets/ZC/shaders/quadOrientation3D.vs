#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoords;

uniform mat4 unModel;   //  consider persepctive * view * model -> look in ZCR_Orientation3D::

out vec2 vTexCoords;

void main()
{
    gl_Position = unModel * vec4(inPosition, 1.0);
    vTexCoords = inTexCoords;
}