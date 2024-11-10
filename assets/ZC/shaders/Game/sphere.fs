#version 460 core

    //  in
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

uniform sampler2D texColor;
uniform float unAlpha = 1.f;

layout (location = 0) in InF
{
    vec2 tex_coords;
    vec3 frag_pos;

    flat uint add_color_packed;
    vec3 add_color;

    vec3 normal;
    flat int object_id;
    float attenuation_linear;
    float attenuation_quadratic;

    vec3 light_color[2];

    flat bool normal_aligned_to_cam;
} inF;


    //  out
out vec4 FragColor;


    //  methods
vec3 Calculate_dir_frag_to_cam(bool use_specular);
float CalculateColor(float start, float end, float growing_coef);
vec3 InterpolateColor(vec3 start_color, vec3 end_color, float growing_coef);
float CalcAttenuationLinear(vec3 light_pos);
float CalcAttenuationRealistic(vec3 light_pos, float attenuation_linear, float attenuation_quadratic);
vec3 CalculateCombineFragColor(vec3 frag_color, vec3 light_pos, vec3 light_color, bool interpolate_light_color, float attenuation,
    float ambient_power, float diffuse_power, float specular_power, bool use_specular, vec3 dir_frag_to_cam, float specular_pow);


// float TestAttenuationLinear(vec3 light_pos)
// {
//     const float light_range = ;
//     const float light_start_offset = 3.f;   //  for platforms light pos further, then lightning must be start (to improve diffuse effect). Loock G_Platform::CalculateLightPos()
//     float dist_frag_to_light = length(light_pos - inF.frag_pos);
//     return 1.f - (dist_frag_to_light / light_range);
// }

#define SPECULAR_POW 16.f

void main()
{       //  if inF.normal_aligned_to_cam true -> cam look at the face from the back
    if (inF.normal_aligned_to_cam || unAlpha == 0.f) discard;

    vec3 frag_color = texture(texColor, inF.tex_coords).rgb;
    if (frag_color == vec3(0.f, 0.f, 0.f))
    {
        FragColor = vec4(0.f, 0.f, 0.f, unAlpha);
        return;
    }



        //  TEST MODE
FragColor = vec4(frag_color, unAlpha);
return;





    switch (inF.object_id)    //  look -> layout(location = 1) in vec4 norm ...
    {
    case -1:    //  sphere playable
    {
        const float floor_color = 0.7f;
        if (inF.add_color_packed != 0 && frag_color.x < floor_color && frag_color.y < floor_color && frag_color.z < floor_color)
            FragColor = vec4(inF.add_color, 1.f);
        else
        {
            const float floor_color_specular = 0.9f;
            bool use_specular = frag_color.x < floor_color_specular && frag_color.y < floor_color_specular && frag_color.z < floor_color_specular;
            vec3 dir_frag_to_cam = Calculate_dir_frag_to_cam(use_specular);
            vec3 frag_colored = CalculateCombineFragColor(frag_color, light[Light_Star].pos, inF.light_color[Light_Star], true, 
                CalcAttenuationRealistic(light[Light_Star].pos, inF.attenuation_linear, inF.attenuation_quadratic),
                0.2f, 0.5f, 2.f, use_specular, dir_frag_to_cam, SPECULAR_POW);
            if (light[Light_Platform].color != 0) frag_colored += CalculateCombineFragColor(frag_color, light[Light_Platform].pos,
                inF.light_color[Light_Platform], false, CalcAttenuationLinear(light[Light_Platform].pos),
                0.04f, 0.7f, 1.2f, use_specular, dir_frag_to_cam, 8.f);
            FragColor = vec4(frag_colored, unAlpha);
        }
    } break;
    case 0:     //  platform
    {
        const float floor_color = 0.1f;
        bool use_specular = frag_color.x > floor_color && frag_color.y > floor_color && frag_color.z > floor_color;
        vec3 frag_colored = CalculateCombineFragColor(frag_color, light[Light_Star].pos, inF.light_color[Light_Star], true,
            CalcAttenuationRealistic(light[Light_Star].pos, inF.attenuation_linear, inF.attenuation_quadratic),
            0.2f, 0.5f, 2.f, use_specular, Calculate_dir_frag_to_cam(use_specular), SPECULAR_POW);
        if (use_specular) frag_colored += inF.add_color;
        FragColor = vec4(frag_colored, unAlpha);
    } break;
    case 1:     //  sphere map
    {
        FragColor = vec4(CalculateCombineFragColor(frag_color, light[Light_Star].pos, inF.light_color[Light_Star], true,
            CalcAttenuationRealistic(light[Light_Star].pos, inF.attenuation_linear, inF.attenuation_quadratic),
            0.3f, 0.5f, 2.f, true, Calculate_dir_frag_to_cam(true), SPECULAR_POW), unAlpha);
    } break;
    default: break;
    }
}


