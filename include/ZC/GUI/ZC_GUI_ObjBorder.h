#pragma once

#include <ZC/GUI/ZC_GUI_Obj.h>
#include <ZC/GUI/ZC_GUI_ButtonMouse.h>
#include <ZC/Tools/ZC_uptr.h>

#include <list>
#include <algorithm>

#include <ZC/GUI/ZC_GUI_IconUV.h>
#include <ZC/GUI/ZC_GUI_Bindings.h>
struct ZC_GUI_ObjBorder : public ZC_GUI_Obj
{
    ZC_GUI_Border* pBorder;
    struct Row
    {
        struct RowParams
        {
            enum Indent_X
            {
                Left,   //  indent X calculates from the left border
                Right,  //  indent X calculates from right border
                Center  //  ignore RowParams::indent_x. Row's objects in center of the X border (window)
            };
            float indent_x = 0.f;       //  indent from left border of window or border
            Indent_X indentFlag_X = Left;   //  indent specifier for indent_x
            float indent_y = 0.f;       //  indent from previous row, or on first line, indent from top border of window or border
            float distance_x = 0.f;     //  distance between object on the row
            float height = 0.f;   //  rows height

            RowParams() = default;
            RowParams(float _indent_x, Indent_X _indentFlag_X, float _indent_y, float _distance_x);
        } rowParams;
        std::list<ZC_GUI_Obj*> objs;

            //  if uses Indent_X::Left or Center at first must be added left obfects in the _objs, if uses Indent_X::Right at first right objects
        Row(const RowParams& _rowParams, std::list<ZC_GUI_Obj*> _objs = {});

        bool operator == (const Row* pRow) const noexcept;

            //  if uses Indent_X::Left or Center at first must be added left obfect, if uses Indent_X::Right at first right objects
        bool AddObj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev = nullptr);
        void SetObjHolder(ZC_GUI_Obj* pObj);
        void CalculateObjs_bl(ZC_Vec2<float>& border_tl, float border_width);
    };

    struct Scroll : public ZC_GUI_ButtonMouse
    {
        static inline const float scroll_width = 4.f;
        float scroll_y = 0.f;
        float caret_height_coef = 0.f;
        float caret_move_max = 0.f;
        
        ZC_GUI_ButtonMouse caret;

        Scroll(float height)
            : ZC_GUI_ButtonBase(ZC_GUI_ObjData(scroll_width, height, 0, ZC_GUI_IconUV::button, ZC_GUI_Bindings::bind_tex_Icons), ZC_GUI_BF__None),
            ZC_GUI_ButtonMouse(scroll_width, height, ZC_GUI_BF__None),
            caret(scroll_width, 0.f, ZC_GUI_BF_M__CursorMoveOnMBLPress)
        {
            this->VAddObj_Obj(&caret, nullptr);
        }

        void CalculateScrollData(bool updateGPU);
            //  return true if scroll may be used
        bool IsActive();
        void MakeScroll(float vertical);

        void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    };
    
    std::list<Row> rows;
    bool haveFrame;     //  have 2 pixels frame
    static inline const float frameBorder = 2.f;   //  2pixel frame border
    ZC_uptr<Scroll> upScroll;

    ZC_GUI_ObjBorder(const ZC_GUI_ObjData& _objData, bool _isScrollable, bool _haveFrame);
    ~ZC_GUI_ObjBorder();

    ZC_Vec2<float>* VGet_pBL_end() override;
    ZC_GUI_ObjData* VGet_pObjData_end() override;
    ZC_GUI_Obj* VGet_pObj_end() override;

    bool VIsDrawing_Obj() const noexcept override;

    const Row* AddRow(Row&& row, const Row* pRow_prev = nullptr);
    void EraseRow(const Row* row);

    bool VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev = nullptr) override;
    bool AddObjInRow_B(const Row* pRow, ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev = nullptr);
    void VEraseObj_Obj(ZC_GUI_Obj* pObj) override;

    const ZC_GUI_Border& VGetBorder_Obj() override;
    ZC_GUI_Obj* VGetObjBorder_Obj() override;
    void VRecalculateBorder_Obj(const ZC_GUI_Border& outer_border) override;
    void CalculateInternalBorder(const ZC_GUI_Border& outer_border);

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount) override;
    void VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex,
        std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs) override;
    void VConf_SetTextUV_Obj() override;
        
    // bool VChangeObjsDrawState_Obj(bool needDraw, ZC_GUI_Obj* pObj_start, ZC_GUI_Obj* pObj_end, bool& mustBeChanged) override;

    bool VIsUseScrollEvent_Obj() const noexcept override;

    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
    bool VCheckCursorCollision_Obj(float x, float y) override;

    void VScroll_Obj(float vertical, float time) override;
};

typedef typename ZC_GUI_ObjBorder::Row::RowParams ZC_GUI_RowParams;
typedef typename ZC_GUI_ObjBorder::Row ZC_GUI_Row;