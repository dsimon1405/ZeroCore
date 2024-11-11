#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    mat4 perspViewSkybox;
    vec3 camPos;
};

uniform mat4 unModel;

    //  out
layout (location = 0) out OutV
{
    vec2 tex_coords;
    bool normal_aligned_to_cam;
} outV;

void main()
{
    outV.tex_coords = tex;

    vec4 frag_pos_v4 = unModel * vec4(pos, 1.f);
    gl_Position =  perspView * frag_pos_v4;
    
    vec3 normal = normalize(mat3(transpose(inverse(unModel))) * vec3(norm));
    outV.normal_aligned_to_cam = dot(normalize(frag_pos_v4.xyz - camPos), normal) > 0.f;
}