#version 460 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;     // 2 points for one line
in vec3 color[];

out vec3 vColor;

void main()
{
    vColor = color[0];
    
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = vec4(0.f, 0.f, 0.f, 1.f); //  draw scene center as second point of line
    EmitVertex();

    EndPrimitive();
}