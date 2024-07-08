#pragma once

#include <ZC/Events/ZC_ButtonID.h>
#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Events/ZC_EC.h>

#include <forward_list>

class ZC_ButtonHolder;

class ZC_ButtonClick
{
public:
    ZC_ButtonClick(ZC_ButtonHolder* _pButtonHolder);

    ZC_EC Connect(ZC_ButtonID buttonId, ZC_Function<void(ZC_ButtonID, float)>&& funcDown, ZC_Function<void(ZC_ButtonID, float)>&& funcUp);
    void Disconnect(const void* pFuncDownData);
        //  returns true if button was used
    bool CallButtonDown(ZC_ButtonID buttonId, float time);
        //  returns true if button was used
    bool CallButtonUp(ZC_ButtonID buttonId, float time);
    bool IsButtonDownConnected(ZC_ButtonID buttonId);
        //  release button if it in pressed state (withought call events)
    void ButtonWasReleased(ZC_ButtonID buttonId);

private:
    struct ButtonHolder
    {
        ZC_ButtonID buttonId;
        ZC_Function<void(ZC_ButtonID, float)> funcDown,
            funcUp;
        bool callDown = true;   //  this flag is used to avoid calls to the pressed button

        bool operator == (ZC_ButtonID _buttonId) const noexcept;
        bool operator == (const void* pFuncDownData) const noexcept;

        bool CallButtonDown(ZC_ButtonID buttonId, float time);
        bool CallButtonUp(ZC_ButtonID buttonId, float time);
    };

    ZC_ButtonHolder* pButtonHolder;
    std::forward_list<ButtonHolder> buttonHolders;
};