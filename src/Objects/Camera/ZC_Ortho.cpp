#include <ZC/Objects/Camera/ZC_Ortho.h>

#include <ZC/Tools/Math/ZC_Mat.h>

ZC_Ortho::ZC_Ortho(float _startX, float _endX, float _startY, float _endY)
    : startX(_startX),
    endX(_endX),
    startY(_startY),
    endY(_endY)
{}

void ZC_Ortho::SetStartX(float _startX)
{
    if (startX == _startX) return;
    startX = _startX;
    orthoNeedUpdate = true;
    isOrthoActual = false;
}

void ZC_Ortho::SetEndX(float _endX)
{
    if (endX == _endX) return;
    endX = _endX;
    orthoNeedUpdate = true;
    isOrthoActual = false;
}

void ZC_Ortho::SetStartY(float _startY)
{
    if (startY == _startY) return;
    startY = _startY;
    orthoNeedUpdate = true;
    isOrthoActual = false;
}

void ZC_Ortho::SetEndY(float _endY)
{
    if (endY == _endY) return;
    endY = _endY;
    orthoNeedUpdate = true;
    isOrthoActual = false;
}

float ZC_Ortho::GetStartX() const noexcept
{
    return startX;
}

float ZC_Ortho::GetEndX() const noexcept
{
    return endX;
}

float ZC_Ortho::GetStartY() const noexcept
{
    return startY;
}

float ZC_Ortho::GetEndY() const noexcept
{
    return endY;
}

const ZC_Mat4<float>* ZC_Ortho::GetOrtho()
{
    ActualizeOrthoMatrix();
    return pOrtho;
}

bool ZC_Ortho::OrthoUpdate()
{
    if (!pOrtho || !orthoNeedUpdate) return false;

    orthoNeedUpdate = false;
    ActualizeOrthoMatrix();
    return true;
}

void ZC_Ortho::ActualizeOrthoMatrix()
{
    if (pOrtho && isOrthoActual) return;
    
    isOrthoActual = true;
    *pOrtho = ZC_Mat::Ortho(startX, endX, startY, endY);
}
