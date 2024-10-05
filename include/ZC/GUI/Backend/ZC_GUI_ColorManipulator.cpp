#include "ZC_GUI_ColorManipulator.h"

#include <ZC/GUI/Backend/Config/ZC_GUI_Bindings.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_IconUV.h>
#include <ZC/Video/ZC_SWindow.h>

ZC_GUI_ColorManipulator::ZC_GUI_ColorManipulator(ZC_Function<void(float,float,float,float)>&& _callback, bool _range_255)
    : ZC_GUI_ObjComposite(ZC_GUI_ObjData(0.f, 20.f, 0, ZC_GUI_IconUV::quad_colored, ZC_GUI_Bindings::bind_tex_Icons)),      //  wisth sets in VSet_pBL_Obj()
    range_255(_range_255),
    cursor_color_line(ZC_GUI_ObjData(2.f, this->GetHeight(), 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons)),
    result_triangle(ZC_GUI_ObjData(GetTriangleSize(), GetTriangleSize(), ZC_PackColorUcharToUInt_RGBA(255, 0, 0, 255), {}, ZC_GUI_Bindings::bind_ColorManipulator_resultTriangle)),
    alpha_triangle(ZC_GUI_ObjData(result_triangle.VGetWidth_Obj(), result_triangle.GetHeight(), 0, ZC_GUI_IconUV::background_alpha, ZC_GUI_Bindings::bind_ColorManipulator_alphaTrinalge)),
    saturation_triangle(GetTriangleSize()),
    bnt_red_uchar(ZC_GUI_ButtonNumber<uchar>(button_width, 0.f, 255, 0, 255, 1, 3, 0, ZC_GUI_TextAlignment::Center, nullptr, nullptr),
        ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(GetDistance(L"R"), ZC_GUI_TFB_Indent::OutOfButtonLeft), L"R", true, 0, ZC_GUI_TextAlignment::Left)),
    bnt_green_uchar(ZC_GUI_ButtonNumber<uchar>(button_width, 0.f, 0, 0, 255, 1, 3, 0, ZC_GUI_TextAlignment::Center, nullptr, nullptr),
        ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(GetDistance(L"G"), ZC_GUI_TFB_Indent::OutOfButtonLeft), L"G", true, 0, ZC_GUI_TextAlignment::Left)),
    bnt_blue_uchar(ZC_GUI_ButtonNumber<uchar>(button_width, 0.f, 0, 0, 255, 1, 3, 0, ZC_GUI_TextAlignment::Center, nullptr, nullptr),
        ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(GetDistance(L"B"), ZC_GUI_TFB_Indent::OutOfButtonLeft), L"B", true, 0, ZC_GUI_TextAlignment::Left)),
    bnt_red_float(ZC_GUI_ButtonNumber<float>(button_width, 0.f, 1.f, 0.f, 1.f, 0.01f, 0.05f, 2, ZC_GUI_TextAlignment::Center, nullptr, nullptr),
        ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(GetDistance(L"R"), ZC_GUI_TFB_Indent::OutOfButtonLeft), L"R", true, 0, ZC_GUI_TextAlignment::Left)),
    bnt_green_float(ZC_GUI_ButtonNumber<float>(button_width, 0.f, 0.f, 0.f, 1.f, 0.01f, 0.05f, 2, ZC_GUI_TextAlignment::Center, nullptr, nullptr),
        ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(GetDistance(L"G"), ZC_GUI_TFB_Indent::OutOfButtonLeft), L"G", true, 0, ZC_GUI_TextAlignment::Left)),
    bnt_blue_float(ZC_GUI_ButtonNumber<float>(button_width, 0.f, 0.f, 0.f, 1.f, 0.01f, 0.05f, 2, ZC_GUI_TextAlignment::Center, nullptr, nullptr),
        ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(GetDistance(L"B"), ZC_GUI_TFB_Indent::OutOfButtonLeft), L"B", true, 0, ZC_GUI_TextAlignment::Left)),
    bnt_alpha(ZC_GUI_ButtonNumber<float>(button_width, 0.f, 1.f, 0.f, 1.f, 0.01f, 0.05f, 2, ZC_GUI_TextAlignment::Center, nullptr, nullptr),
        ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(GetDistance(L"A"), ZC_GUI_TFB_Indent::OutOfButtonLeft), L"A", true, 0, ZC_GUI_TextAlignment::Left)),
    bmt_uchar(bnt_red_uchar.VGetWidthComposite_Obj() / 2.f, true),
    bmt_float(bnt_red_uchar.VGetWidthComposite_Obj() / 2.f, false),
    callback(std::move(_callback))
{}

