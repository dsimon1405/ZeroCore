#version 460 core   //  color.fs

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    mat4 perspViewSkybox;
    vec3 camPos;
};
uniform mat4 unModel;

out vec4 vColor;

void main()
{
    gl_PointSize = 6;   //  ZC_SWindow::GLEnablePointSize()
    gl_Position = perspView * unModel * vec4(inPosition, 1.0);
    vColor = vec4(inColor, 1.f);
}