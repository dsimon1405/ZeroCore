#pragma once

#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/GUI/ZC_GUI_Obj.h>

template<typename TFunc>    //  TObject -> ZC_Function<void(float)> or ZC_Function<void(float)>* or ZC_GUI_Obj*
struct ZC_ConnectedButton
{
    ZC_ButtonID buttonId;
    TFunc func;   //  may be: ZC_Function<void(ZC_ButtonID,float)>, ZC_Function<void(ZC_ButtonID,float)>*
    ZC_GUI_Obj* pGuiObj;    //  sets only on down in ZC_ButtonPressedDown::AddActiveDownButton(), and released in ZC_ButtonPressedDown::EraseActiveDownButton

    template<typename TObject>  //  may be pointer on gui object or pointer on function
    bool operator == (TObject pObject) const noexcept
    {
        if constexpr (std::same_as<TObject, ZC_ButtonID>)
        {
            return buttonId == pObject;
        }
        else if constexpr (std::same_as<TObject, ZC_GUI_Obj*>)
        {
            return pGuiObj == pObject;
        }
        else if constexpr (std::is_pointer<TFunc>())      //  ZC_Function<void(ZC_ButtonID,float)>*
        {
            return (*func).GetPointerOnData() == pObject;
        }
        else    //  ZC_Function<void(ZC_ButtonID,float)>
        {
            return func.GetPointerOnData() == pObject;
        }
    }

    void Call(float time)
    {
        if (pGuiObj)      //  if have gui obj pointer, calls his button down event
        {
            if (!(pGuiObj->ButtonDown(buttonId, time))) pGuiObj = nullptr;  //  if returns false, object must be disabled
            return;
        }
        if constexpr (std::same_as<TFunc, ZC_Function<void(ZC_ButtonID, float)>*>)    //  ZC_Function<void(ZC_ButtonID,float)>*
        {
            if (func) (*func)(buttonId, time);
        }
        else    //  ZC_Function<void(ZC_ButtonID,float)>
        {
            func(buttonId, time);
        }
    }

        //  returns true if have event
    bool ButtonUp_GUIObj(ZC_ButtonID buttonId, float time)
    {
        if (pGuiObj)
        {
            pGuiObj->ButtonUp(buttonId, time);
            return true;
        }
        else if constexpr (std::is_pointer<TFunc>())      //  ZC_Function<void(ZC_ButtonID,float)>*
        {
            return func;
        }
        else    //  ZC_Function<void(ZC_ButtonID,float)>
        {
            return func.GetPointerOnData();
        }
    }
};