ZC_GUI_ColorManipulator::ZC_GUI_ColorManipulator(ZC_GUI_ColorManipulator&& cm)
    : ZC_GUI_ObjComposite(static_cast<ZC_GUI_ObjComposite&&>(cm)),
    isMBL_pressed(cm.isMBL_pressed),
    range_255(cm.range_255),
    cursor_color_line(std::move(cm.cursor_color_line)),
    result_triangle(std::move(cm.result_triangle)),
    alpha_triangle(std::move(cm.alpha_triangle)),
    saturation_triangle(std::move(cm.saturation_triangle)),
    bnt_red_uchar(std::move(cm.bnt_red_uchar)),
    bnt_green_uchar(std::move(cm.bnt_green_uchar)),
    bnt_blue_uchar(std::move(cm.bnt_blue_uchar)),
    bnt_red_float(std::move(cm.bnt_red_float)),
    bnt_green_float(std::move(cm.bnt_green_float)),
    bnt_blue_float(std::move(cm.bnt_blue_float)),
    bnt_alpha(std::move(cm.bnt_alpha)),
    bmt_uchar(std::move(cm.bmt_uchar)),
    bmt_float(std::move(cm.bmt_float)),
    callback(std::move(cm.callback))
{}

void ZC_GUI_ColorManipulator::SetAlpha(float alpha, bool use_callback)
{
    bnt_alpha.SetNumber(alpha, false);
    ChangedAlphaInButton(alpha, use_callback);
}

float ZC_GUI_ColorManipulator::GetDistance(const std::wstring& rgba)
{
    static float dist_R = 0.f;
    static float dist_G = 0.f;
    static float dist_B = 0.f;
    static float dist_A = 0.f;

    if (dist_R == 0.f)
    {
        float distance = ZC_GUI_TextManager::GetFontHeight() / 10.f;     //  distance between letter and button (left arrow)
        
        const ZC_GUI_Font::Character* chs[] { ZC_GUI::pGUI->textManager.font.GetCharacter(L'R'), ZC_GUI::pGUI->textManager.font.GetCharacter(L'G'),
            ZC_GUI::pGUI->textManager.font.GetCharacter(L'B'), ZC_GUI::pGUI->textManager.font.GetCharacter(L'A'),};
        float max_width = 0.f;
        for (auto pCh : chs) if (pCh->width > max_width) max_width = pCh->width;

        dist_R = distance + ((max_width - chs[0]->width) / 2.f);
        dist_G = distance + ((max_width - chs[1]->width) / 2.f);
        dist_B = distance + ((max_width - chs[2]->width) / 2.f);
        dist_A = distance + ((max_width - chs[3]->width) / 2.f);
    }

    return rgba == L"R" ? dist_R
        : rgba == L"G" ? dist_G
        : rgba == L"B" ? dist_B
        : dist_A;
}

void ZC_GUI_ColorManipulator::AddObjs()
{
    this->VAddObj_Obj(&cursor_color_line);
    this->VAddObj_Obj(&alpha_triangle);     //  must be added before result_triangle (draw order)
    this->VAddObj_Obj(&result_triangle);        //  must be added before saturation_trinagle, for color update(UpdateSaturation())
    this->VAddObj_Obj(&saturation_triangle);    //  must be added after result_trianlge, for color update (UpdateSaturation())
        //  uchar buttons
    this->VAddObj_Obj(&bnt_red_uchar);
    this->VAddObj_Obj(&bnt_green_uchar);
    this->VAddObj_Obj(&bnt_blue_uchar);
        //  float buttons
    this->VAddObj_Obj(&bnt_red_float);
    this->VAddObj_Obj(&bnt_green_float);
    this->VAddObj_Obj(&bnt_blue_float);
    bnt_red_float.VSetDrawState_Obj(false, false);
    bnt_green_float.VSetDrawState_Obj(false, false);
    bnt_blue_float.VSetDrawState_Obj(false, false);
        //  switch type buttons (must be added after buttons for pObjData update in ChangeButtonsType())
    this->VAddObj_Obj(&bmt_uchar);
    this->VAddObj_Obj(&bmt_float);
    bmt_uchar.SetColor_Obj(bmt_uchar.colorsButton.color_button_pressed, false);

    this->VAddObj_Obj(&bnt_alpha);

    bnt_red_uchar.UpdateCallbacks(ZC_Function<void(uchar)>(&ZC_GUI_ColorManipulator::Callback_uchar, this), nullptr);
    bnt_green_uchar.UpdateCallbacks(ZC_Function<void(uchar)>(&ZC_GUI_ColorManipulator::Callback_uchar, this), nullptr);
    bnt_blue_uchar.UpdateCallbacks(ZC_Function<void(uchar)>(&ZC_GUI_ColorManipulator::Callback_uchar, this), nullptr);
    bnt_red_float.UpdateCallbacks(ZC_Function<void(float)>(&ZC_GUI_ColorManipulator::Callback_float, this), nullptr);
    bnt_green_float.UpdateCallbacks(ZC_Function<void(float)>(&ZC_GUI_ColorManipulator::Callback_float, this), nullptr);
    bnt_blue_float.UpdateCallbacks(ZC_Function<void(float)>(&ZC_GUI_ColorManipulator::Callback_float, this), nullptr);
    bnt_alpha.UpdateCallbacks(ZC_Function<void(float)>(&ZC_GUI_ColorManipulator::Callback_alpha, this), nullptr);
}

