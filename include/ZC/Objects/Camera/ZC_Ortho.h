#pragma once

#include <ZC/Tools/Math/ZC_Mat4.h>

class ZC_Ortho
{
public:
    ZC_Ortho(float _startX, float _endX, float _startY, float _endY);
    virtual ~ZC_Ortho() = default;

    //  left
    void SetStartX(float _startX);
    //  right
    void SetEndX(float _endX);
    //  bottom
    void SetStartY(float _startY);
    //  top
    void SetEndY(float _endY);
    float GetStartX() const noexcept;
    float GetEndX() const noexcept;
    float GetStartY() const noexcept;
    float GetEndY() const noexcept;
    const ZC_Mat4<float>* GetOrtho();

protected:
    ZC_Mat4<float>* pOrtho;

    //  returns true if where updated.
    bool OrthoUpdate();

private:
    float startX,
        endX,
        startY,
        endY;
    bool orthoNeedUpdate = true,
        isOrthoActual = false;

    void ActualizeOrthoMatrix();
};