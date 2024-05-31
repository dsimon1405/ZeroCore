#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

//  in
layout (location = 0) in InF
{
    int index;
} inF[];

struct Data
{
    float width, height;
    //  uv[0] is u, uv[1] is v -> for one vertex, total 4 vertices is uv[2 * 4]
    float uv[8];
};
layout (std430, binding = 11) buffer InData { Data data[]; } inD;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};

//  out
layout (location = 0) out OutF
{
    vec2 uv;
} outF;

void main()
{
    // Data data = inD.data[gl_PrimitiveIDIn];
    // vec4 startPos = gl_in[gl_PrimitiveIDIn].gl_Position;
    Data data = inD.data[inF[gl_PrimitiveIDIn].index];
    vec4 startPos = gl_in[inF[gl_PrimitiveIDIn].index].gl_Position;

    //  uses quad shema bl, br, tl, tr caurse triangle_strip -> layout (triangle_strip, max_vertices = 4) out;
    //  bl
    outF.uv = vec2(data.uv[0], data.uv[1]);
    gl_Position = ortho * startPos;
    EmitVertex();
    //  br
    outF.uv = vec2(data.uv[2], data.uv[3]);
    gl_Position = startPos;
    gl_Position.x += data.width;
    gl_Position = ortho * gl_Position;
    EmitVertex();
    //  tl
    outF.uv = vec2(data.uv[4], data.uv[5]);
    gl_Position = startPos;
    gl_Position.y += data.height;
    gl_Position = ortho * gl_Position;
    EmitVertex();
    //  tr
    outF.uv = vec2(data.uv[6], data.uv[7]);
    gl_Position = startPos;
    gl_Position.x += data.width;
    gl_Position.y += data.height;
    gl_Position = ortho * gl_Position;
    EmitVertex();
    
    EndPrimitive();
}