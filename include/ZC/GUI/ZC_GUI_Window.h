#pragma once

#include <ZC/Tools/ZC_WindowOrthoIndent.h>
#include <ZC/GUI/ZC_GUI_Obj.h>
#include <ZC/GUI/ZC_GUI_EventObj.h>
#include <ZC/GUI/ZC_GUI_Depth.h>

#include <list>

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

    void SetFocuseDepth();
    bool IsStacionar() const noexcept;
    void AddRow(ZC_GUI_RowParams rowParams);
    bool VAddObj(ZC_GUI_Obj* pObj);
    void MakeForcused();

    ZC_GUI_Border GetBorder() const noexcept;
        //  returns true if vectors and GPU buffers allready created
    virtual bool VIsMutable_W() const noexcept = 0;
        //  true if configured, ohterwise false
    virtual bool VIsConfigured_W() const noexcept = 0;
    virtual void VConfigureWindow_W() = 0;
    static void ConfigureImmutableWindow();
    virtual bool VIsDrawing_W() const noexcept = 0;
    virtual bool VIsBackground() const noexcept = 0;
    virtual void VSetDrawState(bool needDraw) = 0;

    virtual void VDrawMutable_W() {}
    static void DrawImmutable();

    virtual void VMapObjData_W(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData) = 0;

// protected:
    struct WinDepth
    {
        float activeDepth;
        
        void SetNextDepth(ZC_GUI_Window* pWindow);
    };
    static inline WinDepth winDepth { .activeDepth = ZC_GUI_Depth::windowStacionar };

    ZC_GUI_Border* pBorder;
    ZC_Vec2<float>* pBL;    //  start data for latter adding in vectors
    ZC_GUI_ObjData* pObjData;   //  start data for latter adding in vectors

    GLsizeiptr bordersCount = 1,    //  future size of vector (and buffer for that vectors) of borders of heir or static window
        objsCount = 1;          //  future size of vectors (and buffers for that vectors) of bls, depths of heir or static window

    std::list<ZC_GUI_WinRow> rows;     //  each inner list is new line

    ZC_GUI_Window(const ZC_WOIData& woiData, float depth, const ZC_UV& uv);






    bool VMakeCursorCollision_EO(float x, float y, ZC_GUI_Window*& rpWindow, ZC_GUI_EventObj*& rpObj) override;
    bool VCheckCursorCollision_EO(float x, float y) override;
    
    // bool VIsMovable_EO() override { return false; }
    void VCursorCollisionStart_EO(float time) override {}
    void VCursorCollisionEnd_EO(float time) override {}

    void VCursoreMove_EO(float x, float y, float rel_x, float rel_y, float time) override {}
    bool VLeftButtonDown_EO(float time) override { return true; }
    void VLeftButtonUp_EO(float time) override {}
    bool VRightButtonDown_EO(float time) override { return true; }
    void VRightButtonUp_EO(float time) override {}
    void VScroll_EO(float vertical, float time) override {}
};

