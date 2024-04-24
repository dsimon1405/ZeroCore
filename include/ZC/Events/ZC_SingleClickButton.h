#pragma once

#include "ZC_ButtonID.h"
#include <ZC/Tools/Function/ZC_Function.h>
#include <ZC/Tools/Signal/ZC_SConnection.h>

//  Class for calling a button's click function on a button click event only once until the button is no longer clicked.
class ZC_SingleClickButton
{
public:
    /*
    Params:
    - _buttonId - button for connection.
    - _fDown - function wich calls on button down event.
    - _fUp - function wich calls on button up event. If son't need could be nullptr.
    */
    ZC_SingleClickButton(ZC_ButtonID _buttonId, ZC_Function<void(float)> _fDown, ZC_Function<void(float)> _fUp);

    ZC_SingleClickButton(ZC_SingleClickButton&&) = delete;
    ZC_SingleClickButton& operator = (ZC_SingleClickButton&&) = delete;

    ~ZC_SingleClickButton();

    void Connect();
    void Disconnect();

private:
    ZC_ButtonID buttonId;
    ZC_Function<void(float)> fDown;
    ZC_Function<void(float)> fUp;
    ZC_SConnection sconButton;

    void Down(float time);
    void Up(float time);
};