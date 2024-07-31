#include <ZC/GUI/Text/ZC_GUI_TextInputWindow.h>

#include <ZC/GUI/ZC_GUI.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>
#include <ZC/Video/ZC_SWindow.h>
#include <ZC/Events/ZC_Events.h>
#include <ZC/GUI/ZC_GUI_IconUV.h>

bool ZC_GUI_TextInputWindow::StartInputWindow(float bl_x, float bl_y, int width, const std::wstring& wstr, ZC_Function<void(const std::wstring&)> _callBack)
{
    if (pTIW->VIsDrawing_Obj() || !(pTIW->textMut.UpdateText(wstr, true))) return false;     //  allready drawing or wstr have too wide (in pixels)

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
    pTIW->VSubDataBL_Obj(pTIW->Get_pBL_start(), pTIW->caret.VGet_pBL_end());     //  make subdata for all edited bls

    pTIW->VSetDrawState_W(true);

    ZC_SWindow::StartInputText();
    
    return true;
}

bool ZC_GUI_TextInputWindow::VIsInputWindow_W() const noexcept
{
    return true;
}

ZC_GUI_Obj* ZC_GUI_TextInputWindow::VGetButtonKeyboard_W(ZC_ButtonID buttonId)
{
    return eventHandler.AddButtonDown(buttonId);
}

void ZC_GUI_TextInputWindow::VButtonDown_Obj(ZC_ButtonID buttonId, float time)
{
    eventHandler.ButtonDown(buttonId, time);
}

void ZC_GUI_TextInputWindow::VButtonUp_Obj(ZC_ButtonID buttonId, float time)
{
    eventHandler.ButtonUp(buttonId, time);
}

void ZC_GUI_TextInputWindow::TextButtonDown(unsigned char ch)
{
    eventHandler.TextButtonDown(ch);
}

ZC_GUI_TextInputWindow::ZC_GUI_TextInputWindow(float fontHeight)
    : ZC_GUI_WinImmutable(ZC_WOIData(0.f, 0.f, 0.f, 0.f, ZC_WOIF__X_Left_Pixel | ZC_WOIF__Y_Bottom_Pixel), 0),
    highlight(fontHeight),
    textMut(L"", false, ZC_GUI_TextManager::pTM->font.GetLongestCharacterLength() * 200),     //  reserve place for 200 longest symbols (may be more if symbols are smaller)
    caret(fontHeight)
{
    this->pObjData->color = ZC_PackColorUCharToUInt(20, 20, 20);
    this->pObjData->depth = 0.f;    //  allways on first plane
    this->SetHeight_Obj(fontHeight);
    
    AddRow(Row({}, { &highlight, &textMut, &caret }));  //  must be correct order: highlight, textMut, caret

    pTIW = this;

    ZC_Events::ConnectWindowResize({ &ZC_GUI_TextInputWindow::WindowResize, this });
}

void ZC_GUI_TextInputWindow::VSetDrawState_W(bool needDraw)
{
    if (VIsDrawing_Obj() == needDraw) return;
    
    if (!needDraw)
    {
        highlight.SetDefaultState();
        caret.SetDrawState(ZC_GUI_TextInputWindow::Caret::DrawState::NotDisplay);    //  stop drawing caret (actualy no need to stop drawing caret, do this to stop calling (disconnect) caret blinking event)
        funcChangedWstr = nullptr;     //  free callback
        ZC_SWindow::StopInputText();
    }
    eventHandler.mbl_click = ZC_GUI_TextInputWindow::EventHandler::First;

    this->daic.instanceCount = needDraw ? 1 : 0;
    this->bufDAICs.GLMapNamedBufferRange_Write(this->daicOffset + offsetof(ZC_DrawArraysIndirectCommand, instanceCount),
        sizeof(ZC_DrawArraysIndirectCommand::instanceCount), &(this->daic.instanceCount));
}

void ZC_GUI_TextInputWindow::WstrChanged()
{
    std::wstring wstr;
    wstr.reserve(chDatas.size());
    for (ZC_GUI_ChData& chData : chDatas) wstr.append(1, chData.pCh->symbol);
    funcChangedWstr(wstr);
}