float ZC_GUI_ColorManipulator::VGetHeightComposite_Obj()
{
    return this->GetHeight() + (distance_buttons_y * lines_with_buttons) + (bnt_red_uchar.GetHeight() * lines_with_buttons);
}

float ZC_GUI_ColorManipulator::VGetWidthComposite_Obj()
{       //  composite width takes after VSet_pBL_Obj, so can be calculated from positions
                                    //  right_x (button's bl + width + right arrow width (equal font height))   minus  leftest left_x
    return (bnt_red_uchar.Get_bl_Obj()[0] + bnt_red_uchar.VGetWidth_Obj() + ZC_GUI_TextManager::GetFontHeight()) - result_triangle.Get_bl_Obj()[0];
}

void ZC_GUI_ColorManipulator::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    AddObjs();  //  call here to avoid calls in move ctr

           //  bls sets from buttom to top
    alpha_triangle.VSet_pBL_Obj(_bl);   //  drawn under result_triangle
    result_triangle.VSet_pBL_Obj(_bl);
    saturation_triangle.VSet_pBL_Obj({ _bl[0] + 5.f, _bl[1] });     //  4 is distance between result_triangle and saturation_triangle
        //  distances from saturation triangle to buttons, need calculate to have button at same vertical line, caurse text letters R,G,B,A have different pixel width
    static float dist_r_x = 0.f;
    static float dist_g_x = 0.f;
    static float dist_b_x = 0.f;
    static float dist_a_x = 0.f;
    if (dist_r_x == 0.f)
    {
        float widths[] { bnt_red_uchar.VGetWidthComposite_Obj(), bnt_green_uchar.VGetWidthComposite_Obj(), bnt_blue_uchar.VGetWidthComposite_Obj(),
            bnt_alpha.VGetWidthComposite_Obj() };
        float max_width = 0.f;
        for (float width : widths) if (width > max_width) max_width = width;

        float distance_x = ZC_GUI_TextManager::GetFontHeight() / 1.5f;  //  distance between saturation triangle and buttons
        dist_r_x = distance_x + (max_width - widths[0]);
        dist_g_x = distance_x + (max_width - widths[1]);
        dist_b_x = distance_x + (max_width - widths[2]);
        dist_a_x = distance_x + (max_width - widths[3]);
    }

    float buttons_start_x = saturation_triangle.Get_bl_Obj()[0] + saturation_triangle.VGetWidth_Obj();

    bnt_alpha.VSet_pBL_Obj({ buttons_start_x + dist_a_x, _bl[1] });

    float next_y = _bl[1] + bnt_alpha.GetHeight() + distance_buttons_y;
    bmt_uchar.VSet_pBL_Obj({ buttons_start_x + dist_r_x, next_y });
    bmt_float.VSet_pBL_Obj({ bmt_uchar.Get_bl_Obj()[0] + bnt_alpha.VGetWidthComposite_Obj() - bmt_float.VGetWidth_Obj(), next_y });     //  on same line as a bmt_uchar, righter

    next_y += bmt_uchar.GetHeight() + distance_buttons_y;
    bnt_blue_uchar.VSet_pBL_Obj({ buttons_start_x + dist_b_x, next_y });
    bnt_blue_float.VSet_pBL_Obj({ buttons_start_x + dist_b_x, next_y });
    
    next_y += bnt_blue_uchar.GetHeight() + distance_buttons_y;
    bnt_green_uchar.VSet_pBL_Obj({ buttons_start_x + dist_g_x, next_y });
    bnt_green_float.VSet_pBL_Obj({ buttons_start_x + dist_g_x, next_y });
    
    next_y += bnt_green_uchar.GetHeight() + distance_buttons_y;
    bnt_red_uchar.VSet_pBL_Obj({ buttons_start_x + dist_r_x, next_y });
    bnt_red_float.VSet_pBL_Obj({ buttons_start_x + dist_r_x, next_y });

    *(this->pBL) = { _bl[0], next_y + bnt_red_uchar.GetHeight() + distance_buttons_y };

    SetCursorColorLinePosition(this->Get_bl_Obj(), false);      //  sets cursor_color_line bl

    this->pObjData->width = this->VGetWidthComposite_Obj();
}

bool ZC_GUI_ColorManipulator::VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept
{
    return true;
}

bool ZC_GUI_ColorManipulator::VIsUseScrollEvent_Obj() const noexcept
{
    return true;
}

bool ZC_GUI_ColorManipulator::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)
{
    return this->MakeCursorCollision_Obj(x, y, rpObj, rpScroll)
        || (IsActiveUCharButtons() ? (bnt_red_uchar.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)     //  uses uchar buttons
                                        || bnt_green_uchar.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)
                                        || bnt_blue_uchar.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll))
                                    : (bnt_red_float.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)    //  uses float buttons
                                        || bnt_green_float.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)
                                        || bnt_blue_float.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)))
        || bmt_uchar.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)
        || bmt_float.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)
        || bnt_alpha.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll)
        || saturation_triangle.VMakeCursorCollision_Obj(x, y, rpObj, rpScroll);
}

