#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonNumberText.h>
#include <ZC/GUI/Backend/System/ZC_GUI.h>
#include <ZC/Tools/Function/ZC_Function.h>

#include <concepts>

template <typename T>
concept ZC_cGUI_UCharOrFloat = std::same_as<T, uchar> || std::same_as<T, float>;

//  Class for manipulation with rgba channels of color.
class ZC_GUI_ColorManipulator : public ZC_GUI_ObjComposite
{
public:
    /*
    Params:
    - _callback - calls if one of value changed. Params: red, green, blue, alpha. Colors red, gree, blue may be in range [0, 255] or [0, 1], look next parameter. Alpha always in range [0, 1].
    - range_255 - if true, callback r,g,b params will have values in range [0, 255], otherwise in range [0, 1]. No metter in wich range data was input from gui.
    */
    ZC_GUI_ColorManipulator(ZC_Function<void(float,float,float,float)>&& _callback, bool _range_255);

    ZC_GUI_ColorManipulator(ZC_GUI_ColorManipulator&& cm);

    template <ZC_cGUI_UCharOrFloat Trgb>
    void SetColor(Trgb* pRed, Trgb* pGreen, Trgb* pBlue, bool use_callback);

    void SetAlpha(float alpha, bool use_callback);

private:
    struct Saturation : public ZC_GUI_ObjComposite
    {
        bool isMBL_pressed = false;
        ZC_GUI_Obj cursor_saturation;   //  cursor of color line (color line is this)
            //  store colors in range [0,255], but in float type for calculation operations
        float tr_r = 255.f;   //  on start users red color
        float tr_g = 0.f;
        float tr_b = 0.f;
        ZC_Vec3<float> barycenters { 0.f, 0.f, 1.f };   //  tl(u white), br(v black), tr(w color_line), on start w have largest barycenter

        Saturation(float size);

        Saturation(Saturation&& s);

