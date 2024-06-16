#include <ZC/GUI/ZC_GUI_Obj.h>

#include <ZC/GUI/ZC_GUI_Window.h>

#include <cassert>

ZC_GUI_Obj::ZC_GUI_Obj(const ZC_GUI_ObjData& _objData)
    : pBL(new ZC_Vec2<float>()),
    pObjData(new ZC_GUI_ObjData(_objData))
{}

ZC_GUI_Obj::~ZC_GUI_Obj()
{
    if (isFirstGetDataCall)
    {
        if(pBL) delete pBL;
        if(pObjData) delete pObjData;
    }
}

ZC_Vec2<float> ZC_GUI_Obj::GetPosition()
{
    return *pBL;
}

float ZC_GUI_Obj::GetWidth()
{
    return pObjData->width;
}

float ZC_GUI_Obj::GetHeight()
{
    return pObjData->height;
}

bool ZC_GUI_Obj::VSetWindow_Obj(ZC_GUI_Window* pWindow)
{
    if (pWinHolder && pWindow)
    {
        assert(false);  //  window allready setted
        return false;
    }
    pWinHolder = pWindow;
    return true;
}

void ZC_GUI_Obj::VSetPosition_Obj(const ZC_Vec2<float>& _bl)    //  override in heirs wich new bl of composite object
{
    *pBL = _bl;
}

void ZC_GUI_Obj::VGetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount)
{
    ++rObjsCount;
}

void ZC_GUI_Obj::VGetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex)
{
    pObjData->borderIndex = borderIndex;
    // bufIndex = rBLs.size();     //  minus one is not needed because we get the size before adding data
    ZC_Vec2<float>* pBL_temp = &rBLs.emplace_back(*pBL);
    ZC_GUI_ObjData* pObjData_temp = &rObjDatas.emplace_back(*pObjData);

    if (this->isFirstGetDataCall)
    {
        delete pBL;
        delete pObjData;
        isFirstGetDataCall = false;
    }

    pBL = pBL_temp;
    pObjData = pObjData_temp;
}

bool ZC_GUI_Obj::VSetObjHolder_Obj(ZC_GUI_Obj* _pObjHolder)
{
    if (pObjHolder && _pObjHolder)
    {
        assert(false);  //  pObjHolder allready setted
        return false;
    }
    pObjHolder = _pObjHolder;
    return true;
}