bool ZC_GUI_ColorManipulator::VMouseButtonLeftDown_Obj(float time)
{
    if (isMBL_pressed) return false;
    isMBL_pressed = true;
    
    float mouse_x, mouse_y;
    ZC_SWindow::GetCursorPosition(mouse_x, mouse_y);
    ZC_Vec2<float> cursor_color_line_pos = GetCursorColorLinePosition();
    if (cursor_color_line_pos[0] == mouse_x) return true;  //  mouse cursor pos_x equal color line cursor pos_x

    UpdateColorLineCursorPosition({ mouse_x, cursor_color_line_pos[1] });

    return true;
}

void ZC_GUI_ColorManipulator::VCursorMove_Obj(float rel_x, float rel_y)
{
    if (rel_x == 0.f) return;   //  interested only move X
    ZC_Vec2<float> cursor_color_line_pos = GetCursorColorLinePosition();
    float cursor_x = cursor_color_line_pos[0] + rel_x;
    float color_line_start_x = this->Get_bl_Obj()[0];
    float color_line_end_x = color_line_start_x + this->VGetWidth_Obj();
    if (cursor_x < color_line_start_x) cursor_x = color_line_end_x - (color_line_start_x - cursor_x);
    else if (cursor_x > color_line_end_x) cursor_x = color_line_start_x + (cursor_x - color_line_end_x);
    UpdateColorLineCursorPosition({ cursor_x, cursor_color_line_pos[1] });
}

void ZC_GUI_ColorManipulator::VMouseButtonLeftUp_Obj(float time)
{
    isMBL_pressed = false;
}

void ZC_GUI_ColorManipulator::UpdateColorLineCursorPosition(const ZC_Vec2<float>& cursor_color_line_pos)
{
    float color_line_pos_x = cursor_color_line_pos[0] - this->Get_bl_Obj()[0];     //  offset from color line start to cursor pos
    static const float rbgr_step = this->VGetWidth_Obj() / 3.f;    //  3 distances between r-b-g-r on color line (as round)
    static const float rpbager_step = rbgr_step / 2.f;    //  distances between red-pink-blue-azure-green-yellow-red on color line (as round)

    auto lambUpdateData = [this, &cursor_color_line_pos](float r, float g, float b)
    {
        SetCursorColorLinePosition(cursor_color_line_pos, true);
        saturation_triangle.SetColorTR(r, g, b);    //  update tr color
        saturation_triangle.UpdateResultColorByBarycenters(false);
        this->VMapObjData_Obj(result_triangle.pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData) + sizeof(ZC_GUI_ObjData::color),
            &(result_triangle.pObjData->color));     //  updates from result_triangle->pObjData.color to saturation_triangle.pObjData->color
        ValueChanged();
    };
        //  walk by round red-pink-blue-azure-green-yellow-red
        //      rpbager_step - 255
        //      range        - find     range is range into rpbager_step on color_line
        //  find = range * 255.f / rpbager_step (proportion)
    if (color_line_pos_x == 0) lambUpdateData(255.f, 0.f, 0.f);     //  red
    else if (color_line_pos_x < rpbager_step) lambUpdateData(255.f, 0.f, color_line_pos_x * 255.f / rpbager_step);      //  range (red, pink)
    else if (color_line_pos_x == rpbager_step) lambUpdateData(255.f, 0.f, 255.f);    //  pink
    else if (color_line_pos_x < rbgr_step) lambUpdateData(255.f - ((color_line_pos_x - rpbager_step) * 255.f / rpbager_step), 0.f, 255.f);       //  range (pink, blue)
    else if (color_line_pos_x == rbgr_step) lambUpdateData(0.f, 0.f, 255.f);    //  blue
    else if (color_line_pos_x < rpbager_step * 3.f) lambUpdateData(0.f, (color_line_pos_x - rbgr_step) * 255.f / rpbager_step, 255.f);      //  range (blue, azure)
    else if (color_line_pos_x == rpbager_step * 3.f) lambUpdateData(0.f, 255.f, 255.f);    //  azure
    else if (color_line_pos_x < rpbager_step * 4.f) lambUpdateData(0.f, 255.f, 255.f - ((color_line_pos_x - (rpbager_step * 3.f)) * 255.f / rpbager_step));     //  range (azure, green)
    else if (color_line_pos_x == rpbager_step * 4.f) lambUpdateData(0.f, 255.f, 0.f);      //  green
    else if (color_line_pos_x < rpbager_step * 5.f) lambUpdateData((color_line_pos_x - (rpbager_step * 4.f)) * 255.f / rpbager_step, 255.f, 0.f);    //  range (green, yellow)
    else if (color_line_pos_x == rpbager_step * 5.f) lambUpdateData(255.f, 255.f, 0.f);     //  yellow
    else lambUpdateData(255.f, 255.f - ((color_line_pos_x - (rpbager_step * 5.f)) * 255.f / rpbager_step), 0.f);      //  range (yellow, red)
}

