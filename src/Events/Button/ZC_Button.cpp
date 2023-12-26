#include <ZC/Events/Button/ZC_Button.h>

ZC_Button::ZC_Button(ZC_ButtonID _id, State _state) noexcept
    : id(_id),
    state(_state)
{}

bool ZC_Button::operator < (const ZC_Button& btn) const noexcept
{
    return id < btn.id || state < btn.state;
}