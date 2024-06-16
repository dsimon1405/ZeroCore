#include <ZC/GUI/ZC_GUI_ObjComposite.h>

#include <cassert>
#include <algorithm>

ZC_GUI_ObjComposite::ZC_GUI_ObjComposite(const ZC_GUI_ObjData& _objData)
    : ZC_GUI_Obj(_objData) //  composite objects have depth = 0, it will set equal to window depth later
{}

bool ZC_GUI_ObjComposite::VSetWindow_Obj(ZC_GUI_Window* pWindow)
{
    if (pWinHolder && pWindow)
    {
        assert(false);  //  pWinHolder allready setted
        return false;
    }
    for (auto pObj : objs)
    {
        if (!(pObj->VSetWindow_Obj(pWindow)))   //  if set pWinHolder fails in one of obj, set to already setted pWinHolder = nullptr
        {
            for (auto pObj1 : objs)
            {
                if (pObj == pObj1) return false;    //  found obj on wich fail set pWindHolder, may out
                pObj1->VSetWindow_Obj(nullptr);
            }
        }
    }
    pWinHolder = pWindow;
    return true;
}

void ZC_GUI_ObjComposite::VGetBordersAndObjsCount_Obj(GLsizeiptr& rBordersCount, GLsizeiptr& rObjsCount)
{
    for (auto pObj : objs) pObj->VGetBordersAndObjsCount_Obj(rBordersCount, rObjsCount);
}

void ZC_GUI_ObjComposite::VGetData_Obj(std::vector<ZC_GUI_Border>& rBorder, std::vector<ZC_Vec2<float>>& rBLs,
    std::vector<ZC_GUI_ObjData>& rObjDatas, int borderIndex)
{
    for (auto pObj : objs) pObj->VGetData_Obj(rBorder, rBLs, rObjDatas, borderIndex);
}

bool ZC_GUI_ObjComposite::VAddObj_Obj(ZC_GUI_Obj* pObj, ZC_GUI_Obj* pPrevObj)
{
    auto lambAdd = [this, pObj, pPrevObj]()
    {
        auto lambAddHolders = [this, pObj]()
        {
            if (this->pWinHolder && !(pObj->VSetWindow_Obj(this->pWinHolder))) return false;    //  failed add window pointer
            if (!pObj->VSetObjHolder_Obj(this))
            {
                pObj->VSetWindow_Obj(nullptr);  //  return nullptr to pWinHolder
                return false;
            }
            return true;
        };

        if (pPrevObj)
        {
            auto iter_end = objs.cend();
            auto iter = std::find(objs.cbegin(), iter_end, pPrevObj);
            if (iter == iter_end)
            {
                assert(false);  //  there's no pPrevObj in objs;
                return false;
            }
            if (!lambAddHolders()) return false;
            objs.insert(iter, pObj);
        }
        else
        {
            if (!lambAddHolders()) return false;
            objs.emplace_back(pObj);
        }

        return true;
    };
    
    if (this->pWinHolder->VIsConfigured_W())
    {
        if (this->pWinHolder->VIsMutable_W())
        {
            if (!lambAdd()) return false;
                //  if window was already conigured - reconfigure, else window only created and will be configured in ZC_GUI_WinMutable::VDrawMutable()
            if (this->pWinHolder->VIsConfigured_W()) this->pWinHolder->VConfigureWindow_W();
            return true;
        }
        else
        {
            assert(false);  //  can't be added in immutable window after configuration
            return false;
        }
    }
    else return lambAdd();
}

void ZC_GUI_ObjComposite::VEraseObj_Obj(ZC_GUI_Obj* pObj)
{
    auto lambErase = [this, pObj]()
    {
        if (std::erase(objs, pObj) != 1) return false;  //  can't find pObj to erase, return false
        pObj->VSetObjHolder_Obj(nullptr);
        pObj->VSetWindow_Obj(nullptr);
        return true;
    };

    if (this->pWinHolder->VIsConfigured_W())
    {
        if (this->pWinHolder->VIsMutable_W())
        {
            if (!lambErase()) return;
                //  if window was already conigured - reconfigure, else window only created and will be configured in ZC_GUI_WinMutable::VDrawMutable()
            if (this->pWinHolder->VIsConfigured_W()) this->pWinHolder->VConfigureWindow_W();
        }
        else assert(false);  //  can't be erased from immutable window after configuration
    }
    else lambErase();
}

bool ZC_GUI_ObjComposite::VSetObjHolder_Obj(ZC_GUI_Obj* _pObjHolder)
{
    if (this->pObjHolder && _pObjHolder)
    {
        assert(false);  //  pObjHolder allready setted
        return false;
    }
    for (auto pObj : objs)
    {
        if (!(pObj->VSetObjHolder_Obj(_pObjHolder)))   //  if set pObjHolder fails in one of obj, set to already setted pObjHolder = nullptr
        {
            for (auto pObj1 : objs)
            {
                if (pObj == pObj1) return false;    //  found obj on wich fail set pObjHolder, may out
                pObj1->VSetObjHolder_Obj(nullptr);
            }
        }
    }
    this->pObjHolder = _pObjHolder;
    return true;
}