float ZC_GUI_ColorManipulator::GetTriangleSize()
{                   
    if (distance_buttons_y == 0) distance_buttons_y = ZC_GUI_TextManager::GetFontHeight() / 2.f;
    static const float triangle_size = (ZC_GUI_TextManager::GetFontHeight() * lines_with_buttons) + (distance_buttons_y * (lines_with_buttons - 1));
    return triangle_size;
}

bool ZC_GUI_ColorManipulator::IsActiveUCharButtons()
{
    return bnt_red_uchar.VIsDrawing_Obj();
}

void ZC_GUI_ColorManipulator::UpdateSaturationAndResult(float color_line_offset)
{
    float cursor_color_line_pos_x = color_line_offset >= 0 ? this->Get_bl_Obj()[0] + color_line_offset : this->Get_bl_Obj()[0] + this->VGetWidth_Obj() + color_line_offset;
    SetCursorColorLinePosition({ cursor_color_line_pos_x, GetCursorColorLinePosition()[1] }, false);

    this->VSubDataBL_Obj(cursor_color_line.Get_pBL_start(), saturation_triangle.cursor_saturation.VGet_pBL_end());      //  need upadte bl for cursors: color_lien and saturation
        
    result_triangle.pObjData->color = IsActiveUCharButtons() ? ZC_PackColorUcharToUInt_RGBA(bnt_red_uchar.GetNumber(), bnt_green_uchar.GetNumber(), bnt_blue_uchar.GetNumber(), bnt_alpha.GetNumber() * 255.f)
        : ZC_PackColorFloatToUInt_RGBA(bnt_red_float.GetNumber(), bnt_green_float.GetNumber(), bnt_blue_float.GetNumber(), bnt_alpha.GetNumber());
    this->VMapObjData_Obj(result_triangle.pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData) + sizeof(ZC_GUI_ObjData::color), &(result_triangle.pObjData->color));     //  updates from result_triangle->pObjData.color to saturation_triangle.pObjData->color

}

void ZC_GUI_ColorManipulator::UpdateResultColorAndNumbers(float r, float g, float b, bool updateGPU)
{
    if (IsActiveUCharButtons())
    {
        bnt_red_uchar.SetNumber(r, false);
        bnt_green_uchar.SetNumber(g, false);
        bnt_blue_uchar.SetNumber(b, false);
    }
    else
    {
        bnt_red_float.SetNumber(r / 255.f, false);
        bnt_green_float.SetNumber(g / 255.f, false);
        bnt_blue_float.SetNumber(b / 255.f, false);
    }

    result_triangle.pObjData->color = ZC_PackColorUcharToUInt_RGBA(r, g, b, bnt_alpha.GetNumber() * 255);
    if (updateGPU) this->VMapObjData_Obj(result_triangle.pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(result_triangle.pObjData->color));
    ValueChanged();
}

void ZC_GUI_ColorManipulator::SetCursorColorLinePosition(const ZC_Vec2<float>& bl, bool updateGPU)
{
    cursor_color_line.VSet_pBL_Obj({ bl[0] - (cursor_color_line.VGetWidth_Obj() / 2.f), bl[1] });
    if (updateGPU) this->VSubDataBL_Obj(cursor_color_line.pBL, cursor_color_line.pBL);
}

ZC_Vec2<float> ZC_GUI_ColorManipulator::GetCursorColorLinePosition()
{
    ZC_Vec2<float> bl = cursor_color_line.Get_bl_Obj();
    return { bl[0] + (cursor_color_line.VGetWidth_Obj() / 2.f), bl[1] };
}

void ZC_GUI_ColorManipulator::ChangedAlphaInButton(float alpha, bool use_callback)
{
    result_triangle.pObjData->color = IsActiveUCharButtons() ? ZC_PackColorUcharToUInt_RGBA(bnt_red_uchar.GetNumber(), bnt_green_uchar.GetNumber(), bnt_blue_uchar.GetNumber(), alpha * 255.f)
        : ZC_PackColorFloatToUInt_RGBA(bnt_red_float.GetNumber(), bnt_green_float.GetNumber(), bnt_blue_float.GetNumber(), alpha);
    this->VMapObjData_Obj(result_triangle.pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(result_triangle.pObjData->color));
    if (use_callback) ValueChanged();
}

