#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;


layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    mat4 perspViewSkybox;
    vec3 camPos;
};

void main()
{
    TexCoords = aPos;
    
    gl_Position = perspView * vec4(aPos, 1.0);
    // gl_Position = perspViewSkybox * vec4(aPos, 1.0);

// gl_Position = mat4(mat3(perspView)) * vec4(aPos,1);
    
    // vec4 pos = perspViewSkybox * vec4(aPos, 1.0);
    // gl_Position = pos.xyww;
}  