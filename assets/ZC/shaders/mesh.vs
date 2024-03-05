#version 460 core

layout (location = 0) in vec3 inPosition;

layout (std140, binding = 0) uniform Persp { mat4 perspView; };
layout (std140, binding = 2) uniform CamPos { vec3 camPos; };
uniform mat4 unModel;

out vec3 vColor;

void main()
{
    vec4 worldPsition = unModel * vec4(inPosition, 1.f);
    vec3 dirPosToCam = normalize(camPos - vec3(worldPsition).xyz) * 0.02f;
    vec3 newPos = inPosition + dirPosToCam;
    gl_Position = perspView * unModel * vec4(newPos, 1.f);

    vColor = worldPsition.y == 0.f && worldPsition.z == 0 ? vec3(0.8f, 0.f, 0.1f)
        : worldPsition.x == 0.f && worldPsition.z == 0.f ? vec3(0.f, 0.6f, 0.f)
        : worldPsition.x == 0.f || worldPsition.y == 0.f ? vec3(0.f, 0.f, 0.8f)
        : vec3(0.37f,0.37f,0.37f);
}