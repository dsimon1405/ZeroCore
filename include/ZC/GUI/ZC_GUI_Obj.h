#pragma once

#include <ZC/GUI/ZC_GUI_ObjData.h>
#include <glad/glad.h>
#include <ZC/Events/ZC_ButtonID.h>

#include <vector>


struct ZC_GUI_Obj
{
    enum ButtonState
    {
        BS_Pressed,
        BS_Released
    };

    ZC_GUI_Obj* pObjHolder = nullptr;
        //  put here unsafe RAM pointers in ctr with params, then change them on pointers from vectors in VGetData()
    ZC_Vec2<float>* pBL = nullptr;
    ZC_GUI_ObjData* pObjData = nullptr;

    bool isFirstGetDataCall = true;

    ZC_GUI_Obj(const ZC_GUI_ObjData& _objData);

    virtual ~ZC_GUI_Obj();

    ZC_Vec2<float> GetPosition_bl_Obj();
    float GetWidth();
    float GetHeight();
    void SetObjHolder(ZC_GUI_Obj* _pObjHolder);
        //  returns true if window of the object is drawing, otherwise false
    bool IsWindowDrawing_Obj() const noexcept
    {
        return pObjHolder ? pObjHolder->IsWindowDrawing_Obj() : VIsDrawing_Obj();
    };
        //  returns pointer on the highest object holder (must be window)
    ZC_GUI_Obj* GetWindow()
    {
        return pObjHolder ? pObjHolder->GetWindow() : this;
    }
    bool IsWindowFocused();
    void MakeWindowFocused();
    
    virtual bool VIsStacionar_Obj() const noexcept;    //  override in ZC_GUI_Win...
    virtual bool VIsDrawing_Obj() const noexcept = 0;

    virtual bool VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pPrevObj) { return false; }
    virtual void VEraseObj_Obj(ZC_GUI_Obj* pObj) {}
    
    virtual const ZC_GUI_Border& VGetBorder_Obj();
        //  to avoid useless call VSetPosition() for all objects, uses when total changing ZC_GUI_Window::bls vector (for example on scrolling window or changing window size), to recalculate border.
        //  Don't forget to bufferSubData all borders in window (among window border) after that call!!!!!!!!!
    virtual void VRecalculateBorder_Obj(const ZC_GUI_Border& outer_border) {}
    
    virtual void VConfigure_Obj();    //  override in ZC_GUI_Win...
        //  true if configured, ohterwise false
    virtual bool VIsConfigured_Obj() const noexcept;    //  override in ZC_GUI_Win...
    virtual void VConf_Set_bl_Obj(const ZC_Vec2<float>& _bl);
    virtual void VConf_GetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount);
    virtual void VConf_GetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex);
    virtual void VConf_SetTextUV() {};     //  for text functions, configuration of the text texture done, all text heirs must update object uv from text uv

    virtual void VMapObjData_Obj(ZC_GUI_ObjData* pObjData, GLintptr offsetIn_objData, GLsizeiptr byteSize, void* pData);
    virtual void VSubDataBL_Obj(ZC_Vec2<float>* pBL_start, ZC_Vec2<float>* pBL_end);



            //  EVENT SYSTEM
        //  may be called from overrided VMakeCursorCollision_Obj(), for heirs who want have cursor collision
    bool MakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll);
        //  gets mouse cursor position and check collision
    bool CheckCursorCollision_Obj();
        //  make cursor collision
    bool Collision(float x, float y, float bl_x, float bl_y, float tr_x, float tr_y);

            //  VIRTUAL
        //  calls from ZC_GUI_EventManager::UpdateWindowState() on stop event activity (set default state to event actions), may be override to stop active cursor move or some else events
    virtual void VStopEventActivity_Obj() {}
        //  must return true if object use cursor move event on left mouse button pressed
    virtual bool VIsUseCursorMoveEventOnMBLetfDown_Obj() const noexcept { return false; }
        //  if cursor on object and object must use scroll event, heir must override and return true
    virtual bool VIsUseScrollEvent_Obj() const noexcept { return false; }

            //  EVENTS
    //  return true on collision
    virtual bool VMakeCursorCollision_Obj(float x, float y, ZC_GUI_Obj*& rpObj, ZC_GUI_Obj*& rpScroll)  { return false; }
    //  return true if had collision (if window, collision checks only if background drawing) 
    virtual bool VCheckCursorCollision_Obj(float x, float y);

    virtual void VCursorCollisionStart_Obj(float time) {}
    virtual void VCursorCollisionEnd_Obj(float time) {}
        //  Called after calls VMouseButtonLeftDown_Obj(...) or VRightButtonDown_Obj(...) with cursorMoveWhilePressed = true;
        //  Called once per frame (parrams: x, y - have last position; rel_x, rel_y - have sum of all changes in frame).
    virtual void VCursorMove_Obj(float rel_x, float rel_y) {}
        //  return false if button must be disable in event system
    virtual bool VMouseButtonLeftDown_Obj(float time) { return true; }
    virtual void VMouseButtonLeftUp_Obj(float time) {}
        //  return false if button must be disable in event system
    virtual bool VRightButtonDown_Obj(float time) { return true; }
    virtual void VRightButtonUp_Obj(float time) {}

        //  return false if button must be disable in event system
    virtual bool VKeyboardButtonDown_Obj(float time) { return true; }
    virtual void VKeyboardButtonUp_Obj(float time) {}
        //  return false if button must be disables
    bool ButtonDown(ZC_ButtonID buttonID, float time);
    void ButtonUp(ZC_ButtonID buttonID, float time);

    virtual void VScroll_Obj(float vertical, float time) {}

    virtual bool VIsButtonKeyboard_Obj() { return false; }
};