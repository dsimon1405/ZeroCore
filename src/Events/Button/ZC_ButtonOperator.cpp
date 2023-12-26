#include <ZC/Events/Button/ZC_ButtonOperator.h>

ZC_SConnection<void(float)> ZC_ButtonOperator::Connect(ZC_Button but, ZC_Function<void(float)> func)
{
    return ssButton.Connect(but, std::move(func));
}