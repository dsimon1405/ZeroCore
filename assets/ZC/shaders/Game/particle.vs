#version 460 core   //  color.fs

layout (location = 0) in vec4 inPosition;   //  4 is alpha channel, colors don't updated so alpha with vertices
layout (location = 1) in vec3 inColor;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    mat4 perspViewSkybox;
    vec3 camPos;
};

uniform float unAlpha;  //  total alpha of the object (wind particles for example)

out vec4 vColor;

void main()
{
    gl_PointSize = 3;   //  ZC_SWindow::GLEnablePointSize()
    // gl_PointSize = 1.5;   //  ZC_SWindow::GLEnablePointSize()
    vColor = vec4(inColor, inPosition.a * unAlpha);
    gl_Position = perspView * vec4(inPosition.xyz, 1.0);    //  exclude alpha from vertex
}