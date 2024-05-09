#include <ZC_IGWBM.h>

ZC_IGWBM::ZC_IGWBM(ZC_ButtonID _activateButtonId, bool _isStaircase, bool isFloor, bool _use_activateButtonId_ForDeactivation, std::string&& unicName,
        bool needDraw, float _width, float _height, float _indentX, float _indentY, ZC_WindowOrthoIndentFlags _indentFlags, bool _mayClose, int _igwf)
    : ZC_IGWindow(std::move(unicName), needDraw, _width, _height, _indentX, _indentY, _indentFlags, _mayClose, _igwf),
    ZC_ButtonManipulator(_activateButtonId, _isStaircase, isFloor, _use_activateButtonId_ForDeactivation)
{
    if (needDraw) this->ActivateBM();
}

bool ZC_IGWBM::IsEventsTargetIGWBM() const noexcept
{
    return this->IsEventsTargetBM();
}

bool ZC_IGWBM::IsActiveIGWBM() const noexcept
{
    return this->IsActiveBM();
}

void ZC_IGWBM::VActivateBM()
{
    this->NeedDrawIGW(true);   //  if pFloorBM became active in ZC_ButtonManipulator::DeactivateBM(), not throught ActivateIGWBM() that can change his draw state
    VActivateIGWBM();
}

void ZC_IGWBM::VDeactivateBM()
{
    this->NeedDrawIGW((this->IsFloorBM()));    //  stop drawing every heir among ZC_ButtonManipulator::pFloorBM level
    VDeactivateIGWBM();
}

void ZC_IGWBM::VFocusStateChangedIGW(bool _isFocused)
{
    if (_isFocused && !this->IsFloorBM()) this->ActivateBM();
    else if (!this->IsDrawingIGW()) this->DeactivateBM();  //  if changing draw state in ZC_IGWWindow (the window was covered with a cross)
}