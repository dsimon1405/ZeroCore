#pragma once

#include <ZC/GUI/ZC_GUI_Obj.h>

#include <list>
#include <algorithm>

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
    std::list<Row> rows;
    bool isScrollable;
    float scroll_y = 0.f;

    ZC_GUI_ObjBorder(const ZC_GUI_ObjData& _objData, bool _isScrollable);
    ~ZC_GUI_ObjBorder();

    bool VIsDrawing_Obj() const noexcept override;

    const Row* AddRow(const Row& row, const Row* pRow_prev = nullptr);
    void EraseRow(const Row* row);

    bool VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prevObj = nullptr) override;
    bool AddObjInRow_Obj(const Row* pRow, ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prevObj = nullptr);
    void VEraseObj_Obj(ZC_GUI_Obj* pObj) override;

    const ZC_GUI_Border& VGetBorder_Obj() override;
    void VRecalculateBorder_Obj(const ZC_GUI_Border& outer_border) override;
    void CalculateInternalBorder(const ZC_GUI_Border& outer_border);

    void VConf_Set_bl_Obj(const ZC_Vec2<float>& pos) override;
    void VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount) override;
    void VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex,
        std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs) override;

    bool VIsUseScrollEvent_Obj() const noexcept override;

    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
    bool VCheckCursorCollision_Obj(float x, float y) override;

        //  uses to erase added ZC_ButtonKeyboard object before configuration, overrides in ZC_GUI_Window
    virtual void VEraseFrom__buttonKeyboard_objs_B(ZC_GUI_Obj* pDelete);
};

typedef typename ZC_GUI_ObjBorder::Row::RowParams ZC_GUI_RowParams;
typedef typename ZC_GUI_ObjBorder::Row ZC_GUI_Row;