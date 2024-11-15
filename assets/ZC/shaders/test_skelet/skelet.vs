#version 460 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in ivec4 inBoneIds;
layout(location = 4) in vec4 inBoneWeights;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspective;
    mat4 perspView;
    mat4 perspViewSkybox;
    mat4 view;
    vec3 camPos;
};

uniform mat4 unModel;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 unFinalBonesMatrices[MAX_BONES];

out vec4 v_color;
flat out ivec4 temp_boneIds;
out vec4 temp_boeWights;
out mat4 temp_mat;

void main()
{
    vec4 totalPosition = vec4(0.f);
    vec3 totalNormal = vec3(0.f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(inBoneIds[i] < 0) break;
        if(inBoneIds[i] >= MAX_BONES) 
        {
            totalPosition = vec4(inPosition,1.0f);
            break;
        }
        totalPosition += unFinalBonesMatrices[inBoneIds[i]] * vec4(inPosition,1.0f) * inBoneWeights[i];
        totalNormal += mat3(unFinalBonesMatrices[inBoneIds[i]]) * inNormal * inBoneWeights[i];
        // vec3 localNormal = mat3(unFinalBonesMatrices[boneIds[i]]) * norm;
    }
    gl_Position = perspView * unModel * totalPosition;
temp_boneIds = inBoneIds;
temp_boeWights = inBoneWeights;
temp_mat = unFinalBonesMatrices[1];
    // gl_Position = perspView * unModel * vec4(inPosition, 1.f);

	// texCoords = tex;

    vec3 normal = normalize(mat3(unModel) * totalNormal);   //  rotate and scale normals (wrong results for scaled object, but tolerable)
    // vec3 normal = normalize(mat3(unModel) * inNormal.xyz);   //  rotate and scale normals (wrong results for scaled object, but tolerable)
    vec3 dirPosToCam = normalize(camPos - inPosition.xyz);
    float cos = dot(dirPosToCam, normal); //  range (-1,1)
    float coef = (cos + 1.f) / 2.f;   //  from (-1,1) to (0,1)
    vec3 colorGrey = vec3(0.5f, 0.5f, 0.5f);
    v_color = vec4(colorGrey * coef, 1.f);
}
