#include <ZC/GUI/ZC_GUI_ObjBorder.h>

ZC_GUI_ObjBorder::ZC_GUI_ObjBorder(const ZC_GUI_ObjData& _objData)
    : ZC_GUI_ObjComposite(_objData),
    pBorder{ new ZC_GUI_Border() }
{
    this->pObjData->depth = 0.f;  //  allways 0 in border, cause plussing with window depth in geom shader
}

ZC_GUI_ObjBorder::~ZC_GUI_ObjBorder()
{
    if (this->isFirstGetDataCall) if(pBorder) delete pBorder;
}

void ZC_GUI_ObjBorder::VSetPosition_Obj(const ZC_Vec2<float>& _bl)
{
    *(this->pBL) = _bl;
    ZC_GUI_Border borderWin = this->pWinHolder->GetBorder();
        //  set bl and check with window border bl
    ZC_Vec2<float> border_bl = _bl;
    if (border_bl[0] < borderWin.bl[0]) borderWin.bl[0];
    if (border_bl[1] < borderWin.bl[1]) borderWin.bl[1];
        //  set tr and check with window border tr
    ZC_Vec2<float> border_tr(_bl[0] + this->pObjData->width, _bl[1] + this->pObjData->height);   //  calculate tr from new position
    if (borderWin.tr[0] < border_tr[0]) border_tr[0] = borderWin.tr[0];
    if (borderWin.tr[1] < border_tr[1]) border_tr[1] = borderWin.tr[1];

    pBorder->bl = border_bl;
        //  check border size, if X or Y of bl greater then tr, border is not valid, set same data in bl and tr for check in geometry shader,
        //  to path only one fragment in fragment shader (can't discard in vertex and geometry shaders, so send to fragment shader only one fragment(pixel))
    pBorder->tr = border_tr[0] < border_bl[0] || border_tr[1] < border_bl[1] ? border_bl : border_tr;
}

void ZC_GUI_ObjBorder::VGetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount)
{
    ++rBordersCount;
    ++rObjsCount;
    for (auto pObj : objs) pObj->VGetBordersAndObjsCount_Obj(rBordersCount, rObjsCount);
}

void ZC_GUI_ObjBorder::VGetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs, std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex)
{
    this->pObjData->borderIndex = static_cast<int>(rBorder.size());    //  minus one is not needed because we get the size before adding border
    ZC_GUI_Border* pBorder_temp = &rBorder.emplace_back(*pBorder);
    
    ZC_Vec2<float>* pBL_temp = &rBLs.emplace_back(*pBL);
    ZC_GUI_ObjData* pObjData_temp = &rObjDatas.emplace_back(*pObjData);

    if (this->isFirstGetDataCall)
    {
        delete pBorder;
        delete this->pBL;
        delete this->pObjData;
        isFirstGetDataCall = false;
    }

    pBorder = pBorder_temp;
    pBL = pBL_temp;
    pObjData = pObjData_temp;

    for (auto pObj : this->objs) pObj->VGetData_Obj(rBorder, rBLs, rObjDatas, this->pObjData->borderIndex);
}

void ZC_GUI_ObjBorder::VRecalculateBorder_Obj()
{
    VSetPosition_Obj(*(this->pBL));
}