void ZC_GUI_TextInputWindow::WindowResize(float,float)
{
    VSetDrawState_W(false);
}


    //  Highlight

ZC_GUI_TextInputWindow::Highlight::Highlight(float fontHeight)
    : ZC_GUI_Obj(0.f, fontHeight, 0.f, ZC_PackColorUCharToUInt(0,0,0), ZC_GUI_IconUV::window, 0, ZC_GUI_Bindings::bind_tex_Icons)
{}

void ZC_GUI_TextInputWindow::Highlight::MBL_DoubleClick()
{
    if (pTIW->chDatas.empty()) return;

    auto lamb_findEnd_UpdateData = [this](typename std::list<ZC_GUI_ChData>::iterator iter, typename std::list<ZC_GUI_ChData>::iterator start_iter)
    {    //  find end
        bool find_whiteSpace = iter->pCh->symbol != L' ';   //  if caret on white space need find symbol, if on symbol need find white space
        auto end_iter = iter;
        for (++end_iter; ; ++end_iter)
        {
            if (end_iter == pTIW->chDatas.end() || (find_whiteSpace ? end_iter->pCh->symbol == L' ' : end_iter->pCh->symbol != L' '))
            {
                --end_iter;
                break;
            }
        }
        if (end_iter != iter) pTIW->caret.SetPositionOfChData(&*end_iter, false, ZC_GUI_TextInputWindow::Caret::Display);   //  new position for caret
            //  if highlight_start more left then window start, try correct offset
        float highlight_start = start_iter->start_index + pTIW->chDatasOffset;
        float highlight_end = pTIW->caret.Get_bl_Obj()[0] + pTIW->caret.VGetWidth_Obj();
        float win_start = pTIW->Get_bl_Obj()[0];
        float win_end = win_start + pTIW->VGetWidth_Obj();
        if (highlight_start < win_start && highlight_end < win_end)
        {
            float need_on_start = win_start - highlight_start;     //  need place at the start
            float reserve_on_end = win_end - highlight_end;    //  have place at the end
            float offset = need_on_start < reserve_on_end ? need_on_start : reserve_on_end;
            pTIW->chDatasOffset += offset;
            (*(pTIW->caret.pBL))[0] += offset;
            (*(pTIW->textMut.pBL))[0] += offset;
        }

            //  set highlight
        start_x = start_iter->start_index + pTIW->chDatasOffset;
        end_x = pTIW->caret.Get_bl_Obj()[0];
        UpdateData();
    };

    direction = Right;  //  double click and triple click is Right direction highlight

    if (pTIW->caret.pChData_pos)
    {
        auto iter = std::find(pTIW->chDatas.begin(), pTIW->chDatas.end(), pTIW->caret.pChData_pos);
            //  find start
        bool reached_symbol = false;
        auto start_iter = iter;
        for (; start_iter != pTIW->chDatas.begin(); --start_iter)   //  find word start
        {
            if (reached_symbol)
            {
                if (start_iter->pCh->symbol == L' ')
                {
                    ++start_iter;
                    break;
                }
            }
            else if (start_iter->pCh->symbol != L' ') reached_symbol = true;
        }
        lamb_findEnd_UpdateData(iter, start_iter);
    }
    else lamb_findEnd_UpdateData(pTIW->chDatas.begin(), pTIW->chDatas.begin());
}

void ZC_GUI_TextInputWindow::Highlight::MBL_TripleClick()
{
    if (pTIW->chDatas.empty()) return;
        //  highlight all text
    direction = Right;  //  double click and triple click is Right direction highlight
    pTIW->caret.SetPositionOfChData(&(pTIW->chDatas.back()), false, ZC_GUI_TextInputWindow::Caret::Display);    //  caret to end
    start_x = pTIW->chDatas.front().start_index + pTIW->chDatasOffset;
    end_x = pTIW->caret.Get_bl_Obj()[0];
    UpdateData();
}

