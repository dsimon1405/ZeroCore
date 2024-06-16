
// // //                                      instancing + sso st430
// #version 460 core
// //  in
// layout (location = 0) in In
// {
//     flat uint drawID;
// };

// struct InData
// {
//     vec4 color;
//     mat4 model;
// };

// layout (std430, binding = 0) buffer InDataAll
// {
//     InData inData[];
// } inDataAll;

// //  out
// layout (location = 0) out vec4 FragColor;

// void main()
// {
//     FragColor = inDataAll.inData[drawID].color;
// };







// //                         //  shader pipeline + (tes evaluation shader + tes control from cpu (need make tes con shader))

// #version 460 core

// layout (location = 0) out vec4 FragColor;

// layout (location = 0) in In { vec3 color; } inD;

// void main()
// {
//     FragColor = vec4(inD.color, 1.f);
// };



//                                //	texture array
// #version 460 core

// layout (location = 0) in InF
// {
//     vec2 texCoord;
//     flat uint layer;
// } inF;

// layout (location = 2, binding = 11) uniform sampler2DArray texArray;

// out vec4 FragColor;

// float layer2coord(uint capacity, uint layer)
// {
// 	return max(0, min(float(capacity - 1), floor(float(layer) + 0.5)));
// }

// void main()
// {
// 	// FragColor = texture(texArray, texCoord);
// 	FragColor = texture(texArray, vec3(inF.texCoord, layer2coord(3, inF.layer)));
// };




// //                                 //  uses -> uniform block
// #version 460 core

// in vec4 vColor;

// out vec4 FragColor;

// void main()
// {
//     FragColor = vColor;
// };






// //                          ssbo withought vao
// #version 460 core

//     //  in
// layout (location = 0) in In
// {
//     vec2 texCoords;
// };

// layout (location = 0, binding = 0) uniform sampler2D sampTex;

//     //  out
// out vec4 FragColor;

// void main()
// {
//     FragColor = texture(sampTex, texCoords);
// }





//                          //      glMultiDrawElementsIndirect
#version 460 core

layout (location = 0) in inF
{
    vec2 uv;
};

layout (location = 0, binding = 0) uniform sampler2D texColor;

out vec4 FragColor;

void main()
{
    FragColor = texture(texColor, uv);
}
