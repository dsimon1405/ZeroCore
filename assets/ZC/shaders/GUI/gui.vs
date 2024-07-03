#version 460 core

    //  out
layout (location = 0) out OutV
{
    int baseInstance;   //  total offset in: bls, objDatas (uses in glMutiDrawArraysIndirect, sets in it's command struct, by default is 0 for glDrawArrays)
    int vertexID;       //  relative offset in: bls, objDatas
    
} outV;

void main()
{
    outV.baseInstance = gl_BaseInstance;
    outV.vertexID = gl_VertexID;
    gl_Position = vec4(0,0,0,0);
}