#include <ZC/Tools/ZC_ButtonManipulator.h>

#include <ZC/Events/ZC_Events.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <cassert>

ZC_ButtonManipulator::ZC_ButtonManipulator(ZC_ButtonID _activateButtonId, bool _isLadder, bool isFloor, bool _use_activateButtonId_ForDeactivation)
    : activateButtonId(isFloor ? K_UNKNOWN : _activateButtonId),
    isLadder(_isLadder),
    use_activateButtonId_ForDeactivation(isFloor ? false : _use_activateButtonId_ForDeactivation)
{
    // assert(!ZC_FindInPointers(allHeirs, activateButtonId));      //  allready existst ZC_ButtonManipulator with such activateButtonId
    assert(!(isLadder && isFloor));     //  can't be floor and isFloor at the same time
    assert(!(pFloorBM && isFloor));     //  can be only one floor

    if (!ecFistDownButton.IsConnected()) ecFistDownButton.NewConnection(ZC_Events::ConnectFirstDownButton({ &ZC_ButtonManipulator::FirstButtonDown }));
    if (isFloor)
    {
        pFloorBM = this;
        if (!pActiveBM)
        {
            isActive = true;
            pActiveBM = this;
            VActivateBM();
        }
    }
}

// ZC_ButtonManipulator(ZC_ButtonManipulator&& bm)
//     : isActive(bm.isActive),
//     activateButtonId(bm.activateButtonId),
//     isLadder(bm.isLadder),
//     use_activateButtonId_ForDeactivation(bm.use_activateButtonId_ForDeactivation)
// {
//     bm.isActive = false;    //  to not calls in destructor
//     if (isActive)   //  stores state of previous object
//     {
//         if (isLadder)
//         {
//             auto pPrevObjectPlace = ZC_Find(staircaseBM, &bm);
//             if (pPrevObjectPlace) *pPrevObjectPlace = this;  //  take previous object place, if place was
//         }
//         else if (pActiveBM == &bm) pActiveBM = this;    //  if previous object was not staircase and was active, make active current object
//     }
//     if (pFloorBM == &bm) pFloorBM = this;   //  if previous object were floor level, now set current object to floor level
//     allHeirs.emplace_front(this);
// }

ZC_ButtonManipulator::~ZC_ButtonManipulator()
{
    if (isActive)
    {
        if (isLadder) ZC_ForwardListErase(ladderBM, this);
        else pActiveBM = nullptr;
    }
    if (pFloorBM == this) pFloorBM = nullptr;
}

bool ZC_ButtonManipulator::operator == (ZC_ButtonID buttonId)
{
    return buttonId == activateButtonId;
}

void ZC_ButtonManipulator::ActivateBM()
{
    if (isActive)
    {   //  if allready active and it staircase, maby where active another object (more likely another window(s) was upper then that current window),
        //  so now makes current object (window) upper then all others
        if (isLadder)
        {
            ZC_ForwardListErase(ladderBM, this);
            ladderBM.emplace_front(this);
        }
    }
    else
    {
        isActive = true;
        if (isLadder) ladderBM.emplace_front(this);
        else
        {
            pActiveBM->isActive = false;
            pActiveBM->VDeactivateBM();
            // pActiveBM->DeactivateBM();
            pActiveBM = this;
        }
        VActivateBM();
    }
}

void ZC_ButtonManipulator::DeactivateBM()
{
    if (!isActive) return;
    isActive = false;
    if (pFloorBM != this)
    {
        if (isLadder) ZC_ForwardListErase(ladderBM, this);
        else if (pActiveBM == this) pFloorBM->ActivateBM();
    }
    VDeactivateBM();
}

bool ZC_ButtonManipulator::IsFloorBM() const noexcept
{
    return this == pFloorBM;
}

bool ZC_ButtonManipulator::IsEventsTargetBM() const noexcept
{
    return !isActive ? false
        : ladderBM.empty() ? pActiveBM == this
        : *ladderBM.begin() == this;
}

bool ZC_ButtonManipulator::IsActiveBM() const noexcept
{
    return isActive;
}

void ZC_ButtonManipulator::FirstButtonDown(ZC_ButtonID buttonId, float time)
{
    assert(pFloorBM);   //  floor must be allways

    if (buttonId == M_LEFT || buttonId == M_MIDLE || buttonId == M_RIGHT || buttonId == M_X1 || buttonId == M_X2) return;

    if (!ladderBM.empty())   //  work with staircaseBM
    {
        auto pBM = (*(ladderBM.begin()));
        //  As a backspace take ESCAPE or activateButtonId (if use_activateButtonId_ForDeactivation = true button) for each ZCR_ButtonManipulator.
        //  If esacape deactivate allready active heir, otherwise call heir's function
        buttonId == K_ESCAPE || (pBM->use_activateButtonId_ForDeactivation ? buttonId == pBM->activateButtonId : false) ? 
            (*ladderBM.begin())->DeactivateBM() : (*ladderBM.begin())->VCallButtonDownBM(buttonId, time);
    }
    else     //  work with pActiveBM
    {
        if (buttonId == K_ESCAPE || (pActiveBM->use_activateButtonId_ForDeactivation ? buttonId == pActiveBM->activateButtonId : false))
        {
            if (pActiveBM != pFloorBM) pActiveBM->DeactivateBM();   //  can't deactivate pFloorBM level
        }
        else pActiveBM->VCallButtonDownBM(buttonId, time);
    }
}