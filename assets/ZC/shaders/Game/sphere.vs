#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};

uniform mat4 unModel;

    //  out
layout (location = 0) out OutV
{
    vec2 tex_coords;
    float color_coef;
    
} outV;

void main()
{
    vec4 position = unModel * vec4(pos, 1);
    gl_Position =  perspView * position;

    // vec4 position = model * totalPosition;
    // gl_Position =  perspView * model * totalPosition;


    vec3 normal = normalize(mat3(unModel) * norm.xyz);   //  rotate and scale normals (wrong results for scaled object, but tolerable)

    vec3 light_pos = vec3(0, 0, 50);

    vec3 dirPosToLight = normalize(light_pos - position.xyz);
    float cos = dot(dirPosToLight, normal); //  range (-1,1)

    // vec3 dirPosToCam = normalize(camPos - position.xyz);
    // float cos = dot(dirPosToCam, normal); //  range (-1,1)

    float coef = (cos + 1.f) / 2.f;   //  from (-1,1) to (0,1)

    outV.color_coef = coef;
	outV.tex_coords = tex;
}