vec3 Calculate_dir_frag_to_cam(bool use_specular)
{
    return use_specular ? normalize(camPos - inF.frag_pos) : vec3(0.f, 0.f, 0.f);
}

float CalculateColor(float start, float end, float growing_coef)
{
    return start > end ? start - ((start - end) * growing_coef) : start + ((end - start) * growing_coef);
};

vec3 InterpolateColor(vec3 start_color, vec3 end_color, float growing_coef)
{
    return vec3(CalculateColor(start_color.x, end_color.x, growing_coef),
        CalculateColor(start_color.y, end_color.y, growing_coef),
        CalculateColor(start_color.z, end_color.z, growing_coef));
}

float CalcAttenuationLinear(vec3 light_pos)
{
    const float light_platform_range = 10.f;
    const float light_start_offset = 3.f;   //  for platforms light pos further, then lightning must be start (to improve diffuse effect). Loock G_Platform::CalculateLightPos()
    float dist_frag_to_light = length(light_pos - inF.frag_pos);
    return 1.f - ((dist_frag_to_light - light_start_offset) / light_platform_range);
}

float CalcAttenuationRealistic(vec3 light_pos, float attenuation_linear, float attenuation_quadratic)
{
    const float attenuation_constant = 1.f;
    float dist_frag_to_light = length(light_pos - inF.frag_pos);
    return 1.f / (attenuation_constant + attenuation_linear * dist_frag_to_light + attenuation_quadratic * (dist_frag_to_light * dist_frag_to_light));
}

vec3 CalculateCombineFragColor(vec3 frag_color, vec3 light_pos, vec3 light_color, bool interpolate_light_color, float attenuation,
    float ambient_power, float diffuse_power, float specular_power, bool use_specular, vec3 dir_frag_to_cam, float specular_pow)
{
    if (attenuation < 0.f) return vec3(0.f, 0.f, 0.f);
        //  interpolate light color to white with attenuation coef
    if (interpolate_light_color) light_color = InterpolateColor(vec3(0.8f, 0.8f, 0.8f), light_color, attenuation);

        // ambient
    vec3 ambient = ambient_power * frag_color * light_color * attenuation;
        // diffuse
    vec3 dir_frag_to_light = normalize(light_pos - inF.frag_pos);
    float diffuse_coef = max(dot(inF.normal, dir_frag_to_light), 0.f);
    vec3 diffuse = diffuse_power * diffuse_coef * frag_color * light_color * attenuation;
        // specular
    if (use_specular)
    {
        vec3 reflectDir = reflect(-dir_frag_to_light, inF.normal);  
        float specular_coef = pow(max(dot(dir_frag_to_cam, reflectDir), 0.f), specular_pow);
        vec3 specular = specular_power * specular_coef * frag_color * light_color * attenuation;
        return ambient + diffuse + specular;
    }
    else return ambient + diffuse;
}

// #version 460 core

// out vec4 FragColor;

// uniform sampler2D texColor;

//     //  in
// layout (location = 0) in InF
// {
//     vec2 tex_coords;
//     flat uint add_color_packed;
//     vec3 add_color;
//     vec3 light_color_coef;   //  coef of light color
// } inF;

// void main()
// {
//     vec3 color = texture(texColor, inF.tex_coords).xyz;
//     const float floor_color = 0.7;
//     FragColor = inF.add_color_packed != 0 && color.x < floor_color && color.y < floor_color && color.z < floor_color ?
//         vec4(inF.add_color, 1.0)
//         : vec4(color * inF.light_color_coef, 1.0);
// }
