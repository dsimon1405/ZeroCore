#pragma once

#include <ZC/GUI/Backend/ZC_GUI_ObjComposite.h>
#include <ZC/Tools/Time/ZC_Clock.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Colors.h>

typedef int ZC_GUI_ButtonFlags;
enum ZC_GUI_ButtonFlag
{   //  MB flags work only with ZC_GUI_ButtonMouse realizations
    ZC_GUI_BF__None                       = 0,
    ZC_GUI_BF_M__CursorMoveOnMBLPress     = 1,        //  if the left mouse button is pressed on the button, VCursorMove_Obj() will be called until the button is released
    ZC_GUI_BF_M__Scroll                   = 1 << 1,   //  if the mouse cursor under the button, VScroll_Obj() will be called until the cursore leave button's area
    ZC_GUI_BF_M__DoubleCLick              = 1 << 2,   //  mouse left button double click (fast) will call VLeftButtonDoubleClick() on the second click, not VLeftButtonDown_BM()
    ZC_GUI_BF__MBLPress                   = 1 << 3,   //  will call VLeftButtonPressed_BM() event, instead VLeftButtonDown_BM(), if mbl will pressed.
};

struct ZC_GUI_ButtonBase : public ZC_GUI_ObjComposite
{
    struct ColorsButton
    {
        uint color_button;                  //  color adding to the texture of a button by default
        uint color_button_under_cursor;     //  color adding to the texture of a button if button under cursor
        uint color_button_pressed;          //  color adding to the texture of a button if button pressed
        uint color_not_active;              //  color adding to the texture of a button if button not active (activity changes in method VChangeButtonActivity_BS())

        ColorsButton(uint _color_button = ZC_GUI_Colors::button, uint _color_button_under_cursor = ZC_GUI_Colors::button_under_cursor,
            uint _color_button_pressed = ZC_GUI_Colors::button_pressed, uint _color_not_active = ZC_GUI_Colors::button_not_active);
    } colorsButton;
    
    enum ButtonState
    {
        BS_Pressed,
        BS_Released,
        BS_HoldUntilRelease,
    };
    ButtonState bs_mouseButton = BS_Released;   //  mouse button left
    ButtonState bs_keyboardButton = BS_Released;
    ZC_GUI_ButtonFlags buttonFlags;

    ZC_Clock clock; //  uses for ZC_GUI_BF_M__DoubleCLick, ZC_GUI_MB__MBLPress events
    long pressed_time = 0;
    static inline long waitPressLimit_nanosec = 300000000;   //  how long wait to start use VKeyboardButtonPressed_BK(), instead VKeyboardButtonDown_BK()
    static inline long pressedInterval_nanosec = 100000000;

    bool isButtonActive = true;

    ZC_GUI_ButtonBase(const ZC_GUI_ObjData& od, ZC_GUI_ButtonFlags _b_flags, const ColorsButton& _colorsButton = {});
        
    /*
    Sets nanosecond limit of waiting to start use ZC_GUI_ButtonKeyboard::VKeyboardButtonPressed_BK() instead ZC_GUI_ButtonKeyboard::VKeyboardButtonDown_BK(), or ZC_GUI_ButtonMouse::VLeftButtonPressed_BM() instead
    ZC_GUI_ButtonMouse::VLeftButtonDown_BM(). Default 300000000 nanoseconds (uses for all objects).

    Params:
    - _waitPressLimit_nanosec - must be more then 0.
    */
    static void SetWaitPressLimit_BS(long _waitPressLimit_nanosec);

    /*
    Sets nanosecond interval between calls ZC_GUI_ButtonKeyboard::VKeyboardButtonPressed_BK() or ZC_GUI_ButtonMouse::VLeftButtonPressed_BM(). Default 100000000 nanoseconds (uses for all objects).

    Params:
    - _pressedInterval_nanosec - must be more then 0.
    */
    static void SetPressedInterval_BS(long _pressedInterval_nanosec);

        //  uses in method VStopEventActivity_Obj() from ZC_GUI_ButtonMouse and ZC_GUI_ButtonKeyboard
    void StopEventActivity_BS();
        //  change the activity of the button (inactive does not process events and does not change color)
    void VChangeObjectActivity_Obj(bool _isAvtive, bool changeGPU) override;
};

typedef typename ZC_GUI_ButtonBase::ColorsButton ZC_GUI_ColorsButton;