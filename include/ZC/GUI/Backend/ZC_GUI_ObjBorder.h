#pragma once

#include <ZC/GUI/Backend/Button/ZC_GUI_ButtonMouse.h>
#include <ZC/Tools/ZC_uptr.h>
#include <ZC/GUI/Backend/Config/ZC_GUI_Colors.h>

#include <list>

struct ZC_GUI_ObjBorder : public ZC_GUI_Obj
{
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
            /*
            Params:
            - _indent_x - indent from Left/Right (_indentFlag_X) of Object (window) border. No metter if choose Indent_X::Center.
            - _indentFlag_X - from what border take _indent_x.
            - _indent_y - indent from row above or top border.
            - _distance_x - distance between objects in row.
            */
            RowParams(float _indent_x, Indent_X _indentFlag_X, float _indent_y, float _distance_x);
        } rowParams;
        std::list<ZC_GUI_Obj*> objs;

        /*
        Params:
        - _rowParams - parameters of the row. 
        - _objs - object to add in row. If uses RowParams:Indent_X::Left or Center at first must be added left objects, if uses Indent_X::Right at first right objects.
        */
        Row(const RowParams& _rowParams, std::list<ZC_GUI_Obj*> _objs = {});

        bool operator == (const Row* pRow) const noexcept;

            //  if uses Indent_X::Left or Center at first must be added left obfect, if uses Indent_X::Right at first right objects
        bool AddObj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pObj_prev = nullptr);
        void SetObjHolder(ZC_GUI_Obj* pObj);
        void CalculateObjs_bl(ZC_Vec2<float>& border_tl, float border_width);
    };

    struct Scroll : public ZC_GUI_ObjComposite  //  easyer to make new kind of mouse button, than to extend ZC_GUI_MouseButton
    {
        struct ColorsScroll
        {
            uint color_scroll_background;
            ZC_GUI_ButtonBase::ColorsButton colors_caret_scroll;

            ColorsScroll(uint _color_scroll_background = 0, const ZC_GUI_ButtonBase::ColorsButton& _colors_caret_scroll = {});
        };

        struct Caret : public ZC_GUI_ButtonMouse
        {
            float height_coef = 0.f;

            Caret(const ColorsScroll& colorsScroll);

            void VCursorCollisionEnd_Obj(float time) override;
            void VCursorMove_Obj(float rel_x, float rel_y) override;
        };

        static inline float scroll_speed = 40.f;
        bool isMBL_pressed;
        static inline const float scroll_width = 4.f;
        float scroll_y = 0.f;
        float caret_move_max = 0.f;
        
        Caret caret;

        Scroll(float height, const ColorsScroll& colorsScroll);

        void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
        bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
        bool VMouseButtonLeftDown_Obj(float time) override;
        void VMouseButtonLeftUp_Obj(float time) override;
        
        void CalculateScrollData(bool updateGPU);
            //  return true if scroll may be used
        bool IsActive() const;
        void MakeScroll(float vertical, float speed, bool callCursorCollisiton);
        void ChangeDrawState(bool needDraw);
        static float GetCursor_Y();
    };

    struct ColorsObjBorder
    {
        Scroll::ColorsScroll colorsScroll;
        uint color_frame;

        ColorsObjBorder(const Scroll::ColorsScroll& _colorsScroll = {}, uint _color_frame = 0);
    };

    struct Frame : public ZC_GUI_ObjComposite
    {
        ZC_GUI_Obj top;
        ZC_GUI_Obj right;
        ZC_GUI_Obj bottom;

        Frame(float frame_width, float objBorder_width, float objBorder_height, uint color);

        void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    };
    
    ZC_GUI_Border* pBorder;
    std::list<Row> rows;
    ZC_uptr<Scroll> upScroll;
    ZC_uptr<Frame> upFrame;

    ZC_GUI_ObjBorder(const ZC_GUI_ObjData& _objData, bool _isScrollable, float frameWidth, const ColorsObjBorder& colorsObjBorder = {});

    ZC_GUI_ObjBorder(ZC_GUI_ObjBorder&& ob);

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

    ZC_GUI_Border& VGetBorder_Obj() override;
    ZC_GUI_Obj* VGetObjBorder_Obj() override;
    void CalculateInternalBorder(const ZC_GUI_Border& outer_border);

    void VSet_pBL_Obj(const ZC_Vec2<float>& _bl) override;
    void VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount) override;
    void VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex,
        std::forward_list<ZC_GUI_Obj*>& rButtonKeyboard_objs) override;
    void VConf_SetTextUV_Obj() override;

    bool VIsUseScrollEvent_Obj() const noexcept override;

    bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll) override;
    bool VCheckCursorCollision_Obj(float x, float y) override;

    void VScroll_Obj(float vertical, float time) override;
    void VNewScrollObj_underCursor_Obj(ZC_GUI_Obj* pObj_underCursor) override;

    void VMoveBL_Obj(float rel_x, float rel_y, int& update_borders) override;
};

typedef typename ZC_GUI_ObjBorder::Row::RowParams ZC_GUI_RowParams;
typedef typename ZC_GUI_ObjBorder::Row ZC_GUI_Row;