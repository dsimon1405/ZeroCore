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

layout (std140, binding = 1) uniform Light
{
    vec4 pos;
    vec4 color;
} light;

uniform mat4 unModel;

    //  out
layout (location = 0) out OutV
{
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords;
    float color_coef;   //  coef on dmg color
} outV;

void main()
{
    outV.frag_pos = vec3(unModel * vec4(pos,1));
    outV.normal = mat3(transpose(inverse(unModel))) * norm;
    outV.tex_coords = tex;
    gl_Position =  perspView * unModel * vec4(pos, 1);
        //  calculate coef on dmg color
    vec3 normal = normalize(mat3(unModel) * norm.xyz);   //  rotate and scale normals (wrong results for scaled object, but tolerable)
    vec3 dirPosToLight = normalize(light.pos.xyz - outV.frag_pos.xyz);
    float cos = dot(dirPosToLight, normal); //  range (-1,1)
    outV.color_coef = (cos + 1.f) / 2.f;   //  from (-1,1) to (0,1)
}