void ZC_GUI_ColorManipulator::MakeActiveButtonType(bool makeActiveUChar)
{
    if ((makeActiveUChar && IsActiveUCharButtons()) || (!makeActiveUChar && !IsActiveUCharButtons())) return;   //  state allready seted
    if (IsActiveUCharButtons())
    {
        bnt_red_uchar.VSetDrawState_Obj(false, false);
        bnt_green_uchar.VSetDrawState_Obj(false, false);
        bnt_blue_uchar.VSetDrawState_Obj(false, false);

        bnt_red_float.VSetDrawState_Obj(true, false);
        bnt_green_float.VSetDrawState_Obj(true, false);
        bnt_blue_float.VSetDrawState_Obj(true, false);

        bnt_red_float.SetNumber(bnt_red_uchar.GetNumber() / 255.f, false);
        bnt_green_float.SetNumber(bnt_green_uchar.GetNumber() / 255.f, false);
        bnt_blue_float.SetNumber(bnt_blue_uchar.GetNumber() / 255.f, false);

        bmt_uchar.SetColor_Obj(bmt_uchar.colorsButton.color_button, false);   //  move active color
        bmt_float.SetColor_Obj(bmt_float.colorsButton.color_button_pressed, false);   //  set active color
    }
    else
    {
        bnt_red_float.VSetDrawState_Obj(false, false);
        bnt_green_float.VSetDrawState_Obj(false, false);
        bnt_blue_float.VSetDrawState_Obj(false, false);

        bnt_red_uchar.VSetDrawState_Obj(true, false);
        bnt_green_uchar.VSetDrawState_Obj(true, false);
        bnt_blue_uchar.VSetDrawState_Obj(true, false);

        bnt_red_uchar.SetNumber(bnt_red_float.GetNumber() * 255.f, false);
        bnt_green_uchar.SetNumber(bnt_green_float.GetNumber() * 255.f, false);
        bnt_blue_uchar.SetNumber(bnt_blue_float.GetNumber() * 255.f, false);

        bmt_float.SetColor_Obj(bmt_float.colorsButton.color_button, false);   //  move active color
        bmt_uchar.SetColor_Obj(bmt_uchar.colorsButton.color_button_pressed, false);   //  set active color
    }
    VMapObjData_Obj(bnt_red_uchar.Get_pObjData_start(), 0, (bmt_float.VGet_pObjData_end() - bnt_red_uchar.Get_pObjData_start()) * sizeof(ZC_GUI_ObjData),
        bnt_red_uchar.Get_pObjData_start());
}

void ZC_GUI_ColorManipulator::ValueChanged()
{
    if (IsActiveUCharButtons())
    {
        float r = bnt_red_uchar.GetNumber();
        float g = bnt_green_uchar.GetNumber();
        float b = bnt_blue_uchar.GetNumber();
        range_255 ? callback(r, g, b, bnt_alpha.GetNumber()) : callback(r / 255.f, g / 255.f, b / 255.f, bnt_alpha.GetNumber());
    }
    else
    {
        float r = bnt_red_float.GetNumber();
        float g = bnt_green_float.GetNumber();
        float b = bnt_blue_float.GetNumber();
        range_255 ? callback(r * 255.f, g * 255.f, b * 255.f, bnt_alpha.GetNumber()) : callback(r, g, b, bnt_alpha.GetNumber());
    }
}

void ZC_GUI_ColorManipulator::Callback_uchar(uchar)
{
    ChangedColorInButton(&bnt_red_uchar, &bnt_green_uchar, &bnt_blue_uchar, true);
}

void ZC_GUI_ColorManipulator::Callback_float(float)
{
    ChangedColorInButton(&bnt_red_float, &bnt_green_float, &bnt_blue_float, true);
}

void ZC_GUI_ColorManipulator::Callback_alpha(float alpha)
{
    ChangedAlphaInButton(alpha, true);
}

    
    //  Saturation

ZC_GUI_ColorManipulator::Saturation::Saturation(float size)
    : ZC_GUI_ObjComposite(ZC_GUI_ObjData(size, size, ZC_PackColorUCharToUInt_RGB(255, 0, 0), {}, ZC_GUI_Bindings::bind_ColorManipulator_saturationTrianlge)),
    cursor_saturation(ZC_GUI_ObjData(3.f, 3.f, 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons))
{
    this->VAddObj_Obj(&cursor_saturation);
}

ZC_GUI_ColorManipulator::Saturation::Saturation(Saturation&& s)
    : ZC_GUI_ObjComposite(static_cast<ZC_GUI_ObjComposite&&>(s)),
    cursor_saturation(static_cast<ZC_GUI_Obj&&>(s.cursor_saturation))
{
    this->VAddObj_Obj(&cursor_saturation);
}


void ZC_GUI_ColorManipulator::Saturation::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{
    *(this->pBL) = _bl;
    SetCursorSaturationPosition({ _bl[0] + this->VGetWidth_Obj(), _bl[1] + this->GetHeight() }, false);
}

bool ZC_GUI_ColorManipulator::Saturation::VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept
{
    return true;
}

bool ZC_GUI_ColorManipulator::Saturation::VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj *&rpObj, ZC_GUI_Obj *&rpScroll)
{
    if (!IsPointBelongTriangle(x, y)) return false;
    rpObj = this;
    return true;
}

