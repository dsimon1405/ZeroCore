#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};

//  out
layout (location = 0) out OutG
{
    vec2 uv;
};

float win_width = 1850;
float win_height = 600;

void main()
{
    float y_offset = 0.2;

    uv = vec2(0, 0);
    gl_Position = ortho * vec4(0, 0, 0, 1);
    gl_Position.y += y_offset;
    EmitVertex();

    uv = vec2(1, 0);
    gl_Position = ortho * vec4(win_width, 0, 0, 1);
    gl_Position.y += y_offset;
    EmitVertex();

    uv = vec2(0, 1);
    gl_Position = ortho * vec4(0, win_height / 10, 0, 1);
    gl_Position.y += y_offset;
    EmitVertex();

    uv = vec2(1, 1);
    gl_Position = ortho * vec4(win_width, win_height / 10, 0, 1);
    gl_Position.y += y_offset;
    EmitVertex();

    EndPrimitive();
}