#version 460 core

layout (location = 0) in vec3 inPosition;
layout (std140, binding = 0) uniform Persp { mat4 perspView; };
uniform mat4 unModel;

out vec3 vColor;

void main()
{
    vec4 worldPsition = unModel * vec4(inPosition, 1.f);
    gl_Position = perspView * worldPsition;

    vColor =
        worldPsition.y == 0.f  && worldPsition.z == 0 ? vec3(1.f, 0.f, 0.f)
        : worldPsition.x == 0.f && worldPsition.z == 0.f ? vec3(0.f, 1.f, 0.f)
        : worldPsition.x == 0.f || worldPsition.y == 0.f ? vec3(0.f, 0.f, 1.f)
        : vec3(0.5f,0.5f,0.5f);
}