void ZC_GUI_TextInputWindow::Highlight::KeyboardButtonLeftDown()
{
    if (!(pTIW->caret.pChData_pos)) return;     //  already at left
    if (ZC_Events::IsButtonPressed(K_LSHIFT) || ZC_Events::IsButtonPressed(K_RSHIFT))   //  uses highlight
    {
        switch (direction)
        {
        case None:
        {
            end_x = pTIW->caret.Get_bl_Obj()[0];
            int temp_offset_x = pTIW->chDatasOffset;
            pTIW->caret.KeyboardButtonLeftDown(false);     //  all calculations with pTIW->chDatasOffset must be after caret move
            if (temp_offset_x != pTIW->chDatasOffset) end_x += - temp_offset_x + pTIW->chDatasOffset;
            start_x = pTIW->caret.Get_bl_Obj()[0];    //  start_x equal carets bl.x
            UpdateData();
            direction = Left;
        } break;
        case Left:
        {
            int temp_offset_x = pTIW->chDatasOffset;
            pTIW->caret.KeyboardButtonLeftDown(false);     //  all calculations with pTIW->chDatasOffset must be after caret move
            if (temp_offset_x != pTIW->chDatasOffset) end_x += - temp_offset_x + pTIW->chDatasOffset;
            start_x = pTIW->caret.Get_bl_Obj()[0];    //  start_x equal carets bl.x
            UpdateData();
        } break;
        case Right:
        {
            int temp_offset_x = pTIW->chDatasOffset;
            pTIW->caret.KeyboardButtonLeftDown(true);     //  all calculations with pTIW->chDatasOffset must be after caret move
            if (temp_offset_x != pTIW->chDatasOffset) start_x += - temp_offset_x + pTIW->chDatasOffset;  //  correct start_x if offset changed
            float caret_pos_x = pTIW->caret.Get_bl_Obj()[0];    //  start_x equal carets bl.x
            if (caret_pos_x > start_x)    //  direction still Right
            {
                end_x = caret_pos_x;
                this->pObjData->width = end_x - start_x;
                VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), sizeof(ZC_GUI_ObjData::width), &(this->pObjData->width));
            }
            else if (caret_pos_x == start_x) SetDefaultState();    //  start_x = end_x, no highlight
            else    //  direction changed to Left
            {   //  change places start_x and end_x
                end_x = start_x;
                start_x = caret_pos_x;
                UpdateData();
                direction = Left;
            }
        } break;
        }
    }
    else    //  don't uses highlight
    {
        SetDefaultState();
        pTIW->caret.KeyboardButtonLeftDown(true);
    }
}

void ZC_GUI_TextInputWindow::Highlight::KeyboardButtonRightDown()
{
    if (pTIW->chDatas.empty()) return;     //  no where to move
    if (ZC_Events::IsButtonPressed(K_LSHIFT) || ZC_Events::IsButtonPressed(K_RSHIFT))   //  uses highlight
    {
        switch (direction)
        {
        case None:
        {
            start_x = pTIW->caret.Get_bl_Obj()[0];    //  start_x equal carets bl.x
            int temp_offset_x = pTIW->chDatasOffset;
            pTIW->caret.KeyboardButtonRightDown(false);     //  all calculations with pTIW->chDatasOffset must be after caret move
            if (temp_offset_x != pTIW->chDatasOffset) start_x += - temp_offset_x + pTIW->chDatasOffset;
            end_x = pTIW->caret.Get_bl_Obj()[0];    //  end_x equal carets bl.x
            UpdateData();
            direction = Right;
        } break;
        case Left:
        {
            int temp_offset_x = pTIW->chDatasOffset;
            pTIW->caret.KeyboardButtonRightDown(false);     //  all calculations with pTIW->chDatasOffset must be after caret move
            if (temp_offset_x != pTIW->chDatasOffset) end_x += - temp_offset_x + pTIW->chDatasOffset;
            float caret_pos_x = pTIW->caret.Get_bl_Obj()[0];    //  equal carets bl.x
            if (caret_pos_x < end_x)    //  direction still Left
            {
                start_x = caret_pos_x;
                UpdateData();
            }
            else if (caret_pos_x == start_x) SetDefaultState();    //  start_x = end_x, no highlight
            else    //  direction changed to Right
            {
                start_x = end_x;
                end_x = caret_pos_x;
                UpdateData();
                direction = Right;
            }
        } break;
        case Right:
        {
            int temp_offset_x = pTIW->chDatasOffset;
            pTIW->caret.KeyboardButtonRightDown(false);     //  all calculations with pTIW->chDatasOffset must be after caret move
            if (temp_offset_x != pTIW->chDatasOffset) start_x += - temp_offset_x + pTIW->chDatasOffset;
            end_x = pTIW->caret.Get_bl_Obj()[0];    //  end_x equal carets bl.x
            UpdateData();
        } break;
        }
    }
    else    //  don't uses highlight
    {
        SetDefaultState();
        pTIW->caret.KeyboardButtonRightDown(true);
    }
}

