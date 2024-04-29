#pragma once

#include "ZC_ButtonID.h"
#include <ZC/Tools/Signal/ZC_SConnection.h>

#include <ZC/Events/ZC_Events.h>

#include <vector>

struct ZC_ButtonsCombination
{
    struct Button
    {
        ZC_ButtonID buttonId;
        ZC_SConnection sconButtonUp {};

        Button(ZC_ButtonID _buttonId)
            : buttonId(_buttonId)
        {}

        ZC_ButtonID ConnectButtonUp(ZC_Function<void(float)>&& funcFail)
        {
            sconButtonUp = ZC_Events::ConnectButtonUp(buttonId, std::move(funcFail));
            return buttonId;
        }

        void DisconnectButtonUp()
        {
            sconButtonUp.Disconnect();
        }
    };

    ZC_SConnection sconButtonDown;
    std::vector<Button> buttons;
    size_t buttonsId = 0;
    ZC_Function<void()> funcSuccess;

    ZC_ButtonsCombination(std::vector<Button>&& _buttons, ZC_Function<void()>&& _func)
        : buttons(std::move(_buttons)),
        funcSuccess(std::move(_func))
    {}

    void Connect(float time = 0)
    {
        if (buttonsId == 0)
        {
            sconButtonDown = ZC_Events::ConnectButtonDown(buttons[buttonsId++].ConnectButtonUp({ &ZC_ButtonsCombination::Fail, this }),
                { &ZC_ButtonsCombination::Connect, this }, false);
        }
        else
        {
            buttons[buttonsId++].ConnectButtonUp({ &ZC_ButtonsCombination::Fail, this });
            sconButtonDown.Disconnect();
            sconButtonDown = ZC_Events::ConnectFirstDownButton({ &ZC_ButtonsCombination::ButtonCheck, this });
        }
    }

    void Fail(float time = 0.f)
    {
        for (auto& button : buttons) button.DisconnectButtonUp();
        buttonsId = 0;
        Connect();
    }

    void ButtonCheck(ZC_ButtonID buttonId, float time)
    {
        if (buttonId == buttons[buttonsId].buttonId)
        {
            if (buttons.size() - 1 == buttonsId)
            {
                funcSuccess();  //  combination reached, call users function
                Fail();     //  success, reconnect
            }
            else
            {
                buttons[buttonsId++].ConnectButtonUp({ &ZC_ButtonsCombination::Fail, this });
            }
        }
        else Fail();    //  fail, reconnect
    }
};