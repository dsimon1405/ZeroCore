#include "ZC_GUI__Tree.h"

ZC_GUI__Tree::ZC_GUI__Tree(float width, float height, const ZC_GUI_ColorsTree& colorsTree)
    : ZC_GUI__Obj<ZC_GUI_Tree>(ZC_GUI_Tree(width, height, colorsTree))
{}

void ZC_GUI__Tree::SetMaxSymbolsCount(int count)
{
    ZC_GUI_BranchSimple::SetMaxSymbolsCount(count);
}

void ZC_GUI__Tree::AddBranch(ZC_GUI_BranchSimple* pBranchAdd, ZC_GUI_BranchSimple* pBranchAddTo)
{
    this->obj.AddBranch(pBranchAdd, pBranchAddTo);
}

void ZC_GUI__Tree::AddBranches(const std::vector<ZC_GUI_BranchSimple*>& branchesAdd, ZC_GUI_BranchSimple* pBranchAddTo)
{
    this->obj.AddBranches(branchesAdd, pBranchAddTo);
}

void ZC_GUI__Tree::EraseBranch(ZC_GUI_BranchSimple* pBranchDelete)
{
    this->obj.EraseBranch(pBranchDelete);
}

void ZC_GUI__Tree::EraseBranches(const std::vector<ZC_GUI_BranchSimple*>& branchesErase)
{
    this->obj.EraseBranches(branchesErase);
}

bool ZC_GUI__Tree::MakeActive(ZC_GUI_BranchSimple* pBranch)
{
    return this->obj.MakeActive(pBranch);
}


    //  ZC_GUI__BranchSimple

ZC_GUI__BranchSimple::ZC_GUI__BranchSimple(const std::wstring& name, const ZC_GUI_ColorsBranch& colorsBranch)
    : ZC_GUI__Branch<ZC_GUI_BranchSimple>(ZC_GUI_BranchSimple(name, colorsBranch))
{}


    //  ZC_GUI__BranchOpenable

ZC_GUI__BranchOpenable::ZC_GUI__BranchOpenable(const std::wstring& name, bool isOpen, const ZC_GUI_ColorsBranchOpenable& branchOpenableColors)
    : ZC_GUI__Branch<ZC_GUI_BranchOpenable>(ZC_GUI_BranchOpenable(name, isOpen, branchOpenableColors))
{}