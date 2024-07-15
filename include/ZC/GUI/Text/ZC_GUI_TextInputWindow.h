#pragma once

#include <ZC/GUI/ZC_GUI_WinImmutable.h>
#include <ZC/GUI/Text/ZC_GUI_TextMutable.h>
#include <ZC/Events/ZC_EC.h>

#include <ZC/GUI/ZC_GUI_IconUV.h>
#include <ZC/Video/ZC_SWindow.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC/Tools/Math/ZC_Math.h>

struct ZC_GUI_TextInputWindow : protected ZC_GUI_WinImmutable
{
    friend class ZC_GUI;

    struct QuadObj : public ZC_GUI_Obj
    {
        QuadObj(float width, float height, unsigned int color)
            : ZC_GUI_Obj(ZC_GUI_ObjData{ .width = width, .height = height, .color = color, .uv = ZC_GUI_IconUV::window, .tex_binding = ZC_GUI_Bindings::bind_tex_Icons })
        {}
    };

    struct Highlight : public QuadObj
    {
        enum Direction
        {
            None,
            Left,
            Right,
        };

        float start_x = 0;  //  highlight bl.x
        float end_x = 0;    //  highlight br.x (to caltulate width)
        Direction direction = None;     //  start_x is bl.x. If Left: start_x is moveing. If Right: end_x is moving

        Highlight(float fontHeight)
            : QuadObj(0, fontHeight, ZC_PackColorUCharToUInt(0,0,0))
        {}