        void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
        bool VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept override;
        bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj *&rpObj, ZC_GUI_Obj *&rpScroll) override;
        bool VMouseButtonLeftDown_Obj(float time) override;
        void VCursorMove_Obj(float rel_x, float rel_y) override;
        void VMouseButtonLeftUp_Obj(float time) override;
        bool IsPointBelongTriangle(float x, float y);
        void SetCursorSaturationPosition(const ZC_Vec2<float>& bl, bool updateGPU);
        ZC_Vec2<float> GetSaturationCursorPosition();
            //  takes new r,g,b in float but range [0,255] for top right corner in saturation triangle. Return true if color was updated, otherwise false
        bool SetColorTR(float _tr_r, float _tr_g, float _tr_b);
            //  calculates result_triangle color from barycenters and call ZC_GUI_ColorManipulator::UpdateResultColor()
        void UpdateResultColorByBarycenters(bool updateGPU);
            //  returns second color value on color line. Recalculate barycenters, update saturation cursor position.
        float FindSecondColorThroughtBarycenters(float first, float second, float third);
            //  recalculates barycenters from buttons r,g,b. Update cursor_saturation postiion
        void CalculateBarycenters(float r, float g, float b);
        void GetTriangleCoords(ZC_Vec2<float>& tl, ZC_Vec2<float>& br, ZC_Vec2<float>& tr);
    };

    struct TypeSwitcher : public ZC_GUI_ButtonMouseText
    {
        bool isUChar;

        TypeSwitcher(float width, bool _isUChar);

        bool VMouseButtonLeftDown_Obj(float time) override;
        void VMouseButtonLeftUp_Obj(float time) override;
    };

    static inline float distance_buttons_y = 0.f;   //  vertical (Y) distance between buttons (sets in GetTriangleSize())
    static inline const float lines_with_buttons = 5.f;     //  r, g, b, switch, a,
    static inline float button_width = 50.f;

    bool isMBL_pressed = false;

    bool range_255;

    ZC_GUI_Obj cursor_color_line;   //  cursor of color line (color line is this)
    ZC_GUI_Obj result_triangle;
    ZC_GUI_Obj alpha_triangle;
    Saturation saturation_triangle;
        //  rgb buttons uchar
    ZC_GUI_ButtonNumberText<uchar> bnt_red_uchar;
    ZC_GUI_ButtonNumberText<uchar> bnt_green_uchar;
    ZC_GUI_ButtonNumberText<uchar> bnt_blue_uchar;
        //  rgb buttons float
    ZC_GUI_ButtonNumberText<float> bnt_red_float;
    ZC_GUI_ButtonNumberText<float> bnt_green_float;
    ZC_GUI_ButtonNumberText<float> bnt_blue_float;

    ZC_GUI_ButtonNumberText<float> bnt_alpha;  //  allways float

    TypeSwitcher bmt_uchar;
    TypeSwitcher bmt_float;

    ZC_Function<void(float,float,float,float)> callback;

    float GetDistance(const std::wstring& rgba);
    void AddObjs();
    float VGetHeightComposite_Obj() override;
    float VGetWidthComposite_Obj() override;
    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    bool VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept override;
    bool VIsUseScrollEvent_Obj() const noexcept override;
    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
    bool VMouseButtonLeftDown_Obj(float time) override;
    void VCursorMove_Obj(float rel_x, float rel_y) override;
    void VMouseButtonLeftUp_Obj(float time) override;
    void UpdateColorLineCursorPosition(const ZC_Vec2<float>& cursor_color_line_pos);
    float GetTriangleSize();
    bool IsActiveUCharButtons();
    void UpdateSaturationAndResult(float color_line_offset);
    void UpdateResultColorAndNumbers(float r, float g, float b, bool updateGPU);
    void SetCursorColorLinePosition(const ZC_Vec2<float>& bl, bool updateGPU);
    ZC_Vec2<float> GetCursorColorLinePosition();
    void ChangedAlphaInButton(float alpha, bool uae_callback);
    void MakeActiveButtonType(bool makeActiveUChar);
    void ValueChanged();
    void Callback_uchar(uchar);
    void Callback_float(float);
    void Callback_alpha(float alpha);

    template <ZC_cGUI_UCharOrFloat Trgb>
    void ChangedColorInButton(ZC_GUI_ButtonNumberText<Trgb>* pRed, ZC_GUI_ButtonNumberText<Trgb>* pGreen, ZC_GUI_ButtonNumberText<Trgb>* pBlue, bool use_callback);
};


template <ZC_cGUI_UCharOrFloat Trgb>
void ZC_GUI_ColorManipulator::SetColor(Trgb* pRed, Trgb* pGreen, Trgb* pBlue, bool use_callback)
{
    bool changed = false;

    auto lambSetNumber = [&changed, this](Trgb* pColor, ZC_GUI_ButtonNumberText<uchar>& uchar_button, ZC_GUI_ButtonNumberText<float>& float_button)
    {
        IsActiveUCharButtons() ? uchar_button.SetNumber(std::same_as<Trgb, uchar> ? *pColor : uchar(*pColor * 255.f), false)
                                : float_button.SetNumber(std::same_as<Trgb, float> ? *pColor : float(*pColor) / 255.f, false);
        changed = true;
    };
    if (pRed) lambSetNumber(pRed, bnt_red_uchar, bnt_red_float);
    if (pGreen) lambSetNumber(pGreen, bnt_green_uchar, bnt_green_float);
    if (pBlue) lambSetNumber(pBlue, bnt_blue_uchar, bnt_blue_float);
    if (changed) IsActiveUCharButtons() ? ChangedColorInButton(&bnt_red_uchar, &bnt_green_uchar, &bnt_blue_uchar, use_callback)
                                        : ChangedColorInButton(&bnt_red_float, &bnt_green_float, &bnt_blue_float, use_callback);
}
    