void ZC_GUI_TextInputWindow::Highlight::BackspaceDown()
{
    IsHighLight() ? DeleteHighlight(true) : pTIW->caret.BackspaceDown();   //  if have hightlight delete, otherwise call caret
}

void ZC_GUI_TextInputWindow::Highlight::DeleteDown()
{
    IsHighLight() ? DeleteHighlight(true) : pTIW->caret.DeleteDown();   //  if have hightlight delete, otherwise call caret
}

void ZC_GUI_TextInputWindow::Highlight::TextButtonDown()
{
    const typename ZC_GUI_Font::Character* pCh = ZC_GUI_TextManager::pTM->font.GetCharacter(pTIW->eventHandler.wch_lastDown);
    if (!pCh) return;
    if (IsHighLight()) DeleteHighlight(false);
    pTIW->caret.TextButtonDown(pCh);
}

bool ZC_GUI_TextInputWindow::Highlight::IsHighLight()
{
    return start_x != end_x;
}

void ZC_GUI_TextInputWindow::Highlight::SetDefaultState()
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

void ZC_GUI_TextInputWindow::Highlight::UpdateData()
{
        //  update bls
    (*this->pBL)[0] = start_x;
    VSubDataBL_Obj(Get_pBL_start(), pTIW->caret.VGet_pBL_end());     //  update bls: hilight, textMut, caret
        //  update width
    this->pObjData->width = end_x - start_x;
    VMapObjData_Obj(this->pObjData, offsetof(ZC_GUI_ObjData, width), sizeof(ZC_GUI_ObjData::width), &(this->pObjData->width));
}

void ZC_GUI_TextInputWindow::Highlight::DeleteHighlight(bool needSubData)
{
    typename std::list<ZC_GUI_ChData>::iterator start_iter;
    typename std::list<ZC_GUI_ChData>::iterator end_iter = pTIW->chDatas.end();
    int delete_width = 0;
    for (auto iter = pTIW->chDatas.begin(); iter != pTIW->chDatas.end(); ++iter)
    {
        if (delete_width != 0)  //  start_iter and end_iter found, delete them width from rest symbols 
        {
            iter->start_index -= delete_width;
            continue;
        }
        if (iter->start_index + pTIW->chDatasOffset == start_x) start_iter = iter;
        else if (iter->start_index + pTIW->chDatasOffset == end_x)
        {
            end_iter = iter;
            delete_width = end_iter->start_index - start_iter->start_index;
            end_iter->start_index -= delete_width;
        }
    }

    ZC_GUI_ChData* pChData_caret = nullptr;
    if (direction == Right && start_iter != pTIW->chDatas.begin())  //  prepare data for caret position (need only if directin Right)
    {
        std::list<ZC_GUI_ChData>::iterator iter_caret = start_iter;
        pChData_caret = &*(--iter_caret);
    }
    
    pTIW->chDatas.erase(start_iter, end_iter);
    pTIW->textMut.UpdateText(pTIW->chDatas);
    
    if (direction == Right) pTIW->caret.SetPositionOfChData(pChData_caret, needSubData, ZC_GUI_TextInputWindow::Caret::Display);   //  need move caret
    SetDefaultState();
}


    //  Caret

