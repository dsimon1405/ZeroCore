#version 460 core   //  cube_map.fs

    //  in
layout (location = 0) in vec3 inPos;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspective;
    mat4 perspView;
    mat4 perspViewSkybox;
    mat4 view;
    vec3 camPos;
} camera;

    //  bindings
#define G_BIND_UBO_Light 1
        //  light[i]
#define Light_Star 0
#define Light_Platform 1
struct Light
{       //  vec3 + uitn = vec4 alignment
    vec3 pos;
    uint color;
};
layout (std140, binding = G_BIND_UBO_Light) uniform UBOLight
{
    Light light[2];     //  first Star, seond Platform -> look G_LightUBO
    vec4 attenuations[2];    //  [0].x/y - player-sphere; [0].z/y - platforms; [1].x/y - map sphere; [1].z/a - empty     //  ALIGNMENT =(
} ubo_light;


    //  out
layout (location = 0) out OutV
{
    vec3 pos_in_cube;
    vec3 light_color_star;
    float dist_to_frag;
} outV;


vec3 Uint_2_10_10_10_To_vec3(uint val);

void main()
{
    outV.pos_in_cube = inPos;
    outV.light_color_star = ubo_light.light[Light_Star].color != 0 ? Uint_2_10_10_10_To_vec3(ubo_light.light[Light_Star].color) : vec3(0.f, 0.f, 0.f);
    outV.dist_to_frag = length(ubo_light.light[Light_Star].pos) + 75.f;     //  75 - constant distance between the star and the edge of the map at all levels
    gl_Position = camera.perspViewSkybox * vec4(inPos, 1.0);
}


vec3 Uint_2_10_10_10_To_vec3(uint val)
{
    return vec3(((val >> 20) & uint(1023)) / 1023.f,
                ((val >> 10) & uint(1023)) / 1023.f,
                (val & uint(1023)) / 1023.f);
}