template <ZC_cGUI_UCharOrFloat Trgb>
void ZC_GUI_ColorManipulator::ChangedColorInButton(ZC_GUI_ButtonNumberText<Trgb>* pRed, ZC_GUI_ButtonNumberText<Trgb>* pGreen, ZC_GUI_ButtonNumberText<Trgb>* pBlue, bool use_callback)
{
    auto lambUpdateSaturation = [this](float tr_r, float tr_g, float tr_b, float color_line_offset)
    {
        saturation_triangle.SetColorTR(tr_r, tr_g, tr_b);   //  update tr color of saturation triangle(must be before CalculateBarycenters())
        IsActiveUCharButtons() ? saturation_triangle.CalculateBarycenters(bnt_red_uchar.GetNumber(), bnt_green_uchar.GetNumber(), bnt_blue_uchar.GetNumber())    //  calculate saturation barycenters and update cursor position
            : saturation_triangle.CalculateBarycenters(bnt_red_float.GetNumber() * 255.f, bnt_green_float.GetNumber() * 255.f, bnt_blue_float.GetNumber() * 255.f);
        UpdateSaturationAndResult(color_line_offset);
    };

    if (pRed->GetNumber() == pGreen->GetNumber() && pRed->GetNumber() == pBlue->GetNumber()) lambUpdateSaturation(255.f, 0.f, 0.f, 0.f);
    else
    {
        static const float rbgr_step = this->VGetWidth_Obj() / 3.f;    //  3 distances between r-b-g-r on color line (as round)
        static const float rpbager_step = rbgr_step / 2.f;    //  distances between red-pink-blue-azure-green-yellow-red on color line (as round)

        ZC_GUI_ButtonNumberText<Trgb>* colors[] { pRed, pGreen, pBlue };
        std::sort(colors, colors + 3, [](ZC_GUI_ButtonNumberText<Trgb>* pA, ZC_GUI_ButtonNumberText<Trgb>* pB) { return pA->GetNumber() > pB->GetNumber(); });   //  sort colors descending

        const std::wstring& first_color_letter = colors[0]->GetName_BNT();
        const std::wstring& second_color_letter = colors[1]->GetName_BNT();
            //  two largers colors forming color on color line
        if (colors[0]->GetNumber() == colors[1]->GetNumber())   //  both have value 255 on color line, so color will on pink, azure or yellow color
        {
                //  were set on red-pink-blue-azure-green-yellow-red line
            if (first_color_letter == L"R")    //  colors[0] is Red
            {
                second_color_letter == L"G" ? lambUpdateSaturation(255.f, 255.f, 0.f, rpbager_step * 5.f) : lambUpdateSaturation(255.f, 0.f, 255.f, rpbager_step);
            }
            else if (first_color_letter == L"B")    //  colors[0] is Red
            {
                second_color_letter == L"R" ? lambUpdateSaturation(255.f, 0.f, 255.f, rpbager_step) : lambUpdateSaturation(0.f, 255.f, 255.f, rpbager_step * 3.f);
            }
            else    //  colors[0] is Green
            {
                second_color_letter == L"R" ? lambUpdateSaturation(255.f, 255.f, 0.f, rpbager_step * 5.f) : lambUpdateSaturation(0.f, 255.f, 255.f, rpbager_step * 3.f);
            }
        }
        else
        {
            if (colors[1]->GetNumber() == colors[2]->GetNumber())   //  if colors[1] equal colors[2] stay at colors[0] offset
            {
                first_color_letter == L"R" ? lambUpdateSaturation(255.f, 0.f, 0.f, 0.f)
                    : first_color_letter == L"G" ? lambUpdateSaturation(0.f, 255.f, 0.f, rbgr_step * 2.f)
                    : lambUpdateSaturation(0.f, 0.f, 255.f, rbgr_step);
            }
            else
            {       //  at the same time on colors line presents 2 colors one with value 255 and second with value [0, 255]. After sort first is main color, colors[0] with value 255 on color line
                float color_line_offset = first_color_letter == L"R" ? 0    //  X offset on color line from bl
                                    : first_color_letter == L"B" ? rbgr_step
                                    : rbgr_step * 2.f;
                const float max_color_value = 255.f;

                float r = 0.f;
                float g = 0.f;
                float b = 0.f;
                if (IsActiveUCharButtons())
                {
                    r = bnt_red_uchar.GetNumber();;
                    g = bnt_green_uchar.GetNumber();;
                    b = bnt_blue_uchar.GetNumber();;
                }
                else
                {
                    r = bnt_red_float.GetNumber() * 255.f;
                    g = bnt_green_float.GetNumber() * 255.f;
                    b = bnt_blue_float.GetNumber() * 255.f;
                }
                    //  side where to move second_color_step (right/left) on red-pink-blue-azure-green-yellow-red line
                if (first_color_letter == L"R")    //  colors[0] is Red
                {       //   if second color is Blue move from Red to rigth (pink), otherwise to left (yellow)
                    if (second_color_letter == L"B")
                    {
                        float second_color = saturation_triangle.FindSecondColorThroughtBarycenters(r, b, g);   //  return second by power color. Update saturation barycenters and cursor position
                        saturation_triangle.SetColorTR(255.f, 0.f, second_color);
                            //                     proportion
                            //             rpbager_step - max_color_value
                            //  find(second_color_step) - second_color
                            //  find = rpbager_step * second_color / max_color_value
                        float second_color_step = rpbager_step * second_color / max_color_value;    //  width on wich to move from colors[0]
                        UpdateSaturationAndResult(color_line_offset + second_color_step);
                    }
                    else
                    {
                        float second_color = saturation_triangle.FindSecondColorThroughtBarycenters(r, g, b);   //  return second by power color. Update saturation barycenters and cursor position
                        saturation_triangle.SetColorTR(255.f, second_color, 0.f);
                        float second_color_step = rpbager_step * second_color / max_color_value;    //  width on wich to move from colors[0]
                        UpdateSaturationAndResult(color_line_offset - second_color_step);
                    }
                }
                else if (first_color_letter == L"B")    //  colors[0] is Blue
                {      //   if second color is Red move from Blue to left (pink), otherwise to rigth (azure)
                    if (second_color_letter == L"R")
                    {
                        float second_color = saturation_triangle.FindSecondColorThroughtBarycenters(b, r, g);   //  return second by power color. Update saturation barycenters and cursor position
                        saturation_triangle.SetColorTR(second_color, 0.f, 255.f);
                        float second_color_step = rpbager_step * second_color / max_color_value;    //  width on wich to move from colors[0]
                        UpdateSaturationAndResult(color_line_offset - second_color_step);
                    }
                    else
                    {
                        float second_color = saturation_triangle.FindSecondColorThroughtBarycenters(b, g, r);   //  return second by power color. Update saturation barycenters and cursor position
                        saturation_triangle.SetColorTR(0.f, second_color, 255.f);
                        float second_color_step = rpbager_step * second_color / max_color_value;    //  width on wich to move from colors[0]
                        UpdateSaturationAndResult(color_line_offset + second_color_step);
                    }
                }
                else    //  colors[0] is Green
                {      //   if second color is Red move from Green to rigth (yellow), otherwise to left (azure)
                    if (second_color_letter == L"R")
                    {
                        float second_color = saturation_triangle.FindSecondColorThroughtBarycenters(g, r, b);   //  return second by power color. Update saturation barycenters and cursor position
                        saturation_triangle.SetColorTR(second_color, 255.f, 0.f);
                        float second_color_step = rpbager_step * second_color / max_color_value;    //  width on wich to move from colors[0]
                        UpdateSaturationAndResult(color_line_offset + second_color_step);
                    }
                    else
                    {
                        float second_color = saturation_triangle.FindSecondColorThroughtBarycenters(g, b, r);   //  return second by power color. Update saturation barycenters and cursor position
                        saturation_triangle.SetColorTR(0.f, 255.f, second_color);
                        float second_color_step = rpbager_step * second_color / max_color_value;    //  width on wich to move from colors[0]
                        UpdateSaturationAndResult(color_line_offset - second_color_step);
                    }
                }
            }
        }
    }
    if (use_callback) ValueChanged();
}