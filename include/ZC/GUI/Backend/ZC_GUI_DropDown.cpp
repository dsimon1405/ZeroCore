#include "ZC_GUI_DropDown.h"

ZC_GUI_DropDown::ZC_GUI_DropDown(const std::wstring& name, const std::vector<std::wstring>& variants, float width, float height, ZC_GUI_DropDownFlags dropDownFlags, ZC_Function<void(uint)>&& _callback,
        const ZC_GUI_ColorsDropDown& colorsDropDown)
    : ZC_GUI_ButtonBase(ZC_GUI_ObjData(width, height, 0, ZC_GUI_IconUV::quad, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None, colorsDropDown.colorsButton),
    ZC_GUI_ButtonMouseText(width, height, ZC_GUI_BF__None,ZC_GUI_TextForButton(ZC_GUI_TFB_Indent(ZC_GUI_DropDownIcon::GetTextIndentX(),
        ZC_GUI_TFB_Indent::Left), name, true, 0, ZC_GUI_TextAlignment::Left, colorsDropDown.color_text), colorsDropDown.colorsButton),
    isUnderCursorFlag(dropDownFlags & ZC_GUI_DDF__UnderCursor),
    upDropDownIcon(dropDownFlags & ZC_GUI_DDF__DropIcon ? new ZC_GUI_DropDownIcon(colorsDropDown.color_arrow) : nullptr),
    ddVariants(FillVariants(name, variants, width, height)),
    ddWindow(ZC_WOIData(ddVariants.front().VGetWidth_Obj(), this->GetHeight() * ddVariants.size(), 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel),
        isUnderCursorFlag ? ZC_GUI_WF__EscapeClose : ZC_GUI_WF__OutAreaClickClose | ZC_GUI_WF__EscapeClose),
    callback(std::move(_callback))
{
    if (dropDownFlags & ZC_GUI_DDF__DropIcon) this->VSetWidth_Obj(ddWindow.VGetWidth_Obj());     //  need use dipo down window width in button
    else
    {
        float text_indent_x = ZC_GUI_DropDownIcon::GetTextIndentX();
        float width = text_indent_x + this->textForButton.VGetWidth_Obj() + text_indent_x;
        if (this->VGetWidth_Obj() < width) this->VSetWidth_Obj(width);
    }
}

ZC_GUI_DropDown::ZC_GUI_DropDown(ZC_GUI_DropDown&& dd)
    : ZC_GUI_ButtonBase(static_cast<ZC_GUI_ButtonBase&&>(dd)),
    ZC_GUI_ButtonMouseText(static_cast<ZC_GUI_ButtonMouseText&&>(dd)),
    isUnderCursorFlag(dd.isUnderCursorFlag),
    upDropDownIcon(std::move(dd.upDropDownIcon)),
    ddVariants(std::move(dd.ddVariants)),
    ddWindow(ZC_WOIData(ddVariants.front().VGetWidth_Obj(), this->GetHeight() * ddVariants.size(), 0.f, 0.f, ZC_WOIF__X_Right_Pixel | ZC_WOIF__Y_Top_Pixel),
        isUnderCursorFlag ? ZC_GUI_WF__EscapeClose : ZC_GUI_WF__OutAreaClickClose | ZC_GUI_WF__EscapeClose),
    callback(std::move(dd.callback))
{}

std::vector<ZC_GUI_DDVariant<ZC_GUI_DropDown>> ZC_GUI_DropDown::FillVariants(const std::wstring& name, const std::vector<std::wstring>& variants, float width, float height)
{
    float text_indent_x = ZC_GUI_DropDownIcon::GetTextIndentX();
    float name_width = text_indent_x + ZC_GUI_TextManager::CalculateWstrWidth(name) + text_indent_x;      //  text_indent_x - distances from button border left and right to text
    if (upDropDownIcon) name_width += text_indent_x + ZC_GUI_DropDownIcon::width;  //  uses drop down icon
    if (width < name_width) width = name_width;

    for (const std::wstring& var : variants)    //  get longest of variants width
    {
        float var_width = text_indent_x + ZC_GUI_TextManager::CalculateWstrWidth(var) + text_indent_x;      //  text_indent_x - distances from button border left and right to text
        if (width < var_width) width = var_width;
    }

    std::vector<ZC_GUI_DDVariant<ZC_GUI_DropDown>> ddVars;
    ddVars.reserve(variants.size());
    for (const std::wstring& var : variants)    //  get longest of buttons width
        ddVars.emplace_back(this, width, height, var);
    
    return ddVars;
}

void ZC_GUI_DropDown::VSet_pBL_Obj(const ZC_Vec2<float>& _bl)
{       //  make it here to avoid that in move ctr
    if (upDropDownIcon) this->VAddObj_Obj(upDropDownIcon.Get(), nullptr);
    for (ZC_GUI_DDVariant<ZC_GUI_DropDown>& ddVar : ddVariants)
    {
        ddVar.pHolder = this;     //  that not pObjHolder!
        ddWindow.AddRow(ZC_GUI_ObjBorder::Row({}, { &ddVar }));
    }

    *(this->pBL) = _bl;
    this->objs.front()->VSet_pBL_Obj(_bl);  //  set bl for textButton
    if (upDropDownIcon) upDropDownIcon->VSet_pBL_Obj(ZC_Vec2<float>(_bl[0] + this->VGetWidth_Obj() - upDropDownIcon->VGetWidth_Obj(), _bl[1] + (this->GetHeight() - upDropDownIcon->GetHeight()) / 2.f));
}

void ZC_GUI_DropDown::VMoveBL_Obj(float rel_x, float rel_y, int& update_borders)
{
    MoveVec2(rel_x, rel_y, *(this->pBL));
    for (ZC_GUI_Obj* pObj : objs) pObj->VMoveBL_Obj(rel_x, rel_y, update_borders);
    ddWindow.VSetDrawState_W(false);
}

void ZC_GUI_DropDown::VCursorCollisionStart_Obj(float time)
{
    if (isUnderCursorFlag)    //  uses ZC_GUI_DDF__UnderCursor
    {
        if (ddWindow.VIsDrawing_Obj()) return;
            //  button under cursor and window don't drawn, start draw
        UpdatePos_ddWindow();
        ddWindow.VSetDrawState_W(true);
        this->SetButtonColor_BS(this->colorsButton.color_button_pressed, true);   //  while drawing window make button's collor - pressed
    }
    else    //  default behaviour
    {
        if (this->pObjData->color == colorsButton.color_button_pressed) return;     //  button pressed, wait while up
        this->SetButtonColor_BS(this->colorsButton.color_button_under_cursor, true);
    }
}

void ZC_GUI_DropDown::VCursorCollisionEnd_Obj(float time)
{
    if (isUnderCursorFlag)      //  uses ZC_GUI_DDF__UnderCursor
    {
        if (ddWindow.CheckCursorCollision_Obj()) return;    //  cursor abore ddWindow
        ddWindow.VSetDrawState_W(false);    //  cursor not above ddWidnow, stop drawing
        this->SetButtonColor_BS(this->colorsButton.color_button, true);   //  while drawing window make button's collor - pressed
    }
    else    //  default behaviour
    {
        if (this->pObjData->color == colorsButton.color_button_pressed) return;     //  button pressed, wait while up
        this->SetButtonColor_BS(this->colorsButton.color_button, true);
    }
}

bool ZC_GUI_DropDown::VMouseButtonLeftDown_Obj(float time)
{   
    if (isUnderCursorFlag) return false;    //  if ZC_GUI_DDF__UnderCursor, don't use click events (only cursor move events)
    if (this->bs_mouseButton == BS_HoldUntilRelease) return false;  //  don't do anything while uses another button down event
    if (this->bs_mouseButton == BS_Released)
    {
        this->SetButtonColor_BS(this->colorsButton.color_button_pressed, true);
        this->bs_mouseButton = BS_Pressed;
    }
    return true;
}

void ZC_GUI_DropDown::VMouseButtonLeftUp_Obj(float time)
{
    if (isUnderCursorFlag) return;  //  if ZC_GUI_DDF__UnderCursor, don't use click events (only cursor move events)
    if (this->bs_mouseButton == BS_Pressed)
    {
        if (CheckCursorCollision_Obj())
        {
            this->pObjData->color = colorsButton.color_button_under_cursor;
                //  start drawing ddWindow
            UpdatePos_ddWindow();
            ddWindow.VSetDrawState_W(true);
        }
        else this->pObjData->color = colorsButton.color_button;
        VMapObjData_Obj(pObjData, offsetof(ZC_GUI_ObjData, color), sizeof(ZC_GUI_ObjData::color), &(this->pObjData->color));
    }
    this->bs_mouseButton = BS_Released;
}

void ZC_GUI_DropDown::UpdatePos_ddWindow()
{
    ZC_Vec2<float> bl = this->Get_bl_Obj();    //  button's bl
    bl[1] -= ddWindow.GetHeight();     //  ddWindow new bl (under button)
    bl -= ddWindow.Get_bl_Obj();   //  vector to move from ddWindow current bl to new bl
    if (bl != ZC_Vec2<float>()) ddWindow.VCursorMove_Obj(bl[0], bl[1]);     //  if bl.x or bl.y not 0 need to move ddWindow
}

void ZC_GUI_DropDown::VariantChoosed(ZC_GUI_DDVariant<ZC_GUI_DropDown>* pDDVariant_choosed)
{
    if (isUnderCursorFlag) this->SetButtonColor_BS(this->colorsButton.color_button, true);      //  if ZC_GUI_DDF__UnderCursor, set default color for button changed in VCursorCollisionStart_Obj()
    ddWindow.VSetDrawState_W(false);
    callback(pDDVariant_choosed - &(ddVariants.front()));
}