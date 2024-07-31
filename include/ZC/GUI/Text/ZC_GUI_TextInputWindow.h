#pragma once

#include <ZC/GUI/ZC_GUI_WinImmutable.h>
#include <ZC/GUI/Text/ZC_GUI_Text.h>
#include <ZC/Events/ZC_EC.h>
#include <ZC/Tools/Time/ZC_Clock.h>

struct ZC_GUI_TextInputWindow : protected ZC_GUI_WinImmutable
{
    friend class ZC_GUI;
 
    static bool StartInputWindow(float bl_x, float bl_y, int width, const std::wstring& wstr, ZC_Function<void(const std::wstring&)> _callBack);

    bool VIsInputWindow_W() const noexcept override;
    ZC_GUI_Obj* VGetButtonKeyboard_W(ZC_ButtonID buttonId) override;
    void VButtonDown_Obj(ZC_ButtonID buttonId, float time) override;
    void VButtonUp_Obj(ZC_ButtonID buttonId, float time) override;

    void TextButtonDown(unsigned char ch);

private:
    struct Highlight : public ZC_GUI_Obj
    {
        enum Direction
        {
            None,
            Left,
            Right,
        };

        float start_x = 0;  //  highlight bl.x
        float end_x = 0;    //  highlight br.x (to calculate width)
        Direction direction = None;     //  start_x is bl.x. If Left: start_x is moveing. If Right: end_x is moving

        Highlight(float fontHeight);

        void MBL_DoubleClick();
        void MBL_TripleClick();
        void KeyboardButtonLeftDown();
        void KeyboardButtonRightDown();
        void BackspaceDown();
        void DeleteDown();
        void TextButtonDown();
        bool IsHighLight();
        void SetDefaultState();
        void UpdateData();
        void DeleteHighlight(bool needSubData);
    };

    struct EventHandler
    {
        ZC_ButtonID buttonId_lastDown;  //  store last down button
        wchar_t wch_lastDown;   //  store last down unicode symbol

        enum MBL_Click
        {
            First,
            Double,
            Triple
        };
        
        ZC_Clock clock_click;
        MBL_Click mbl_click = First;

        const float waitPressNanoseconds = 300000000.f;   //  how long wait to start use VKeyboardButtonPressed_BK(), instead VKeyboardButtonDown_BK()
        const float beteenCallsWhenPressedNanoseconds = 100000000.f;   //  how long to wait between calls when the button is pressed
        float wait_time = 0.f;   //  current wait time
        bool buttonPressedStarted = false;

        bool isCampsLock = false;

        ZC_GUI_Obj* AddButtonDown(ZC_ButtonID buttonId);
        void TextButtonDown(unsigned char ch);
        void ButtonDown(ZC_ButtonID buttonID, float time);
        void ButtonUp(ZC_ButtonID buttonID, float time);
        void CallPressedButton(float time, bool isLeft, void(ZC_GUI_TextInputWindow::Highlight::* pFuncMove)());
        wchar_t GetUnicode(unsigned char ch, ZC_ButtonID butID);
    };

    struct Caret : public ZC_GUI_Obj
    {
        enum DrawState
        {
            Blinking,
            NotDisplay,
            Display
        };
        
        ZC_GUI_ChData* pChData_pos = nullptr;    //  it's positoin, caret now after that character in text. If nullptr with not empty text then caret in the begining of the text.
        DrawState drawState = NotDisplay;
        ZC_EC ecBlinkingUpdate;
        const float blinkTime_limit = 600000000;    //  nanoseconds
        float blinkTime = 0;    //  nanoseconds

        Caret(float fontHeight);

        void SetDrawState(DrawState _drawState);
            //  set position by ZC_GUI_ChData*. Position for caret is space after ChData*
        void SetPositionOfChData(ZC_GUI_ChData* _pChData_pos, bool needSubData, DrawState _drawState);
        void SetPositionOfMouseCursor();
        void KeyboardButtonLeftDown(bool needSubData);
        void KeyboardButtonRightDown(bool needSubData);
        void BackspaceDown();
        void DeleteDown();
        void TextButtonDown(const typename ZC_GUI_Font::Character* pCh);

    private:
        void BlinkingUpdate(float time);
    };

    static inline ZC_GUI_TextInputWindow* pTIW;
  
    EventHandler eventHandler;

    Highlight highlight;
    ZC_GUI_Text textMut;
    Caret caret;
    ZC_Function<void(const std::wstring&)> funcChangedWstr;

    int winEnd = 0;
    std::wstring startWString;

    std::list<ZC_GUI_ChData> chDatas;
    int chDatasOffset = 0;    //  texture's offset in window (negative or 0)

    ZC_GUI_TextInputWindow(float fontHeight);

    void VSetDrawState_W(bool needDraw) override;

        //  call user's callback
    void WstrChanged();
    void WindowResize(float,float);
};