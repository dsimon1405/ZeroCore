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
        //  colors
    float red;
    float green;
    float blue;
    float alpha;     //  alpha sets only in color pack uint32 -> 8x8x8x8 in ZC_GUI_ColorManipulator
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

void SetVertex(vec2 pos, float depth)
{
    gl_Position = ortho * vec4(pos, 0, 1);
    gl_Position.z = depth;
    EmitVertex();
}

void SetVertex(float u, float v, vec2 pos, float depth)
{
    outG.uv = vec2(u, v);
    SetVertex(pos, depth);
}

void UnpackAndSetColor(uint color)
{
    if (color != 0)
    {
        outG.red = (color >> 20) / 255.f;
        outG.green = (color >> 10 & uint(1023)) / 255.f;
        outG.blue = (color & uint(1023)) / 255.f;
    }
    else
    {
        outG.red = 0.f;
        outG.green = 0.f;
        outG.blue = 0.f;
    }
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
    if (objData.borderIndex != -1)  // if -1 it's window's frame (don't have border)
    {
        ZC_GUI_Border border = inBorder.borders[objData.borderIndex];
        if (border.bl.x == border.tr.x && Discard()) return;      //  not valid border (border in the window, but now outside the window border) discard element
    }

    vec2 bl = inBL.bls[vertIndex];
    vec2 br = vec2(bl.x + objData.width, bl.y);
    vec2 tl = vec2(bl.x, bl.y + objData.height);
    vec2 tr = vec2(br.x, tl.y);

    if (vertexID == 0 || objData.borderIndex == -1) outG.borderIndex = -1;   //  window background may be with frame, so don't check border for window's background and frame, just draw
    else if (!ConfigureBorder(objData.borderIndex, bl, br, tl, tr) && Discard()) return;    //  out of border
        //  depth
    float depth = objData.depth != 0 ? objData.depth : inObjData.objDatas[baseInstance].depth;      //  if not equal 0 it's window wit hown depth, otherwise it object withought own depth, set window depth
    // if (depth == 0) depth = inObjData.objDatas[baseInstance].depth;  //  object not window, need add window's depth
    // if (vertexID != 0) depth += inObjData.objDatas[baseInstance].depth;
    
        //  texture index
    outG.tex_binding = objData.tex_binding;

    switch (outG.tex_binding)    //  classification in ZC_GUI_Bindings.h
    {
    case 10:     //  result triangle, don't have texture, just a color RGBA packed uint32 -> 8x8x8x8 (look ZC_GUI_ColorManipulator)
    {
        if (objData.color != 0)
        {
            outG.red = (objData.color >> 24 & uint(255)) / 255.f;
            outG.green = (objData.color >> 16 & uint(255)) / 255.f;
            outG.blue = (objData.color >> 8 & uint(255)) / 255.f;
            outG.alpha = (objData.color & uint(255)) / 255.f;
        }
        else
        {
            outG.red = 0.f;
            outG.green = 0.f;
            outG.blue = 0.f;
            outG.alpha = 0.f;
        }
            //  vertices
        SetVertex(bl, depth);
        SetVertex(br, depth);
        SetVertex(tl, depth);
    } break;
    case 11:     //  saturation triangle, don't have texture, just a color RGB packed uint32 -> 2x10x10x10 (look ZC_GUI_ColorManipulator)
    {       //  set black color
        outG.red = 0.f;
        outG.green = 0.f;
        outG.blue = 0.f;
        SetVertex(br, depth);   //  uses black color
            //  set white color
        outG.red = 1.f;
        outG.green = 1.f;
        outG.blue = 1.f;
        SetVertex(tl, depth);   //  uses white color
            //  set saturatuion color
        UnpackAndSetColor(objData.color);
        SetVertex(tr, depth);   //  uses saturation color
    } break;
    case 12:    //  alpha triangle, have texture
    {
        UnpackAndSetColor(objData.color);
        SetVertex(objData.uv[0], objData.uv[1], bl, depth);
        SetVertex(objData.uv[2], objData.uv[1], br, depth);
        SetVertex(objData.uv[0], objData.uv[3], tl, depth);
    }
    default:    //  uses texture, color RGB packed uint32 -> 2x10x10x10 (2 unused), that color adds to texture, if it is
    {
        UnpackAndSetColor(objData.color);
            //  vertices
        SetVertex(objData.uv[0], objData.uv[1], bl, depth);
        SetVertex(objData.uv[2], objData.uv[1], br, depth);
        SetVertex(objData.uv[0], objData.uv[3], tl, depth);
        SetVertex(objData.uv[2], objData.uv[3], tr, depth);
    } break;
    }
    EndPrimitive();
}
