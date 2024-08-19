#pragma once

#include <ZC/GUI/ZC_GUI_WinImmutable.h>
#include <ZC/GUI/Text/ZC_GUI_Text.h>
#include <ZC/Events/ZC_EC.h>
#include <ZC/Tools/Time/ZC_Clock.h>
#include <ZC/GUI/ZC_cGUI_Number.h>

#include <climits>

struct ZC_GUI_TextInputWindow : protected ZC_GUI_WinImmutable
{
    friend class ZC_GUI;

    struct INumberInput
    {
        virtual ~INumberInput() = default;

        virtual std::wstring GetStart_wstr() = 0;
            //  return true if character coud be added, otherwise false
        virtual bool NewCharacter(wchar_t wch) = 0;
        virtual void Call_Callback() = 0;
    };

    template <ZC_GUI_Number::cNumber TNum>
    struct NumberInput : public INumberInput
    {
        NumberInput(TNum _number, ZC_Function<void(TNum)>&& _callback, uchar _afterDot_count)
            : number(_number),
            funcChangedNumber(std::move(_callback)),
            afterDot_count(std::same_as<TNum, float> || std::same_as<TNum, double> || std::same_as<TNum, long double> ?
                _afterDot_count < 1 ? 1   //  1 is min after point count
                    : _afterDot_count > 6 ? 6     //  6 is max after point count
                        : _afterDot_count     //  legal after point count
                : 0),   //  number is integer
            haveSigne((std::same_as<TNum, signed char> || std::same_as<TNum, short> || std::same_as<TNum, int> || std::same_as<TNum, long> || std::same_as<TNum, long long>
                || std::same_as<TNum, float> || std::same_as<TNum, double> || std::same_as<TNum, long double>)),
            haveDot(std::same_as<TNum, float> || std::same_as<TNum, double> || std::same_as<TNum, long double>)
        {}

        TNum number;    //  uses only in GetStart_wstr()
        ZC_Function<void(TNum)> funcChangedNumber;
        TNum afterDot_count;    //  uses only in GetStart_wstr()
        bool haveSigne; //  may have minus
        bool haveDot;   //  number is float double ldouble

        std::wstring GetStart_wstr() override
        {
            return ZC_GUI_Number::NumberToWstr(number, afterDot_count);
        }
        
        bool NewCharacter(wchar_t wch) override
        {
            switch (wch)
            {
            case L'1': return true;
            case L'2': return true;
            case L'3': return true;
            case L'4': return true;
            case L'5': return true;
            case L'6': return true;
            case L'7': return true;
            case L'8': return true;
            case L'9': return true;
            case L'0': return true;
            case L'-': return haveSigne && ((pTIW->highlight.VGetWidth_Obj() != 0 && pTIW->highlight.start_x == pTIW->chDatas.begin()->start_index)      //  have highlight with starts at the begining of the wstring
                                    || (pTIW->caret.IsCaretOnStart() && (pTIW->text.GetWStr().empty() ? true : pTIW->text.GetWStr()[0] != L'-')));     //  caret at the beginning and the first character of the text, if any, not a minus
            case L'.': return haveDot && (!(pTIW->text.GetWStr().contains(wch)) || pTIW->highlight.IsContainCharacter(wch));
            default: break;
            }
            return false;
        }

        void Call_Callback()
        {
            std::wstring wstr = pTIW->text.GetWStr();
            if (wstr.size() == 0) return;   //  wstr is empty, don't call funcChangedNumber()
            else if (wstr.size() == 1 && wstr[0] == L'-') funcChangedNumber(TNum(0));    //  in wstr only -> minus
            else if (std::same_as<TNum, float>) funcChangedNumber(std::stof(wstr));
            else if (std::same_as<TNum, double>) funcChangedNumber(std::stod(wstr));
            else if (std::same_as<TNum, long double>) funcChangedNumber(std::stold(wstr));
            else    //  number is integer, check min/max limit
            {
                    //  need avoid 0 on start for correct min/max type limitation
                std::wstring wstr_integer;
                bool isNegative = wstr[0] == L'-';
                size_t i = isNegative ? 1 : 0;  //  if negative first index must be 1, otherwise 0
                for ( ; i < wstr.size(); ++i)
                {
                    if (wstr[i] != L'0')  //  found not null symbol
                    {
                        size_t nulls_count = isNegative ? i - 1 : i;  //  count of nulls on start (if number is negative, index must exclude that minus from nulls counting)
                        wstr_integer = nulls_count == 0 ? wstr :    //  no nulls on start
                            isNegative ? L"-" + wstr.substr(i, wstr.size()) : wstr.substr(i, wstr.size());      //  need cut nulls on front of the wstr, second param of substr must consider number of copying symbols, may be more then need if copying to the end of the string like in that case
                        break;
                    }
                }
                if ((wstr_integer.size() == 1 && wstr[0] == L'-')    //  in wstr_integer only -> minus
                    || (wstr_integer.size() == 0 && wstr[(isNegative ? 1 : 0)] == L'0')) funcChangedNumber(TNum(0));    //  in wstr_integer is empty but in wstr null(s), may be with minus
                else
                {
                    auto lambCompare = [&wstr_integer, isNegative](const std::wstring& compareer) -> const std::wstring&
                    {
                        return compareer.size() > wstr_integer.size() ? wstr_integer
                            : compareer.size() < wstr_integer.size() ? compareer
                            : compareer.compare(wstr_integer) >= 0 ? wstr_integer : compareer; 
                    };

                    funcChangedNumber(static_cast<TNum>(haveSigne ? std::stoll(std::same_as<TNum, schar> ? lambCompare(isNegative ? schar_min : schar_max)
                                                                                : std::same_as<TNum, short> ? lambCompare(isNegative ? short_min : short_max)
                                                                                : std::same_as<TNum, int> ? lambCompare(isNegative ? int_min : int_max)
                                                                                : std::same_as<TNum, long> ? lambCompare(isNegative ? long_min : long_max)
                                                                                : lambCompare(isNegative ? llong_min : llong_max))
                                                                : std::stoull(std::same_as<TNum, uchar> ? lambCompare(uchar_max)
                                                                                : std::same_as<TNum, ushort> ? lambCompare(ushort_max)
                                                                                : std::same_as<TNum, uint> ? lambCompare(uint_max)
                                                                                : std::same_as<TNum, ulong> ? lambCompare(ulong_max)
                                                                                : lambCompare(ullong_max))));
                }
            }
        } 
    };

