#version 460 core
layout (location = 0) in vec4 inPosition; 	// <vec2 pos, vec2 tex>

layout (std140, binding = 1) uniform Ortho { mat4 ortho; };

out vec2 vTexCoords;

void main()
{
    gl_Position = ortho * vec4(inPosition.xy, 0.0, 1.0);
    vTexCoords = inPosition.zw;
}  