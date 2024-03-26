// #version 460 core

// layout (location = 0) in vec2 inPosition;    on cpu 2 float
// layout (location = 0) in vec3 inPosition;    on cpu 3 float
// layout (location = 1) in vec3 inColor;       on cpu 3 uchar
// layout (location = 2) in vec4 inNormal;      on cpu 1 GL_INT_2_10_10_10_REV
// layout (location = 3) in vec2 inTexCoords;   on cpu 2 ushort

// layout (std140, binding = 0) uniform PerspView { mat4 perspView; };
// layout (std140, binding = 1) uniform Ortho { mat4 ortho; };

// uniform mat4 unModel;
// uniform uint unColor;  stencil.vs
// uniform uint unPosition;  textWindow.vs

// uniform sampler2D texColor;