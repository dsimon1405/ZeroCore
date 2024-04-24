#include <ZC/Events/ZC_SingleClickButton.h>

#include <ZC/Events/ZC_Events.h>

ZC_SingleClickButton::ZC_SingleClickButton(ZC_ButtonID _buttonId, ZC_Function<void(float)> _fDown, ZC_Function<void(float)> _fUp)
    : buttonId(_buttonId),
    fDown(std::move(_fDown)),
    fUp(std::move(_fUp))
{}

ZC_SingleClickButton::~ZC_SingleClickButton()
{
    sconButton.Disconnect();
}

void ZC_SingleClickButton::Connect()
{
    if (!sconButton.IsConnected()) sconButton = ZC_Events::ConnectButtonDown(buttonId, { &ZC_SingleClickButton::Down, this }, false);
}

void ZC_SingleClickButton::Disconnect()
{
    sconButton.Disconnect();
}

void ZC_SingleClickButton::Down(float time)
{
    sconButton.Disconnect();
    sconButton = ZC_Events::ConnectButtonUp(buttonId, { &ZC_SingleClickButton::Up, this });
    fDown(time);
}

void ZC_SingleClickButton::Up(float time)
{
    sconButton.Disconnect();
    sconButton = ZC_Events::ConnectButtonDown(buttonId, { &ZC_SingleClickButton::Down, this }, false);
    if (fUp) fUp(time);
}