#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

//  in
layout (location = 0) in InG
{
    int baseInstance;   //  total offset in: bls, objDatas (uses in glMutiDrawArraysIndirect, sets in it's command struct, by default is 0 for glDrawArrays)
    int vertexID;       //  relative offset in: bls, objDatas
} inG[];

struct ZC_GUI_Border
{
    vec2 bl;     //  coord of frame border, to avoid drawing objects out of window
    vec2 tr;
};
layout (std430, binding = 0) readonly buffer InBorder { ZC_GUI_Border borders[]; } inBorder;

layout (std430, binding = 1) readonly buffer InVertex { vec2 bls[]; } inBL;

struct ZC_GUI_ObjData 
{
    float width;
    float height;
    float depth;    //  first element is window and it's start window depth, each other object must calculate own depth from that depth
    uint color;
    float uv[4];    //  bl, tr
    int borderIndex;
    uint tex_binding;
};
layout (std430, binding = 2) readonly buffer InObjData { ZC_GUI_ObjData objDatas[]; } inObjData;

layout (std140, binding = 0) uniform Camera
{
    mat4 ortho;
    mat4 perspView;
    vec3 camPos;
};

//  out
layout (location = 0) out OutG
{
    int borderIndex;    //  if -1, don't need border check
    uint tex_binding;
    vec2 uv;
    uint color;
} outG;


float ToZeroOneRange(float val)     //  [-1,1] -> [0,1]
{
    return (val + 1.0) / 2.0;
}

bool Discard()
{
    // outG.needDiscard = true;
    // gl_Position = vec4(0, 0, 0, 1);
    EmitVertex();
    EndPrimitive();
    return true;    //  useless return value, uses for return in one line from if() calls
}

//  return false if out of border (need discard)
bool ConfigureBorder(int borderIndex, vec2 bl, vec2 br, vec2 tl, vec2 tr)
{
    ZC_GUI_Border border = inBorder.borders[borderIndex];
    bool corner1 = false;
    bool corner2 = false;
    bool corner3 = false;
    bool corner4 = false;
    if (border.bl.x <= bl.x && border.bl.y <= bl.y && border.tr.x >= bl.x && border.tr.y >= bl.y) corner1 = true;
    if (border.bl.x <= br.x && border.bl.y <= br.y && border.tr.x >= br.x && border.tr.y >= br.y) corner2 = true;
    if (border.bl.x <= tl.x && border.bl.y <= tl.y && border.tr.x >= tl.x && border.tr.y >= tl.y) corner3 = true;
    if (border.bl.x <= tr.x && border.bl.y <= tr.y && border.tr.x >= tr.x && border.tr.y >= tr.y) corner4 = true;

    // if (!corner1 && !corner2 && !corner3 && !corner4) return false;     //  out of border (discard)
        //  if all object's corners in border, don't need border check in fragment shader, set -1. Otherwise set border index
    outG.borderIndex = corner1 && corner2 && corner3 && corner4 ? -1 : borderIndex;
    return true;
}

void SetVertex(float u, float v, vec2 pos, float depth)
{
    outG.uv = vec2(u, v);
    gl_Position = ortho * vec4(pos, 0, 1);
    gl_Position.z = depth;
    EmitVertex();
}

void main()
{
    // // Data data = inD.data[gl_PrimitiveIDIn];
    // // vec4 startPos = gl_in[gl_PrimitiveIDIn].gl_Position;

    int baseInstance = inG[gl_PrimitiveIDIn].baseInstance;
    int vertexID = inG[gl_PrimitiveIDIn].vertexID;

    int vertIndex = baseInstance + vertexID;

    ZC_GUI_ObjData objData = inObjData.objDatas[vertIndex];
    if (objData.height == 0 && Discard()) return;   //  object not drawing
        //  border
    ZC_GUI_Border border = inBorder.borders[objData.borderIndex];
    if (border.bl.x == border.tr.x && Discard()) return;      //  not valid border (border in the window, but now outside the window border) discard element

    vec2 bl = inBL.bls[vertIndex];
    vec2 br = vec2(bl.x + objData.width, bl.y);
    vec2 tl = vec2(bl.x, bl.y + objData.height);
    vec2 tr = vec2(br.x, tl.y);

    if (!ConfigureBorder(objData.borderIndex, bl, br, tl, tr) && Discard()) return;    //  out of border
        //  depth
    float depth = objData.depth;
    if (vertexID != 0) depth += inObjData.objDatas[baseInstance].depth;  //  object not window border, need add window's border depth
    
        //  texture index
    outG.tex_binding = objData.tex_binding;
    outG.color = objData.color;

        //  vertices
    SetVertex(objData.uv[0], objData.uv[1], bl, depth);
    SetVertex(objData.uv[2], objData.uv[1], br, depth);
    SetVertex(objData.uv[0], objData.uv[3], tl, depth);
    SetVertex(objData.uv[2], objData.uv[3], tr, depth);
    EndPrimitive();
}
