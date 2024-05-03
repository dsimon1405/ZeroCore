#pragma once

#include <ZC/Events/ZC_Events.h>
#include <ZC/Tools/Container/ZC_ContFunc.h>
#include <forward_list>
#include <cassert>

struct ZC_ButtonManipulator
{
    //  descrition                      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ZC_ButtonManipulator(ZC_ButtonID _activateButtonId, bool _isStaircase, bool isFloor)
        : activateButtonId(_activateButtonId),
        isStaircase(_isStaircase)
    {
        assert(!ZC_FindInPointers(allHeirs, activateButtonId));   //  allready existst ZC_ButtonManipulator with such activateButtonId
        if (isFloor) pFloorBM = this;
        if (allHeirs.empty()) ecFistDownButton.NewConnection(ZC_Events::ConnectFirstDownButton({ &ZC_ButtonManipulator::FirstButtonDown }));
        allHeirs.emplace_front(this);
    }

    ~ZC_ButtonManipulator()
    {
        if (isActive)
        {
            if (isStaircase) ZC_ForwardListErase(staircaseBM, this);
            else pActiveBM = nullptr;
        }
        ZC_ForwardListErase(allHeirs, this);
        if (allHeirs.empty()) ecFistDownButton.Disconnect();
    }

    bool operator == (ZC_ButtonID buttonId)
    {
        return buttonId == activateButtonId;
    }

    void ActivateBM()
    {
        if (isActive) return;
        isActive = true;
        if (isStaircase) staircaseBM.emplace_front(this);
        else pActiveBM = this;
        VActivateBM();
    }

    void DeactivateBM()
    {
        if (!isActive) return;
        isActive = false;
        if (isStaircase) staircaseBM.erase_after(staircaseBM.before_begin());
        else pFloorBM->ActivateBM();
        VDeactivateBM();
    }

    bool IsActiveBM()
    {
        return isActive;
    }

private:
    bool isActive = false;
    ZC_ButtonID activateButtonId;
    bool isStaircase;

    virtual void VCallButtonDownBM(ZC_ButtonID buttonId, float time) = 0;  //  here heir must realize switch for all necessary fucntions
    virtual void VActivateBM() {}      //  if need some activation on activate
    virtual void VDeactivateBM() {}    //  if need some deactivation on deactivate

    static inline std::forward_list<ZC_ButtonManipulator*> allHeirs;
    static inline std::forward_list<ZC_ButtonManipulator*> staircaseBM;
    static inline ZC_ButtonManipulator* pFloorBM = nullptr;
    static inline ZC_ButtonManipulator* pActiveBM = nullptr;
    static inline ZC_EC ecFistDownButton;

    static void FirstButtonDown(ZC_ButtonID buttonId, float time)
    {
        if (!staircaseBM.empty())
        {
            buttonId == K_ESCAPE ? (*staircaseBM.begin())->DeactivateBM() : (*staircaseBM.begin())->VCallButtonDownBM(buttonId, time);
        }
        else if (pActiveBM)     //  have active manipulator work with them
        {   //  take ESCAPE as a backspace button for each ZCR_ButtonManipulator. If esacape deactivate allready active heir, otherwise call heir's function
            buttonId == K_ESCAPE ? pActiveBM->DeactivateBM() : pActiveBM->VCallButtonDownBM(buttonId, time);
        }
        else
        {   //  tries to find heir, that have activate button, if found makes him active
            auto pManipulator = ZC_FindInPointers(allHeirs, buttonId);
            if (pManipulator) pManipulator->ActivateBM();
        }
    }
};