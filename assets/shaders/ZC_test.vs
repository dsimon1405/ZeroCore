layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

layout (std140, binding = 0) uniform Persp { mat4 perspView; };
layout (std140, binding = 1) uniform Ortho { mat4 ortho; };
uniform mat4 model;

out vec3 vColor;

void main()
{
    gl_Position = perspView * model * vec4(inPosition, 1.0);
    vColor = inColor;
}