#include <ZC/Tools/ZC_OrthoSquare.h>

void ZC_OrthoSquare::SetSize(float _width, float _height)
{
    if (!this->SetNewSize(_width, _height)) return;     //  if false, size same

    float vertices[]
    {
                                //  bl - without changing
        _width, 0.f,         //  br
        _width, _height,     //  tr
        0.f,    _height,     //  tl
    };
                                //  8 -> is size of bl wich don't change
    renderSet.buffers.begin()->BufferSubData(8, sizeof(vertices), vertices);
}

void ZC_OrthoSquare::SetAlpha(float alpha)
{
    alpha = alpha < 0.f ? 0.f
        : alpha > 1.f ? 1.f
        : alpha;
    rsController.SetUniformsData(ZC_UN_unAlpha, &alpha);
}

void ZC_OrthoSquare::NeedDraw(bool needDraw)
{
    needDraw ? rsController.SwitchToDrawLvl(frameBuffer, ZC_DrawLevels::OrthoBlend) : rsController.SwitchToDrawLvl(frameBuffer, ZC_DL_None);
}

bool ZC_OrthoSquare::IsDrawing()
{
    return rsController.IsDrawing(frameBuffer);
}

ZC_RenderSet ZC_OrthoSquare::CreateRenderSet(float _width, float _height)
{
    float vertices[]
    {
        0.f,    0.f,         //  bl
        _width, 0.f,         //  br
        _width, _height,     //  tr
        0.f,    _height,     //  tl
    };

    ushort packedOne = ZC_PackTexCoordFloatToUShort(1.f);
    ushort texCoords[]
    {
        0, 0,                   //  bl
        packedOne, 0,           //  br
        packedOne, packedOne,   //  tr
        0, packedOne,           //  tl
    };

    ZC_Buffer vbo(GL_ARRAY_BUFFER);
    vbo.BufferData(sizeof(vertices) + sizeof(texCoords), vertices, GL_DYNAMIC_DRAW);
    vbo.BufferSubData(sizeof(vertices), sizeof(texCoords), texCoords);

    auto upGLDraw = ZC_uptrMakeFromChild<ZC_GLDraw, ZC_DrawArrays>(GL_TRIANGLE_FAN, 0, 4);

    auto pShPIS = ZC_ShProgs::Get(ShPN_ZC_OrthoTexture);

    ZC_VAO vao;
    vao.Config(pShPIS->vaoConfigData, vbo, nullptr, 0, 4);
    
    std::forward_list<ZC_Buffer> buffers;
    buffers.emplace_front(std::move(vbo));

    return ZC_RenderSet(pShPIS, std::move(vao), std::move(upGLDraw), std::move(buffers), {}, { { frameBuffer } });
}

void ZC_OrthoSquare::VCallAfterZC_WindowResized()
{
    if (funcIndentChange) funcIndentChange(this->currentIndents[0], this->currentIndents[1]);
}