        void KeyboardButtonLeftDown()
        {
            if (!(pTIW->caret.pChData_pos)) return;     //  already at left
            if (ZC_Events::IsButtonPressed(K_LSHIFT) || ZC_Events::IsButtonPressed(K_RSHIFT))
            {
                switch (direction)
                {
                case None:
                {
                    ZC_GUI_ChData* pChD_end = pTIW->caret.pChData_pos; 
                    pTIW->caret.KeyboardButtonLeftDown(false);
                    end_x = pChD_end->start_index + pTIW->chDatasOffset + (&(pTIW->chDatas.front()) == pChD_end ? 0 : pChD_end->pCh->left_offset) + pChD_end->pCh->width;   //  end of carets pos
                    start_x = pTIW->caret.pChData_pos ?   //  if have pChData_pos, then start_x is end of pChData_pos
                        pTIW->caret.pChData_pos->start_index + pTIW->chDatasOffset + (&(pTIW->chDatas.front()) == pTIW->caret.pChData_pos ? 0 : pTIW->caret.pChData_pos->pCh->left_offset) + pTIW->caret.pChData_pos->pCh->width
                        : (*(pTIW->pBL))[0];    //  start_x is begin of the window
                        //  update bls
                    (*this->pBL)[0] = start_x;
                    VSubDataBL_Obj(this->pBL, pTIW->caret.pBL);     //  update bls: hilight, textMut, caret
                        //  update width
                    this->pObjData->width = end_x - start_x;
                    VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), sizeof(ZC_GUI_ObjData::width), &(this->pObjData->width));
                    direction = Left;
                } break;
                case Right:
                {
                    pTIW->caret.KeyboardButtonLeftDown(false);
                    float caret_pos_x = pTIW->caret.pChData_pos ?   //  if have pChData_pos, then caret_pos_x is end of pChData_pos
                        pTIW->caret.pChData_pos->start_index + pTIW->chDatasOffset + (&(pTIW->chDatas.front()) == pTIW->caret.pChData_pos ? 0 : pTIW->caret.pChData_pos->pCh->left_offset) + pTIW->caret.pChData_pos->pCh->width
                        : (*(pTIW->pBL))[0];    //  caret_pos_x is begin of the window
                    
                    //  where is caret
                } break;
                case Left:
                {
                    pTIW->caret.KeyboardButtonLeftDown(false);
                    start_x = pTIW->caret.pChData_pos ?   //  if have pChData_pos, then start_x is end of pChData_pos
                        pTIW->caret.pChData_pos->start_index + pTIW->chDatasOffset + (&(pTIW->chDatas.front()) == pTIW->caret.pChData_pos ? 0 : pTIW->caret.pChData_pos->pCh->left_offset) + pTIW->caret.pChData_pos->pCh->width
                        : pTIW->chDatas.front().start_index + pTIW->chDatasOffset;    //  start_x is begin of front symbol
                        //  update bls
                    (*this->pBL)[0] = start_x;
                    VSubDataBL_Obj(this->pBL, pTIW->caret.pBL);     //  update bls: hilight, textMut, caret
                        //  update width
                    this->pObjData->width = end_x - start_x;
                    VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), sizeof(ZC_GUI_ObjData::width), &(this->pObjData->width));
                } break;
                }
            }
            else
            {
                DefaultState();
                pTIW->caret.KeyboardButtonLeftDown(true);
            }
        }

        void KeyboardButtonRightDown()
        {
            pTIW->caret.KeyboardButtonRightDown(true);
        }

        void BackspaceDown()
        {
            pTIW->caret.BackspaceDown();
        }
        
        void DeleteDown()
        {
            pTIW->caret.DeleteDown();
        }
        
        void TextButtonDown()
        {
            pTIW->caret.TextButtonDown();
        }


        void DefaultState()
        {
            if (this->pObjData->width != 0.f)
            {
                this->pObjData->width = 0.f;
                VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), sizeof(ZC_GUI_ObjData::width), &(this->pObjData->width));
            }
            start_x = 0.f;
            end_x = 0.f;
            direction = None;
        }
    };

    struct Caret : public QuadObj
    {
        enum DrawState
        {
            Blinking,
            NotDisplay,
            Display
        };

        Caret(float fontHeight)
            : QuadObj(1, fontHeight, ZC_PackColorUCharToUInt(150,150,150))
        {}
        
        ZC_GUI_ChData* pChData_pos = nullptr;    //  it's positoin, caret now after that character in text. If nullptr with not empty text then caret in the begining of the text.
        DrawState drawState = NotDisplay;
        ZC_EC ecBlinkingUpdate;
        const float blinkTime_limit = 600000000;    //  nanoseconds
        float blinkTime = 0;    //  nanoseconds

        void SetDrawState(DrawState _drawState)
        {
            if (drawState == _drawState) return;
            drawState = _drawState;
            switch (drawState)
            {
            case Blinking:
            {
                ecBlinkingUpdate.NewConnection(ZC_Events::ConnectHandleEventsEnd({ &ZC_GUI_TextInputWindow::Caret::BlinkingUpdate, this }));
                blinkTime = 0;
            } break;
            case NotDisplay:
            {
                ecBlinkingUpdate.Disconnect();  //  disconnect update if was blinking
                if (VIsDrawing_Obj()) ChangeObjsDrawState(false, this, this);   //  stop draw
            } break;
            case Display:
            {
                ecBlinkingUpdate.Disconnect();  //  disconnect update if was blinking
                if (!VIsDrawing_Obj()) ChangeObjsDrawState(true, this, this);   //  start draw
            } break;
            }
        }

            //  set positin by ChData*. Position for caret is space after ChData*
        void SetPositionOfChData(ZC_GUI_ChData* _pChData_pos, bool needSubData, DrawState _drawState)
        {
            pChData_pos = _pChData_pos;
            bool texturePosChanged = false;
            if (pChData_pos)    //   have object
            {
                int caretPos_start = _pChData_pos->start_index + pTIW->chDatasOffset + (&(pTIW->chDatas.front()) == pChData_pos ? 0 : _pChData_pos->pCh->left_offset)
                    + _pChData_pos->pCh->width;
                int caretPos_end = caretPos_start + this->pObjData->width;  //  add carets width
                if (caretPos_end > pTIW->winEnd)     //  caret_start after window end, need move left texture
                {
                    pTIW->chDatasOffset += pTIW->winEnd - caretPos_end;     //  move offset to left
                    (*(pTIW->textMut.pBL))[0] = (*(pTIW->pBL))[0] + pTIW->chDatasOffset;   //  move texture X left
                    texturePosChanged = true;
                    (*(pBL))[0] = pTIW->winEnd - this->pObjData->width;  //  caret before win end
                }
                else if (caretPos_start < int((*(pTIW->pBL))[0]))   //  caret start before window start, need move right texture
                {
                    pTIW->chDatasOffset += int((*(pTIW->pBL))[0]) - caretPos_start;     //  move offset to right
                    (*(pTIW->textMut.pBL))[0] = (*(pTIW->pBL))[0] + pTIW->chDatasOffset;   //  move texture X left
                    texturePosChanged = true;
                    (*(pBL))[0] = (*(pTIW->pBL))[0];     //  caret on win start
                }
                else (*(pBL))[0] = caretPos_start;  //  caret in the window space, set start pos after pChData_pos
            }
            else
            {
                float win_start_x = (*(pTIW->pBL))[0];
                (*(pBL))[0] = win_start_x;   //  no object, caret on win start
                if (pTIW->chDatasOffset != (int)win_start_x)
                {
                    (*(pTIW->textMut.pBL))[0] = win_start_x;
                    texturePosChanged = true;
                    pTIW->chDatasOffset = 0;
                }
            }

            if (needSubData) VSubDataBL_Obj(texturePosChanged ? pTIW->textMut.pBL : pBL, pBL);   //  if texture pos was changed, start subData from texture, otherwise only Caret (pBL)
            SetDrawState(_drawState);
        }

        void SetPositionOfMouseCursor()
        {
            float x = 0,
                y = 0;
            ZC_SWindow::GetCursorPosition(x, y);
            int cursor_x = x;

            pChData_pos = nullptr;
            auto chData_begin = pTIW->chDatas.begin();
            auto chData_beforeEnd = --(pTIW->chDatas.end());
            for (auto chData_iter = pTIW->chDatas.begin(); chData_iter != pTIW->chDatas.end(); ++chData_iter)
            {
                int chData_start = chData_iter->start_index + pTIW->chDatasOffset;
                int chData_end = chData_start + (chData_iter == chData_begin ? 0 : chData_iter->pCh->left_offset) + chData_iter->pCh->width;    //  if not forst character add left_offset
                int chData_midle = chData_end - (chData_iter->pCh->width / 2);  //  character's midle, not including left_offset
                if (chData_start <= cursor_x && cursor_x <= chData_midle)   //  cursor in first half of symbol, set caret on previous symbol
                {
                    pChData_pos = chData_iter == chData_begin ? nullptr : &*(--chData_iter);    //  if first element in list then set
                    break;
                }
                else if (chData_midle < cursor_x && (chData_iter == chData_beforeEnd ? true : cursor_x <= chData_end))   //  cursor in second half of symbol, if thiw is the last symbol, caret can be only on it, furthe in window empty space
                {
                    pChData_pos = &*chData_iter;
                    break;
                }
            } 
            SetPositionOfChData(pChData_pos, true, Display);
        }

        void KeyboardButtonLeftDown(bool needSubData)
        {
            auto iter = std::find(pTIW->chDatas.begin(), pTIW->chDatas.end(), pChData_pos);
            if (ZC_Events::IsButtonPressed(K_LCTRL) || ZC_Events::IsButtonPressed(K_RCTRL))
            {
                if (iter == pTIW->chDatas.begin())
                {
                    SetPositionOfChData(nullptr, needSubData, Display);
                    return;
                }
                while (true)
                {
                    --iter;
                    if (iter->pCh->symbol == L' ')  //  reached whitespace
                    {
                        SetPositionOfChData(&*iter, needSubData, Display);
                        return;
                    }
                    if (iter == pTIW->chDatas.begin())  //  reached text's begin
                    {
                        SetPositionOfChData(nullptr, needSubData, Display);
                        return;
                    }
                }
            }
            else SetPositionOfChData(pTIW->chDatas.begin() == iter ? nullptr : &*(--iter), true, Display);
        }

        void KeyboardButtonRightDown(bool needSubData)
        {
            if (pTIW->chDatas.empty()) return;     //  no where to move
            auto iter = pChData_pos ? ++std::find(pTIW->chDatas.begin(), pTIW->chDatas.end(), pChData_pos) : pTIW->chDatas.begin();     //  start from next itertor
            if (iter == pTIW->chDatas.end()) return;    //  no where to move
            if (ZC_Events::IsButtonPressed(K_LCTRL) || ZC_Events::IsButtonPressed(K_RCTRL))
            {
                bool char_reached = false;
                while (true)
                {
                    if (iter == pTIW->chDatas.end())
                    {
                        SetPositionOfChData(&*(--iter), needSubData, Display);  //  set caret on previous symbol
                        return;
                    }
                    else if (iter->pCh->symbol == L' ')  //  reached whitespace
                    {
                        if (char_reached)   //  reached whitespace after text
                        {
                            SetPositionOfChData(&*(--iter), needSubData, Display);  //  set caret on previous symbol
                            return;
                        }
                    }
                    else char_reached = true;
                    ++iter;
                }
            }
            else SetPositionOfChData(&*iter, needSubData, Display);
        }

        void BackspaceDown()
        {
            if (!pChData_pos) return;   //  caret on start
            auto iter = std::find(pTIW->chDatas.begin(), pTIW->chDatas.end(), pChData_pos);
            if (iter == pTIW->chDatas.begin())  //  caret on first symbol
            {
                int deleted_width = iter->pCh->width;
                iter = pTIW->chDatas.erase(iter);
                for (; iter != pTIW->chDatas.end(); ++iter) iter->start_index -= deleted_width;
                SetPositionOfChData(nullptr, true, Display);
                pTIW->textMut.UpdateText(pTIW->chDatas);
            }
            else if (ZC_Events::IsButtonPressed(K_LCTRL) || ZC_Events::IsButtonPressed(K_RCTRL))    //  dleete from caret to the begin of the word
            {
                auto last_iter = iter;
                bool text_reached = false;
                for (; ; --iter)    //  search white space
                {
                    if (text_reached && iter->pCh->symbol == L' ')  //  word's start reached
                    {
                        ++iter;
                        break;
                    }
                    else if (iter == pTIW->chDatas.begin()) break;   //  reached text's begin
                    else text_reached = true;
                }
                int deleted_width = (last_iter->start_index + last_iter->pCh->left_offset + last_iter->pCh->width) - iter->start_index;
                iter = pTIW->chDatas.erase(iter, ++last_iter);
                ZC_GUI_ChData* pChData_newCaretPos = nullptr;   //  new pos on start of text
                if (iter != pTIW->chDatas.begin())  //  if iterator not on start, set new position
                {
                    pChData_newCaretPos = &*(--iter);
                    ++iter;
                }
                for ( ; iter != pTIW->chDatas.end(); ++iter) iter->start_index -= deleted_width;
                SetPositionOfChData(pChData_newCaretPos, true, Display);
                pTIW->textMut.UpdateText(pTIW->chDatas);
            }
            else
            {
                int deleted_width = iter->pCh->left_offset + iter->pCh->width;
                iter = pTIW->chDatas.erase(iter);
                auto prev_iter = --iter;
                for (++iter; iter != pTIW->chDatas.end(); ++iter) iter->start_index -= deleted_width;
                SetPositionOfChData(&*prev_iter, true, Display);
                pTIW->textMut.UpdateText(pTIW->chDatas);
            }
        }

        void DeleteDown()
        {
            if (pTIW->chDatas.empty()) return;      //  text empty
            auto iter = pChData_pos ? std::find(pTIW->chDatas.begin(), pTIW->chDatas.end(), pChData_pos) : pTIW->chDatas.begin();
            if (++iter == pTIW->chDatas.end()) return;     //  go to the first element to be removed, if caret is at the last element, return
            
            int deleted_width = iter->pCh->left_offset + iter->pCh->width;
            if (ZC_Events::IsButtonPressed(K_LCTRL) || ZC_Events::IsButtonPressed(K_RCTRL))    //  dleete from caret to the end of the word
            {
                auto first_iter = iter;
                bool text_reached = false;
                for (++iter; iter != pTIW->chDatas.end(); ++iter)
                {
                    if (text_reached && iter->pCh->symbol == L' ') break;
                    else text_reached = true;
                    
                    deleted_width += iter->pCh->left_offset + iter->pCh->width;
                }
                iter = pTIW->chDatas.erase(first_iter, iter);
                for ( ; iter != pTIW->chDatas.end(); ++iter) iter->start_index -= deleted_width;
                SetDrawState(Display);      //   no need to change caret position just make not blicking
                pTIW->textMut.UpdateText(pTIW->chDatas);
            }
            else
            {
                iter = pTIW->chDatas.erase(iter);
                for ( ; iter != pTIW->chDatas.end(); ++iter) iter->start_index -= deleted_width;
                SetDrawState(Display);      //   no need to change caret position just make not blicking
                pTIW->textMut.UpdateText(pTIW->chDatas);
            }
        }

        void TextButtonDown()
        {
            const typename ZC_GUI_Font::Character* pCh = ZC_GUI_TextManager::pTM->font.GetCharacter(pTIW->eventHandler.wch_lastDown);
            if (!pCh) return;
            int add_width = (pChData_pos ? pCh->left_offset : 0) + pCh->width;  //  if caret at the begining of the text add left_offset
            if (!pTIW->chDatas.empty())     //  check buffer capasity
            {
                auto& first_element = pTIW->chDatas.front();
                auto& last_element = pTIW->chDatas.back();
                int current_width = (last_element.start_index + last_element.pCh->left_offset + last_element.pCh->width) - first_element.start_index;   //  dirty check. Don't check is first == last, and so on
                if (current_width + add_width > pTIW->textMut.GetWidth()) return;   //  reached texture capacity
            }
            if (pChData_pos)    //  caret in text
            {
                auto iter = std::find(pTIW->chDatas.begin(), pTIW->chDatas.end(), pChData_pos);
                ZC_GUI_ChData chData{ .pCh = pCh, .start_index = iter->start_index + iter->pCh->left_offset + iter->pCh->width };
                iter = pTIW->chDatas.emplace(++iter, chData);
                ZC_GUI_ChData* pCaret_pos = &*iter;
                for (++iter; iter != pTIW->chDatas.end(); ++iter) iter->start_index += add_width;
                SetPositionOfChData(pCaret_pos, true, Display);
                pTIW->textMut.UpdateText(pTIW->chDatas);
            }
            else    //  caret on texts start
            {
                auto iter = pTIW->chDatas.begin();
                ZC_GUI_ChData chData{ .pCh = pCh, .start_index = int(pTIW->GetPosition_bl_Obj()[0]) };
                iter = pTIW->chDatas.emplace(iter, chData);
                ZC_GUI_ChData* pCaret_pos = &*iter;
                for (++iter; iter != pTIW->chDatas.end(); ++iter) iter->start_index += add_width;
                SetPositionOfChData(pCaret_pos, true, Display);
                pTIW->textMut.UpdateText(pTIW->chDatas);
            }
        }

    private:
        void BlinkingUpdate(float time)
        {
            blinkTime += time;
            if (blinkTime >= blinkTime_limit)   //   time over
            {
                blinkTime -= blinkTime_limit;   //  update time
                ChangeObjsDrawState(!(VIsDrawing_Obj()), this, this);
            }
        }
    };
    
    ZC_GUI_TextInputWindow(float fontHeight);

    bool VIsInputWindow_W() const noexcept override;

    static bool StartInputWindow(float bl_x, float bl_y, int width, const std::wstring& wstr, ZC_Function<void(const std::wstring&)> _callBack)
    {
        if (pTIW->VIsDrawing_Obj() || !(pTIW->textMut.UpdateText(wstr))) return false;     //  allready drawing or wstr have too wide (in pixels)

        pTIW->funcChangedWstr = std::move(_callBack);
        pTIW->chDatasOffset = 0;

        pTIW->winEnd = bl_x + width;

            //  set bl
        *(pTIW->pBL) = { bl_x, bl_y };          //  set window position
        *(pTIW->highlight.pBL) = { bl_x, bl_y };          //  set highlight position
        *(pTIW->textMut.pBL) = { bl_x, bl_y };  //  set text position
        *(pTIW->caret.pBL) = { bl_x, bl_y };    //  set coret pos
            //  set border
        *(pTIW->pBorder) = { .bl{ bl_x, bl_y }, .tr{ bl_x + width, bl_y + pTIW->GetHeight() } };
        pTIW->VSubDataBorder_Obj(pTIW->pBorder, pTIW->pBorder);
            //  set width
        pTIW->pObjData->width = width;
        pTIW->VMapObjData_Obj(pTIW->pObjData, offsetof(ZC_GUI_ObjData, width), sizeof(ZC_GUI_ObjData::width), &(pTIW->pObjData->width));
        
        pTIW->startWString = wstr;
        pTIW->chDatas.clear();
        for (const wchar_t& wch : wstr)     //  fill chDatas
        {
            ZC_GUI_ChData& chPos =  pTIW->chDatas.emplace_back(ZC_GUI_ChData{ .pCh = ZC_GUI_TextManager::pTM->font.GetCharacter(wch), .start_index = int(bl_x) });
            bl_x += chPos.pCh->width;
            if (&wch != wstr.data()) bl_x += chPos.pCh->left_offset;    //   not first element add offset
        }
        pTIW->caret.SetPositionOfChData(pTIW->chDatas.empty() ? nullptr : &(pTIW->chDatas.back()), false, Caret::Blinking);  //  set caret position, must be before pTIW->VSubDataBL_Obj()
        pTIW->VSubDataBL_Obj(pTIW->pBL, pTIW->caret.pBL);     //  make subdata for all edited bls

        pTIW->VSetDrawState_W(true);

        ZC_SWindow::StartInputText();
        
        return true;
    }

    struct EventHandler
    {
        ZC_ButtonID buttonId_lastDown;  //  store last down button
        wchar_t wch_lastDown;   //  store last down unicode symbol

        const float waitPressNanoseconds = 300000000.f;   //  how long wait to start use VKeyboardButtonPressed_BK(), instead VKeyboardButtonDown_BK()
        const float beteenCallsWhenPressedNanoseconds = 100000000.f;   //  how long to wait between calls when the button is pressed
        float wait_time = 0.f;   //  current wait time
        bool buttonPressedStarted = false;

        bool isCampsLock = false;

        ZC_GUI_Obj* AddButtonDown(ZC_ButtonID buttonId)
        {
            if (!(pTIW->VIsDrawing_Obj())) return nullptr;
            buttonId_lastDown = buttonId;
            switch (buttonId)   //  process event wich closing window, other process in ButtonDown()
            {
            case M_LEFT:
            {
                if (!(pTIW->CheckCursorCollision_Obj()))  //  cursor out of window, don't use them and free ZC_GUI_TextInputWindow
                {
                    pTIW->WstrChanged();    //  user's callback (must be before pTIW->VSetDrawState_W())
                    pTIW->VSetDrawState_W(false);    //  stop drawing window
                }
                else pTIW->caret.SetPositionOfMouseCursor();
            } break;
            case M_RIGHT:
            {
                if (!(pTIW->CheckCursorCollision_Obj()))  //  cursor out window, don't use them and free ZC_GUI_TextInputWindow
                {
                    pTIW->WstrChanged();    //  user's callback (must be before pTIW->VSetDrawState_W())
                    pTIW->VSetDrawState_W(false);    //  stop drawing window
                }
            } break;
            case K_ESCAPE: pTIW->VSetDrawState_W(false); break;    //  don't change user's wstr, just stop drawing window
            case K_KP_ENTER:
            {
                pTIW->WstrChanged();    //  user's callback (must be before pTIW->VSetDrawState_W())
                pTIW->VSetDrawState_W(false);    //  stop drawing window
            } break;
            case K_LEFT: pTIW->highlight.KeyboardButtonLeftDown(); break;
            case K_RIGHT: pTIW->highlight.KeyboardButtonRightDown(); break;
            case K_BACKSPACE: pTIW->caret.BackspaceDown(); break;
            case K_DELETE: pTIW->caret.DeleteDown(); break;
            case K_CAPSLOCK: isCampsLock = !isCampsLock; break;
            default: break;
            }
                //  new button pressed, reset pressed button settings
            wait_time = 0.f;
            buttonPressedStarted = false;

            return pTIW;
        }

        void TextButtonDown(unsigned char ch)
        {
            wchar_t new_wch = GetUnicode(ch, buttonId_lastDown);
            if (new_wch == wch_lastDown) return;    //  allready pressed
            wch_lastDown = new_wch;
            pTIW->caret.TextButtonDown();   //  for input text button first call form here button down, not from AddButtonDown()
        }

        void ButtonDown(ZC_ButtonID buttonID, float time)
        {
            if (buttonID != buttonId_lastDown) return;

            switch (buttonID)
            {
            case K_LEFT: CallPressedButton(time, true, &ZC_GUI_TextInputWindow::Highlight::KeyboardButtonLeftDown); break;
            case K_RIGHT: CallPressedButton(time, false, &ZC_GUI_TextInputWindow::Highlight::KeyboardButtonRightDown); break;
            case K_BACKSPACE: CallPressedButton(time, false, &ZC_GUI_TextInputWindow::Highlight::BackspaceDown); break;
            case K_DELETE: CallPressedButton(time, false, &ZC_GUI_TextInputWindow::Highlight::DeleteDown); break;
            default: CallPressedButton(time, false, &ZC_GUI_TextInputWindow::Highlight::TextButtonDown); break;
            }
        }
      
        void ButtonUp(ZC_ButtonID buttonID, float time)
        {
            if (buttonId_lastDown == buttonID)
            {
                wait_time = 0.f;
                buttonPressedStarted = false;
                buttonId_lastDown = K_UNKNOWN;
                wch_lastDown = -1;
                if (pTIW->VIsDrawing_Obj()) pTIW->caret.SetDrawState(ZC_GUI_TextInputWindow::Caret::Blinking);
            }
        }

        void CallPressedButton(float time, bool isLeft, void(ZC_GUI_TextInputWindow::Highlight::* pFuncMove)())
        {
            wait_time += time;
            if (buttonPressedStarted)
            {
                if (wait_time >= beteenCallsWhenPressedNanoseconds)
                {
                    wait_time -= beteenCallsWhenPressedNanoseconds;
                    if (pFuncMove) (pTIW->highlight.*pFuncMove)();
                }
            }
            else
            {
                if (wait_time >= waitPressNanoseconds)
                {
                    wait_time -= waitPressNanoseconds;
                    if (pFuncMove) (pTIW->highlight.*pFuncMove)();
                    buttonPressedStarted = true;
                }
            }
        }

        wchar_t GetUnicode(unsigned char ch, ZC_ButtonID butID)
        {
            if (ch < 128) return ch;   //  return ch, is english ascii
            //     //  here must be if for keyboard, now next is russian. Letters have repeats in uchar (uchar Ё = c = 129), ё (uchar ё = Б = 145),
            //     //  and we don't know unicode thancs SDL... so use stupid tactic with ZC_ButtonID
            // if (ch == 129 && butID == ZC_ButtonID::K_GRAVE) return 0x401;    //  Ё (unicode 0x401)
            // else if (ch == 145 && butID == ZC_ButtonID::K_GRAVE) return 0x451;    //  ё (unicode 0x451)
            // else if (ch >= 128 && ch <= 143) return (wchar_t)ch + 960;  //  first range: 1088 - 128 = 960 -> 1088(0x440 unicode start), 128(uchar ascii start)
            // else if (ch >= 144 && ch <= 191) return (wchar_t)ch + 896;  //  second range: 1040 - 144 = 896 -> 1040(0x410 unicode start), 144(uchar ascii start)

            bool isCapital = isCampsLock || ZC_Events::IsButtonPressed(K_LSHIFT) || ZC_Events::IsButtonPressed(K_RSHIFT);

            switch (butID)
            {
            case K_Q: return isCapital ? L'Й' : L'й';
            case K_W: return isCapital ? L'Ц' : L'ц';
            case K_E: return isCapital ? L'У' : L'у';
            case K_R: return isCapital ? L'К' : L'к';
            case K_T: return isCapital ? L'Е' : L'е';
            case K_Y: return isCapital ? L'Н' : L'н';
            case K_U: return isCapital ? L'Г' : L'г';
            case K_I: return isCapital ? L'Ш' : L'ш';
            case K_O: return isCapital ? L'Щ' : L'щ';
            case K_P: return isCapital ? L'З' : L'з';
            case K_LEFTBRACKET: return isCapital ? L'Х' : L'х';
            case K_RIGHTBRACKET: return isCapital ? L'Ъ' : L'ъ';
            case K_A: return isCapital ? L'Ф' : L'ф';
            case K_S: return isCapital ? L'Ы' : L'ы';
            case K_D: return isCapital ? L'В' : L'в';
            case K_F: return isCapital ? L'А' : L'а';
            case K_G: return isCapital ? L'П' : L'п';
            case K_H: return isCapital ? L'Р' : L'р';
            case K_J: return isCapital ? L'О' : L'о';
            case K_K: return isCapital ? L'Л' : L'л';
            case K_L: return isCapital ? L'Д' : L'д';
            case K_SEMICOLON: return isCapital ? L'Ж' : L'ж';
            case K_APOSTROPHE: return isCapital ? L'Э' : L'э';
            case K_Z: return isCapital ? L'Я' : L'я';
            case K_X: return isCapital ? L'Ч' : L'ч';
            case K_C: return isCapital ? L'С' : L'с';
            case K_V: return isCapital ? L'М' : L'м';
            case K_B: return isCapital ? L'И' : L'и';
            case K_N: return isCapital ? L'Т' : L'т';
            case K_M: return isCapital ? L'Ь' : L'ь';
            case K_COMMA: return isCapital ? L'Б' : L'б';
            case K_PERIOD: return isCapital ? L'Ю' : L'ю';
            case K_GRAVE: return isCapital ? L'Ё' : L'ё';
            default: break;
            }
            assert(false);  //  can't find charracter
            return ch;
        }
    };

    ZC_GUI_Obj* VGetButtonKeyboard_W(ZC_ButtonID buttonId) override
    {
        return eventHandler.AddButtonDown(buttonId);
    }

    void VButtonDown_Obj(ZC_ButtonID buttonId, float time) override
    {
        eventHandler.ButtonDown(buttonId, time);
    }

    void VButtonUp_Obj(ZC_ButtonID buttonId, float time) override
    {
        eventHandler.ButtonUp(buttonId, time);
    }

    void TextButtonDown(unsigned char ch)
    {
        eventHandler.TextButtonDown(ch);
    }