ZC_GUI_TextInputWindow::Caret::Caret(float fontHeight)
    : ZC_GUI_Obj(1.f, fontHeight, 0.f, ZC_PackColorUCharToUInt(150,150,150), ZC_GUI_IconUV::window, 0, ZC_GUI_Bindings::bind_tex_Icons)
{}

void ZC_GUI_TextInputWindow::Caret::SetDrawState(DrawState _drawState)
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
        if (VIsDrawing_Obj()) VSetDrawState_Obj(false, true);   //  stop draw
    } break;
    case Display:
    {
        ecBlinkingUpdate.Disconnect();  //  disconnect update if was blinking
        if (!VIsDrawing_Obj()) VSetDrawState_Obj(true, true);   //  start draw
    } break;
    }
}

void ZC_GUI_TextInputWindow::Caret::SetPositionOfChData(ZC_GUI_ChData* _pChData_pos, bool needSubData, DrawState _drawState)
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
            (*(pTIW->textMut.pBL))[0] = pTIW->Get_bl_Obj()[0] + pTIW->chDatasOffset;   //  move texture X left
            texturePosChanged = true;
            (*(pBL))[0] = pTIW->winEnd - this->pObjData->width;  //  caret before win end
        }
        else if (caretPos_start < int(pTIW->Get_bl_Obj()[0]))   //  caret start before window start, need move right texture
        {
            pTIW->chDatasOffset += int(pTIW->Get_bl_Obj()[0]) - caretPos_start;     //  move offset to right
            (*(pTIW->textMut.pBL))[0] = pTIW->Get_bl_Obj()[0] + pTIW->chDatasOffset;   //  move texture X left
            texturePosChanged = true;
            (*(pBL))[0] = pTIW->Get_bl_Obj()[0];     //  caret on win start
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

    if (needSubData) VSubDataBL_Obj(texturePosChanged ? pTIW->textMut.Get_pBL_start() : Get_pBL_start(), VGet_pBL_end());   //  if texture pos was changed, start subData from texture, otherwise only Caret (pBL)
    SetDrawState(_drawState);
}

void ZC_GUI_TextInputWindow::Caret::SetPositionOfMouseCursor()
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

void ZC_GUI_TextInputWindow::Caret::KeyboardButtonLeftDown(bool needSubData)
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

