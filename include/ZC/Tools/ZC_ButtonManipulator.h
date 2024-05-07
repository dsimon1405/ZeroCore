#pragma once

#include <forward_list>
#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/Events/ZC_EC.h>

/*
Class for controling button events. Heir must override -> void VCallButtonDownBM(ZC_ButtonID buttonId, float time);. In wich must be something like switch with
reactions on different buttons calls. For activation that fucntion uses button from _activateButtonId (and for deactivation if _use_activateButtonId_ForDeactivation = true),
for deactivation in each case uses "K_ESCAPE". Heir may have ladder type (windows that may be opened/closed, but active may be only one heir at a time) or not (something like
window opened under ladder windows, such windows can be several, and you can switch between that windows for activation, but they can't be active until open ladder window(s)).
VCallButtonDownBM() calls first, for ladder type heirs, until they are deactivate. One of the heirs, MUST be floor type (isFloor = true), it something like desktop,
he can't be deactivated, became active if no nore active heirs. In overriden VCallButtonDownBM(...) can't be used mouse buttons: M_LEFT, M_MIDLE, M_RIGHT, M_X1, M_X2!
*/
class ZC_ButtonManipulator
{
public:
    const ZC_ButtonID activateButtonId;
    const bool isLadder,
        use_activateButtonId_ForDeactivation;

    /*
    Params:
    - _activateButtonId - button for activation.
    - _isLadder - indicates whether the heir is a ladder type or not (can't be ladder and floor at the same time).
    - _isFloor - indicates whether the heir is a floor type or not, can be only one heir (can't be ladder and floor at the same time).
    - _use_activateButtonId_ForDeactivation - use _activateButtonId for deactivation or not (no metter, if isFloor = false).
    */
    ZC_ButtonManipulator(ZC_ButtonID _activateButtonId, bool _isLadder, bool isFloor, bool _use_activateButtonId_ForDeactivation);

    // ZC_ButtonManipulator(ZC_ButtonManipulator&& bm);

    ~ZC_ButtonManipulator();

    bool operator == (ZC_ButtonID buttonId);

    void ActivateBM();
    void DeactivateBM();
    bool IsFloorBM() const noexcept;
    //  Returns true if current object target for call buttons events, otherwise false. If return true, it doesn't mean that call of button event happening right now!
    bool IsActiveBM() const noexcept;

private:
    bool isActive = false;

    virtual void VCallButtonDownBM(ZC_ButtonID buttonId, float time) = 0;  //  here heir must realize switch for all necessary fucntions
    virtual void VActivateBM() {}      //  if need some activation on activate
    virtual void VDeactivateBM() {}    //  if need some deactivation on deactivate

    static inline std::forward_list<ZC_ButtonManipulator*> ladderBM;
    static inline ZC_ButtonManipulator* pFloorBM = nullptr;
    static inline ZC_ButtonManipulator* pActiveBM = nullptr;
    static inline ZC_EC ecFistDownButton;

    static void FirstButtonDown(ZC_ButtonID buttonId, float time);
};