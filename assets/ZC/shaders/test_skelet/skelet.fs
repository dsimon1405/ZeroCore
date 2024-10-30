#version 460 core

out vec4 FragColor;

// in vec2 texCoords;
in vec4 v_color;
flat in ivec4 temp_boneIds;
in vec4 temp_boeWights;
in mat4 temp_mat;

// uniform sampler2D texColor;

void main()
{    
    // FragColor = texture(texColor, texCoords);
    FragColor = v_color;
}