bool ZC_GUI_ColorManipulator::Saturation::VMouseButtonLeftDown_Obj(float time)
{       //  here changes cursor position in saturation triangle. Need change numbers in bmt(s) and result_triangle color
    if (isMBL_pressed) return false;
    isMBL_pressed = true;

    float cursor_x = 0.f;
    float cursor_y = 0.f;
    ZC_SWindow::GetCursorPosition(cursor_x, cursor_y);
    ZC_Vec2<float> cursor_pos(cursor_x, cursor_y);
    if (cursor_pos == GetSaturationCursorPosition()) return true;    //  same pos

    ZC_Vec2<float> tl, br, tr;
    GetTriangleCoords(tl, br, tr);
    barycenters = ZC_CalculateTriangleBarycenters(tl, br, tr, cursor_pos);    //  update barycenters

    SetCursorSaturationPosition(cursor_pos, true);
    UpdateResultColorByBarycenters(true);

    return true;
}

void ZC_GUI_ColorManipulator::Saturation::VCursorMove_Obj(float rel_x, float rel_y)
{
    ZC_Vec2<float> sat_cursor_pos = GetSaturationCursorPosition();
    ZC_Vec2<float> new_sat_cursor_pos(sat_cursor_pos[0] + rel_x, sat_cursor_pos[1] + rel_y);

    ZC_Vec2<float> tl, br, tr;
    GetTriangleCoords(tl, br, tr);
        //  ckeck is new_sat_cursor_pos out of quad's border, if need back to quad's border, point still can be not at actual triangle tl-br-tr, but at tl-bl-br, that case covers later     in pointBelongTriangle check
        //  y check
    if (tl[1] < new_sat_cursor_pos[1]) new_sat_cursor_pos[1] = tl[1];   //  new pos under triangle, back to tl-tr line
    else if (br[1] > new_sat_cursor_pos[1]) new_sat_cursor_pos[1] = br[1];      //  new line under br.y, back to br.y  (still may be out trinalge in triangle wich not exists tl-bl-br)
        //  x check
    if (tl[0] > new_sat_cursor_pos[0]) new_sat_cursor_pos[0] = tl[0];   //  new pos lefter then tl.x, back to tl.x (still may be out trinalge in triangle wich not exists tl-bl-br)
    else if  (br[0] < new_sat_cursor_pos[0]) new_sat_cursor_pos[0] = br[0];     //  new pos righter then br.x, back to br.x

    if (sat_cursor_pos == new_sat_cursor_pos) return;   //  new point backs to start point

    if (!ZC_IsPointBelongTriangle_2D(tl, br, tr, new_sat_cursor_pos, barycenters))   //  new point out of triangle tl-br-tr (recalculate barycenters)
    {       //  find new point on intersection of lines (new and old point) with (tl br)
        ZC_FindLineIntercectionPoint_2D(tl, br, sat_cursor_pos, new_sat_cursor_pos, new_sat_cursor_pos);    //  set intercect point in new_sat_cursor_pos
        ZC_CalculateTriangleBarycenters(tl, br, tr, new_sat_cursor_pos);    //  recalculate barycenters
    }

    SetCursorSaturationPosition(new_sat_cursor_pos, true);
    UpdateResultColorByBarycenters(true);
}

void ZC_GUI_ColorManipulator::Saturation::VMouseButtonLeftUp_Obj(float time)
{
    isMBL_pressed = false;
}

bool ZC_GUI_ColorManipulator::Saturation::IsPointBelongTriangle(float x, float y)
{
    ZC_Vec2<float> tl, br, tr;
    GetTriangleCoords(tl, br, tr);
    return ZC_IsPointBelongTriangle_2D(tl, br, tr, { x, y });
}

void ZC_GUI_ColorManipulator::Saturation::SetCursorSaturationPosition(const ZC_Vec2<float>& bl, bool updateGPU)
{
    cursor_saturation.VSet_pBL_Obj({ bl[0] - (cursor_saturation.VGetWidth_Obj() / 2.f), bl[1] - (cursor_saturation.GetHeight() / 2.f) });
    if (updateGPU) this->VSubDataBL_Obj(cursor_saturation.pBL, cursor_saturation.pBL);
}

ZC_Vec2<float> ZC_GUI_ColorManipulator::Saturation::GetSaturationCursorPosition()
{
    ZC_Vec2<float> bl = cursor_saturation.Get_bl_Obj();
    return { bl[0] + (cursor_saturation.VGetWidth_Obj() / 2.f), bl[1] + (cursor_saturation.GetHeight() / 2.f) };
}

bool ZC_GUI_ColorManipulator::Saturation::SetColorTR(float _tr_r, float _tr_g, float _tr_b)
{
    if (tr_r == _tr_r && tr_g == _tr_g && tr_b == _tr_b) return false;
    tr_r = _tr_r;
    tr_g = _tr_g;
    tr_b = _tr_b;
    this->pObjData->color = ZC_PackColorUCharToUInt_RGB(tr_r, tr_g, tr_b);
    return true;
}

