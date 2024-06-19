#pragma once

#include <ZC/Tools/ZC_WindowOrthoIndent.h>
#include <ZC/GUI/ZC_GUI_Obj.h>
#include <ZC/GUI/ZC_GUI_EventObj.h>
#include <ZC/GUI/ZC_GUI_Depth.h>

#include <list>

typedef int ZC_GUI_WinFlags;
enum ZC_GUI_WinFlag
{
    ZC_GUI_WF__Stacionar        = 1,        //  The window is always under open windows (if the window is in focus and not stationary windows overlap it, the stationary window is still drawn under those windows)
    ZC_GUI_WF__NeedDraw         = 1 << 1,   //  Need draw on start
    ZC_GUI_WF__NoBackground     = 1 << 2,   //  Don't draw background (window backgground don't make collisioin with mouse cursor, objects of the window still make collision)
    ZC_GUI_WF__Movable          = 1 << 3,   //  Window can be moved. Start position can be setted in ZC_WOIData parameter (can't be used with ZC_GUI_WF__NoBackground)
    ZC_GUI_WF__Scrollable       = 1 << 4,   //  Can be used scrall (can't be used with ZC_GUI_WF__NoBackground)
};

struct ZC_GUI_RowParams
{
    float indent_x = 0.f;     //  indent from left border of window
    float indent_y = 0.f;     //  indent from previous row, on first line indent top border of window
    float distance_x = 0.f;   //  distance between object on the row
    float height = 0.f; //  row height, if not specified, will be taken from the tallest object added to the row 
};

struct ZC_GUI_WinRow
{
    ZC_GUI_RowParams rowParams;
    std::list<ZC_GUI_Obj*> objs;
};

struct ZC_GUI_Window : public ZC_WindowOrthoIndent1, public ZC_GUI_EventObj
{
    static inline const uint bind_Border = 0;
    static inline const uint bind_BL = 1;
    static inline const uint bind_ObjData = 2;
    static inline const uint bind_Texture = 0;

    bool IsStacionar() const noexcept;
    bool IsBackground() const noexcept;
    bool IsMovable() const noexcept;
    void AddRow(ZC_GUI_RowParams rowParams);
    bool AddObj(ZC_GUI_Obj* pObj);
    void MakeForcused();
    void SetFocuseDepth();

    ZC_GUI_Border GetBorder() const noexcept;
        //  returns true if vectors and GPU buffers allready created
    virtual bool VIsMutable_W() const noexcept = 0;
        //  true if configured, ohterwise false
    virtual bool VIsConfigured_W() const noexcept = 0;
    virtual void VConfigureWindow_W(std::list<ZC_GUI_Window*>* pWindows = nullptr) = 0;
    static void ConfigureImmutableWindow();
    virtual bool VIsDrawing_W() const noexcept = 0;
    virtual void VSetDrawState_W(bool needDraw) = 0;

    virtual void VDrawMutable_W() {}
    static void DrawImmutable();

    virtual void VMapObjData_W(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData) = 0;
    virtual void VSubDataBL_W(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end) = 0;

// protected:
    struct WinDepth
    {
        float activeDepth;
        
        void SetNextDepth(ZC_GUI_Window* pWindow);
    };
    static inline WinDepth winDepth { .activeDepth = ZC_GUI_Depth::windowStacionar };

    ZC_GUI_WinFlags winFlags;
    ZC_GUI_Border* pBorder;
    ZC_Vec2<float>* pBL;    //  start data for latter adding in vectors
    ZC_GUI_ObjData* pObjData;   //  start data for latter adding in vectors

    GLsizeiptr bordersCount = 1,    //  future size of vector (and buffer for that vectors) of borders of heir or static window
        objsCount = 1;          //  future size of vectors (and buffers for that vectors) of bls, depths of heir or static window

    std::list<ZC_GUI_WinRow> rows;     //  each inner list is new line

    ZC_GUI_Window(const ZC_WOIData& woiData, const ZC_UV& uv, ZC_GUI_WinFlags winFlags);






    bool VMakeCursorCollision_EO(float x, float y, ZC_GUI_Window*& rpWindow, ZC_GUI_EventObj*& rpObj) override;
    bool VCheckCursorCollision_EO(float x, float y) override;
    
    void VCursorCollisionStart_EO(float time) override {}
    void VCursorCollisionEnd_EO(float time) override {}

    // void VCursoreMove_EO(float x, float y, float rel_x, float rel_y, float time) override {}
    bool VLeftButtonDown_EO(float time, bool& cursorMoveWhilePressed) override
    {
        cursorMoveWhilePressed = IsMovable();
        MakeForcused();
        return true;
    }
    void VLeftButtonUp_EO(float time) override {}
    bool VRightButtonDown_EO(float time, bool& cursorMoveWhilePressed) override { return true; }
    void VRightButtonUp_EO(float time) override {}
    void VScroll_EO(float vertical, float time) override {}
};

