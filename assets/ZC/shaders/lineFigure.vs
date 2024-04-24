#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};

uniform mat4 unModel;

out vec4 vColor;

void main()
{
    vec4 position = unModel * vec4(inPosition, 1.f);
    vec3 dirPosToCam = normalize(camPos - vec3(position).xyz) * 0.01f;
    vec3 newPos = inPosition + dirPosToCam;
    gl_Position = perspView * unModel * vec4(newPos, 1.f);

    vColor = vec4(inColor, 1.f);
}