private:
    static inline ZC_GUI_TextInputWindow* pTIW;
  
    EventHandler eventHandler;

    Highlight highlight;
    ZC_GUI_TextMutable textMut;
    Caret caret;
    ZC_Function<void(const std::wstring&)> funcChangedWstr;

    int winEnd = 0;
    std::wstring startWString;

    std::list<ZC_GUI_ChData> chDatas;
    int chDatasOffset = 0;    //  texture's offset in window (negative or 0)

    void VSetDrawState_W(bool needDraw) override
    {
        if (VIsDrawing_Obj() == needDraw) return;
        
        if (!needDraw)
        {
            highlight.DefaultState();
            caret.SetDrawState(ZC_GUI_TextInputWindow::Caret::DrawState::NotDisplay);    //  stop drawing caret (actualy no need to stop drawing caret, do this to stop calling (disconnect) caret blinking event)
            funcChangedWstr = nullptr;     //  free callback
            ZC_SWindow::StopInputText();
        }

        this->daic.instanceCount = needDraw ? 1 : 0;
        this->bufDAICs.GLMapNamedBufferRange_Write(this->daicOffset + offsetof(ZC_DrawArraysIndirectCommand, instanceCount),
            sizeof(ZC_DrawArraysIndirectCommand::instanceCount), &(this->daic.instanceCount));
    }

        //  call user's callback
    void WstrChanged()
    {
        ZC_DA<wchar_t> da_wch(chDatas.size());
        int da_wch_i = 0;
        for (ZC_GUI_ChData& chData : chDatas) da_wch[da_wch_i++] = chData.pCh->symbol;     //  fill da_wch
        funcChangedWstr(std::wstring(da_wch.Begin()));      //  create wstr and call callback
    }
};