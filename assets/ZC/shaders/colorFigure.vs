#version 460 core

layout (location = 0) in vec3 inPosition;
// layout (location = 1) in vec3 inColor;
layout (location = 2) in vec4 inNormal;
layout (std140, binding = 0) uniform PerspView { mat4 perspView; };
layout (std140, binding = 2) uniform CamPos { vec3 camPos; };
uniform mat4 unModel;

out vec3 vColor;

void main()
{
    vec4 position = unModel * vec4(inPosition, 1.f);
    gl_Position = perspView * position;
    
    vec3 normal = mat3(unModel) * inNormal.xyz;   //  rotation of normals (wrong way if the model matrix is ​​scaled with different factors for different axes)
    vec3 dirPosToCam = normalize(camPos - position.xyz);
    float cos = dot(dirPosToCam, normal); //  range -1 to 1

    // float coef = max(0.f, cos);     //  avoid negative range (black color if angle less then 90 degree)
    float coef = (cos + 1.f) / 2.f;   //  from (-1,1) to (0,1)

    // vColor = inColor * coef;
    vColor = vec3(0.5f, 0.5f, 0.5f) * coef;
}