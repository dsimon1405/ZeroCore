#version 460 core
    //  in
layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 norm;  //  packed in GL_INT_REV_2_10_10_10, last 2 bytes store object id : -1(sphere playable), 0(platform), 1(sphere map). Packed in G_ModelLoader::CreateDrawerSet()
layout(location = 2) in vec2 tex;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    mat4 perspViewSkybox;
    vec3 camPos;
};

        //  light
#define Light_Star 0
#define Light_Platform 1
struct Light
{       //  vec3 + uitn = vec4 alignment
    vec3 pos;
    uint color;
};
layout (std140, binding = 1) uniform UBOLight
{
    Light light[2];     //  first Star, seond Platform -> look G_LightUBO

    // float test_atten_linear;
    // float test_atten_quadratic;
    // float test_ambient;
    // float test_difuse;
    // float test_specular;
    // float test_spec_pow;
};

uniform mat4 unModel;
uniform uint unColor = 0;


    //  out
layout (location = 0) out OutV
{
    vec2 tex_coords;
    vec3 frag_pos;

    uint add_color_packed;
    vec3 add_color;

    vec3 normal;
    int object_id;
    float attenuation_linear;
    float attenuation_quadratic;

    vec3 light_color[2];

    // bool normal_aligned_to_cam;
} outV;


    //  methods
vec3 Uint_2_10_10_10_To_vec3(uint val);
// vec2 MoveByLengthXY(vec2 v, vec2 direction, float length);
// void CalculateLightPlatformPos();

void main()
{
    outV.tex_coords = tex;
    vec4 frag_pos_v4 = unModel * vec4(pos, 1.f);
    outV.frag_pos = vec3(frag_pos_v4);
    outV.normal = normalize(mat3(transpose(inverse(unModel))) * vec3(norm));
    
    gl_Position = perspView * frag_pos_v4;

    // outV.normal_aligned_to_cam = dot(normalize(outV.frag_pos - camPos), outV.normal) > 0.f;
    // // outV.normal_aligned_to_cam = false;                                                                         //  TEST MODE
    // if (outV.normal_aligned_to_cam) return;     //  cam look at the face from the back, will be discard in fs
    
        //  unpack adding color
    outV.add_color_packed = unColor;
    outV.add_color = vec3(0.f,0.f,0.f);
    outV.add_color = unColor != 0 ? Uint_2_10_10_10_To_vec3(unColor) : vec3(0.f, 0.f, 0.f);
        //  unpack light color Star
    outV.light_color[Light_Star] = light[Light_Star].color != 0 ? Uint_2_10_10_10_To_vec3(light[Light_Star].color) : vec3(0.f, 0.f, 0.f);

        //  attenuation
    outV.object_id = int(norm.w);
    float attenuation_linear_start = 0.f;
    float attenuation_quadratic_start = 0.f;
    switch (outV.object_id)    //  look upper -> layout(location = 1) in vec4 norm ...
    {
    case -1:    //  sphere playable
    {
        attenuation_linear_start = 0.014f;
        attenuation_quadratic_start = 0.00007f;
        if (light[Light_Platform].color != 0)
            outV.light_color[Light_Platform] = Uint_2_10_10_10_To_vec3(light[Light_Platform].color);
    } break;
    case 0:     //  platform
    {
        attenuation_linear_start = 0.0028f;
        attenuation_quadratic_start = 0.00002f;
    } break;
    case 1:     //  sphere map
    {
        attenuation_linear_start = 0.007f;
        attenuation_quadratic_start = 0.0002f;
    } break;
    default: break;
    }
    float distance = length(vec3(light[Light_Star].pos));
    float attenuation_dist_coef = floor(distance / 100.0);  //  coef for calculation linear and quadratic params, each section is pluss 100.f to map radius
    outV.attenuation_linear = attenuation_linear_start / (2.f * attenuation_dist_coef);  //  linear has progression with some near 2 divisor on each new section must be added 
    outV.attenuation_quadratic = attenuation_quadratic_start / (4.f * attenuation_dist_coef);    //  quadratic has progression with some near 4 divisor on each new section must be added 
}


vec3 Uint_2_10_10_10_To_vec3(uint val)
{
    return vec3((val >> 20) / 255.f,
                (val >> 10 & uint(1023)) / 255.f,
                (val & uint(1023)) / 255.f);
}

// vec2 MoveByLengthXY(vec2 v, vec2 direction, float length)
// {
//     return v + (direction * (length / sqrt(direction[0] * direction[0] + direction[1] * direction[1])));
// }