    static void SetColors(uint textInput_background, uint textInput_text, uint textInput_caret, uint texInput_highlight);

    static bool StartInputWindow(float bl_x, float bl_y, int win_width, int max_symbols, const std::wstring& wstr, ZC_Function<void(const std::wstring&)>&& _callBack, bool highlight_text);
    
    template <ZC_GUI_Number::cNumber TNum>
    static bool StartInputWindow(float bl_x, float bl_y, int win_width, NumberInput<TNum>&& numberInput, bool highlight_text)
    {
        if (!(pTIW->StartInputWindow(bl_x, bl_y, win_width, std::same_as<TNum, schar> ? schar_min.size() :
                                                            std::same_as<TNum, uchar> ? uchar_max.size() :
                                                            std::same_as<TNum, short> ? short_min.size() :
                                                            std::same_as<TNum, ushort> ? ushort_max.size() :
                                                            std::same_as<TNum, int> ? int_min.size() :
                                                            std::same_as<TNum, uint> ? uint_max.size() :
                                                            std::same_as<TNum, long> ? long_min.size() :
                                                            std::same_as<TNum, ulong> ? ulong_max.size() :
                                                            std::same_as<TNum, llong> ? llong_min.size() :
                                                            std::same_as<TNum, ullong> ? ullong_max.size() :    //  rest is float, double, ldouble. Take 20 symbols for them
                                                                20, numberInput.GetStart_wstr(), highlight_text))) return false;
        upINumberInput = new NumberInput<TNum>(std::move(numberInput));
        return true;
    }

    bool VIsInputWindow_W() const noexcept override;
    ZC_GUI_Obj* VGetButtonKeyboard_W(ZC_ButtonID buttonId) override;
    void VButtonDown_Obj(ZC_ButtonID buttonId, float time) override;
    void VButtonUp_Obj(ZC_ButtonID buttonId, float time) override;

    void TextButtonDown(unsigned char ch);

private:
    static inline std::wstring schar_min;
    static inline std::wstring schar_max;
    static inline std::wstring uchar_max;
    static inline std::wstring short_min;
    static inline std::wstring short_max;
    static inline std::wstring ushort_max;
    static inline std::wstring int_max;
    static inline std::wstring int_min;
    static inline std::wstring uint_max;
    static inline std::wstring long_min;
    static inline std::wstring long_max;
    static inline std::wstring ulong_max;
    static inline std::wstring llong_min;
    static inline std::wstring llong_max;
    static inline std::wstring ullong_max;
    
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
        bool IsContainCharacter(wchar_t character);
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

        bool IsCaretOnStart();
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
    ZC_GUI_Text text;
    Caret caret;

        //  sets text callback or number system (for number input) in different StartInputWindow() methods
    static inline ZC_Function<void(const std::wstring&)> funcChangedWstr;   //  callback for text input
    static inline ZC_uptr<INumberInput> upINumberInput;     //  for number input (include callback)

    int winEnd = 0;

    std::list<ZC_GUI_ChData> chDatas;
    int chDatasOffset = 0;    //  texture's offset in window (negative or 0)

    ZC_GUI_TextInputWindow(float fontHeight);

    void VSetDrawState_W(bool needDraw) override;

    bool StartInputWindow(float bl_x, float bl_y, int win_width, int max_symbols, const std::wstring& wstr, bool highlight_text);

    void WindowResize(float,float);
};