void ZC_GUI_TextInputWindow::Caret::KeyboardButtonRightDown(bool needSubData)
{
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

void ZC_GUI_TextInputWindow::Caret::BackspaceDown()
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

void ZC_GUI_TextInputWindow::Caret::DeleteDown()
{
    if (pTIW->chDatas.empty()) return;      //  text empty
    auto iter = pChData_pos ? ++(std::find(pTIW->chDatas.begin(), pTIW->chDatas.end(), pChData_pos)) : pTIW->chDatas.begin();     //  go to the first element to be removed
    if (iter == pTIW->chDatas.end()) return;     //  if caret is at the last element, return
    
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

void ZC_GUI_TextInputWindow::Caret::TextButtonDown(const typename ZC_GUI_Font::Character* pCh)
{
    int add_width = (pChData_pos ? pCh->left_offset : 0) + pCh->width;  //  if caret at the begining of the text add left_offset
    if (!pTIW->chDatas.empty())     //  check buffer capasity
    {
        auto& first_element = pTIW->chDatas.front();
        auto& last_element = pTIW->chDatas.back();
        int current_width = (last_element.start_index + last_element.pCh->left_offset + last_element.pCh->width) - first_element.start_index;   //  dirty check. Don't check is first == last, and so on
        if (current_width + add_width > pTIW->textMut.VGetWidth_Obj()) return;   //  reached texture capacity
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
        ZC_GUI_ChData chData{ .pCh = pCh, .start_index = int(pTIW->Get_bl_Obj()[0]) };
        iter = pTIW->chDatas.emplace(iter, chData);
        ZC_GUI_ChData* pCaret_pos = &*iter;
        for (++iter; iter != pTIW->chDatas.end(); ++iter) iter->start_index += add_width;
        SetPositionOfChData(pCaret_pos, true, Display);
        pTIW->textMut.UpdateText(pTIW->chDatas);
    }
}

void ZC_GUI_TextInputWindow::Caret::BlinkingUpdate(float time)
{
    blinkTime += time;
    if (blinkTime >= blinkTime_limit)   //   time over
    {
        blinkTime -= blinkTime_limit;   //  update time
        VSetDrawState_Obj(!(VIsDrawing_Obj()), true);
    }
}


    //  EventHandler

ZC_GUI_Obj* ZC_GUI_TextInputWindow::EventHandler::AddButtonDown(ZC_ButtonID buttonId)
{
    if (!(pTIW->VIsDrawing_Obj())) return nullptr;
    buttonId_lastDown = buttonId;

    auto lamb_closeWindow = []()
    {
        pTIW->WstrChanged();    //  user's callback (must be before pTIW->VSetDrawState_W())
        pTIW->VSetDrawState_W(false);    //  stop drawing window
    };

    switch (buttonId)   //  process event wich closing window, other process in ButtonDown()
    {
    case M_LEFT:
    {
        if (!(pTIW->CheckCursorCollision_Obj())) lamb_closeWindow();    //  cursor out of window, don't use them and free ZC_GUI_TextInputWindow
        else
        {
            auto lamb_oneClick = []()
            {
                pTIW->caret.SetPositionOfMouseCursor();
                pTIW->highlight.SetDefaultState();
            };
            switch (mbl_click)
            {
            case First:
            {
                mbl_click = Double;
                lamb_oneClick();
                clock_click.Start();
            } break;
            case Double:
            {
                if (clock_click.Restart<ZC_Nanoseconds>() <= waitPressNanoseconds)  //  managed to double click
                {
                    mbl_click = Triple;
                    pTIW->highlight.MBL_DoubleClick();
                }
                else lamb_oneClick();
            } break;
            case Triple:
            {
                mbl_click = Double;
                clock_click.Restart<ZC_Nanoseconds>() <= waitPressNanoseconds ? pTIW->highlight.MBL_TripleClick() : lamb_oneClick();  //  Tiple click or back to First click
            } break;
            }
        }
    } break;
    case M_RIGHT: { if (!(pTIW->CheckCursorCollision_Obj())) lamb_closeWindow(); } break;    //  cursor out window, don't use them and free ZC_GUI_TextInputWindow
    case K_ESCAPE: pTIW->VSetDrawState_W(false); break;    //  don't change user's wstr, just stop drawing window
    case K_KP_ENTER: lamb_closeWindow(); break;
    case K_RETURN: lamb_closeWindow(); break;
    case K_LEFT: pTIW->highlight.KeyboardButtonLeftDown(); break;
    case K_RIGHT: pTIW->highlight.KeyboardButtonRightDown(); break;
    case K_BACKSPACE: pTIW->highlight.BackspaceDown(); break;
    case K_DELETE: pTIW->highlight.DeleteDown(); break;
    case K_CAPSLOCK: isCampsLock = !isCampsLock; break;
    default: break;
    }
        //  new button pressed, reset pressed button settings
    wait_time = 0.f;
    buttonPressedStarted = false;

    return pTIW;
}

void ZC_GUI_TextInputWindow::EventHandler::TextButtonDown(unsigned char ch)
{
    wchar_t new_wch = GetUnicode(ch, buttonId_lastDown);
    if (new_wch == wch_lastDown) return;    //  allready pressed
    wch_lastDown = new_wch;
    pTIW->highlight.TextButtonDown();   //  for input text button first call form here button down, not from AddButtonDown()
}

void ZC_GUI_TextInputWindow::EventHandler::ButtonDown(ZC_ButtonID buttonID, float time)
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

void ZC_GUI_TextInputWindow::EventHandler::ButtonUp(ZC_ButtonID buttonID, float time)
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

void ZC_GUI_TextInputWindow::EventHandler::CallPressedButton(float time, bool isLeft, void(ZC_GUI_TextInputWindow::Highlight::* pFuncMove)())
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

wchar_t ZC_GUI_TextInputWindow::EventHandler::GetUnicode(unsigned char ch, ZC_ButtonID butID)
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