// void CalculateLightPlatformPos()
// {
//         //  calulate light platform pos for vertex
//     const float platf_radius_xy = 7.5f;
//     const float platf_half_height_z = 1.25f;

//     vec3 dir_platf_center_to_frag = outV.frag_pos - light[Light_Platform].pos;
//     outV.dir_platf_center_to_frag = dir_platf_center_to_frag;
//         //  calculate light pos xy
//     float platf_center_to_frag_length_xy = length(dir_platf_center_to_frag.xy);
//     outV.platf_center_to_frag_length_xy = platf_center_to_frag_length_xy;
//     vec2 light_platf_xy = platf_center_to_frag_length_xy <= platf_radius_xy ? light[Light_Platform].pos.xy      //  xy in platform radius
//         : MoveByLengthXY(light[Light_Platform].pos.xy, dir_platf_center_to_frag.xy, platf_radius_xy);   //  move platf_center to platf border by xy
//     outV.light_platf_xy = light_platf_xy;
//         //  calculate light pos z
//     float length_platf_center_to_frag_z = abs(dir_platf_center_to_frag.z);    //  dir could be negative, need length
//     outV.length_platf_center_to_frag_z = length_platf_center_to_frag_z;
//     float move_length_z = length_platf_center_to_frag_z > platf_half_height_z ? platf_half_height_z : length_platf_center_to_frag_z; //  light can't be ouut of platf_half_height_z
//     outV.move_length_z = move_length_z;
//     if (dir_platf_center_to_frag.z < 0.f) move_length_z *= -1.f;  //  take signe of the diraction
//     outV.move_length_z_signe = move_length_z;
//     float light_platf_z = light[Light_Platform].pos.z + move_length_z;

//     outV.light_pltform_pos = vec3(light_platf_xy, light_platf_z);

//     outV.light_pltform_pos.z = -1.f;
// }


// #version 460 core

// layout(location = 0) in vec3 pos;
// layout(location = 1) in vec3 norm;
// layout(location = 2) in vec2 tex;

// layout (std140, binding = 0) uniform Camera
// {
//     mat4 ortho;
//     mat4 perspView;
//     mat4 perspViewSkybox;
//     vec3 camPos;
// };

// layout (std140, binding = 1) uniform Light
// {
//     vec4 pos;
//     vec4 color;
// } light;

// uniform mat4 unModel;
// uniform uint unColor = 0;

//     //  out
// layout (location = 0) out OutV
// {
//     vec2 tex_coords;
//     uint add_color_packed;
//     vec3 add_color;
//     vec3 light_color_coef;   //  coef of light color
// } outV;

// void main()
// {
//     vec3 vert_pos = vec3(unModel * vec4(pos,1));
//     outV.tex_coords = tex;
//     gl_Position =  perspView * vec4(vert_pos, 1.);
//     //  * unModel * vec4(pos, 1);

//         //  unpack adding color 
//     outV.add_color_packed = unColor;
//     outV.add_color = vec3(0.,0.,0.);
//     if (unColor != 0)
//     {
//         outV.add_color.r = (unColor >> 20) / 255.f;
//         outV.add_color.g = (unColor >> 10 & uint(1023)) / 255.f;
//         outV.add_color.b = (unColor & uint(1023)) / 255.f;
//     }

//     //  calculate light color coef
//         // light distance attenuation coef
//     const float constant_c = 1.0;
//     const float linear_c = 0.0014;
//     const float quadratic_c = 0.00007;
//     float dist_vert_light = length(vec3(light.pos) - vert_pos);
//     float distance_attenuation_coef = 1.0 / (constant_c + linear_c * dist_vert_light + quadratic_c * (dist_vert_light * dist_vert_light));
//         //  light normal attenuation coef
//     // vec3 normal = normalize(mat3(unModel) * norm.xyz);   //  rotate and scale normals (wrong results for scaled object, but tolerable)
//     vec3 normal = mat3(transpose(inverse(unModel))) * norm;
//     vec3 dirPosToLight = normalize(light.pos.xyz - vert_pos);
//     float cos = dot(dirPosToLight, normal); //  range (-1,1)
//     float normal_attenuation_coef = (cos + 1.f) / 2.f;   //  from (-1,1) to (0,1)
//         //  light color coef
//     vec3 light_color_power_coef = vec3(light.color) + 1.0;
//     outV.light_color_coef = light_color_power_coef * normal_attenuation_coef * distance_attenuation_coef;
// }