void ZC_GUI_ColorManipulator::Saturation::UpdateResultColorByBarycenters(bool updateGPU)
{
        // https://vscode.ru/prog-lessons/rasterizatsiya-treugolnikov-c.html#google_vignette
        //  tl(white) br(black)
        //  255 * u + 0 * v + tr_r * w = r
        //  255 * u + 0 * v + tr_g * w = g
        //  255 * u + 0 * v + tr_b * w = b
    float tl_color = 255.f * barycenters[0];      //  tl r,g,b is 255, so all colors will have same result with barycenter of tl
        //  no need to calculate br baricenter caurse all it's r,g,b is 0
    float r = tl_color + tr_r * barycenters[2];
    float g = tl_color + tr_g * barycenters[2];
    float b = tl_color + tr_b * barycenters[2];
    dynamic_cast<ZC_GUI_ColorManipulator*>(this->pObjHolder)->UpdateResultColorAndNumbers(r, g, b, updateGPU);
}

float ZC_GUI_ColorManipulator::Saturation::FindSecondColorThroughtBarycenters(float first, float second, float third)
{
    ZC_Vec2<float> tl, br, tr;
    GetTriangleCoords(tl, br, tr);
    const float saturation_max = 255.f;
    float x_coef = third / saturation_max;
    float x_offset = this->VGetWidth_Obj() * x_coef;    //  offset from tr to tl
    float y_coef = first / saturation_max;
    float y_offset = this->GetHeight() * y_coef;    //  offset from br to tr
    ZC_Vec2<float> sat_cursor(tr[0] - x_offset, br[1] + y_offset);   //  point in triangle, no need to calculate ral point pos
    barycenters = ZC_CalculateTriangleBarycenters(tl, br, tr, sat_cursor);
    SetCursorSaturationPosition(sat_cursor, false);
    return (second - 255.f * barycenters[0]) / barycenters[2];
}

void ZC_GUI_ColorManipulator::Saturation::CalculateBarycenters(float r, float g, float b)
{
        // https://vscode.ru/prog-lessons/rasterizatsiya-treugolnikov-c.html#google_vignette
        //  tl(white) br(black)
        //  255 * u + 0 * v + tr_r * w = r
        //  255 * u + 0 * v + tr_g * w = g
        //  255 * u + 0 * v + tr_b * w = b
        //  w = (g - 255 * u) / tr_g
        //  u = (r * tr_g - tr_r * g) / 255 * (tr_g - tr_r)
        //  v = 1 - u - v
    float u = tr_r != tr_g ? (r * tr_g - tr_r * g) / (255.f * (tr_g - tr_r))        //  tl barycenter, avoid null divisor (tr_r != tr_g)
                : tr_r != tr_b ? (r * tr_b - tr_r * b) / (255.f * (tr_b - tr_r))    //  avoid null divisor (tr_r != tr_b)
                : (b * tr_g - tr_b * g) / (255.f * (tr_g - tr_b));                  //  last variant
    float w = tr_r != 0.f ? (r - 255.f * u) / tr_r          //  tr barycenter, also avoid null divisor
                : tr_g != 0.f ? (g - 255.f * u) / tr_g
                : (b - 255.f * u) / tr_b;
    float v = 1.f - u - w;      //  br barycenter
    barycenters = { u, v, w };

    ZC_Vec2<float> tl, br, tr;
    GetTriangleCoords(tl, br, tr);
    SetCursorSaturationPosition(tl * barycenters[0] + br * barycenters[1] + tr * barycenters[2], false);
}

void ZC_GUI_ColorManipulator::Saturation::GetTriangleCoords(ZC_Vec2<float>& tl, ZC_Vec2<float>& br, ZC_Vec2<float>& tr)
{
    ZC_Vec2<float> bl = this->Get_bl_Obj();
    float right_x = bl[0] + this->VGetWidth_Obj();
    float top_y = bl[1] + this->GetHeight();
    tl = { bl[0], top_y };
    br = { right_x, bl[1] };
    tr = { right_x, top_y };
}


    //  TypeSwitcher

ZC_GUI_ColorManipulator::TypeSwitcher::TypeSwitcher(float width, bool _isUChar)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, ZC_GUI_TextManager::GetFontHeight(), 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_ButtonFlag::ZC_GUI_BF__None),
    ZC_GUI_ButtonMouseText(width, ZC_GUI_TextManager::GetFontHeight(), 0, ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(0.f, ZC_GUI_TFB_Indent::Location::Center),
        _isUChar ? L"255" : L"1.0", true, 0, ZC_GUI_TextAlignment::Center)),
    isUChar(_isUChar)
{}

bool ZC_GUI_ColorManipulator::TypeSwitcher::VMouseButtonLeftDown_Obj(float time)
{
    if (this->bs_mouseButton == BS_Pressed) return false;
    this->bs_mouseButton = BS_Pressed;
    dynamic_cast<ZC_GUI_ColorManipulator*>(this->pObjHolder)->MakeActiveButtonType(isUChar);
    return true;
}

void ZC_GUI_ColorManipulator::TypeSwitcher::VMouseButtonLeftUp_Obj(float time)
{
    this->bs_mouseButton = BS_Released;
}