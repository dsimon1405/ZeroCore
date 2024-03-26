#version 460 core
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoords;

layout (std140, binding = 1) uniform Ortho { mat4 ortho; };
uniform vec2 unPositionWindow;

out vec2 vTexCoords;

void main()
{
    gl_Position = ortho * vec4(inPosition.x + unPositionWindow.x, inPosition.y + unPositionWindow.y, 0.0, 1.0);
    vTexCoords = inTexCoords;
}