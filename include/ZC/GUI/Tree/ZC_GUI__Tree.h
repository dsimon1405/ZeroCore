#pragma once

#include <ZC/GUI/ZC_GUI__Obj.h>
#include <ZC/GUI/Backend/Tree/ZC_GUI_Tree.h>
#include "ZC_GUI__Branch.h"

/*
Creates tree in window with own space and scroll system. In tree may be added ZC_GUI__BranchSimple and ZC_GUI__BranchOpenable (may include other branches). Each branch have method fro override: void VBranchBecameActive(),
wich calls whe nbranch became active (focused).
*/
struct ZC_GUI__Tree : public ZC_GUI__Obj<ZC_GUI_Tree>
{
public:
    ZC_GUI__Tree(float width, float height, const ZC_GUI_ColorsTree& colorsTree = {});

    //  Static method for all trees and branches. Sets maximum count of symbols in name of a branch.
    static void SetMaxSymbolsCount(int count);

    /*
    Adds branche in tree.

    Params:
    - pBranchAdd - pointer to branch to add.
    - pBranchAddTo - pointer to branch to add to. If nullptr pBranchAdd is added in the end of the tree.
    */
    void AddBranch(ZC_GUI_BranchSimple* pBranchAdd, ZC_GUI_BranchSimple* pBranchAddTo);

    /*
    Adds several branches.

    Params:
    - branchesAdd - branches to add.
    - pBranchAddTo - pointer to branch to add to. If nullptr pBranchAdd is added in the end of the tree.
    */
    void AddBranches(const std::vector<ZC_GUI_BranchSimple*>& branchesAdd, ZC_GUI_BranchSimple* pBranchAddTo);
        //  Erase branch.
    void EraseBranch(ZC_GUI_BranchSimple* pBranchDelete);
        //  Erase branches.
    void EraseBranches(const std::vector<ZC_GUI_BranchSimple*>& branchesErase);
    
    /*
    Makes branch active (focused)

    Return:
    true if branch became active
    */
    bool MakeActive(ZC_GUI_BranchSimple* pBranch);
};

/*
Method GetBranch() returns ZC_GUI_BranchSimple* to add in ZC_GUI__Tree.
May be override:
- void VBranchBecameActive()
*/
class ZC_GUI__BranchSimple : public ZC_GUI__Branch<ZC_GUI_BranchSimple>
{
public:
    ZC_GUI__BranchSimple(const std::wstring& name, const ZC_GUI_ColorsBranch& colorsBranch = {});
};

/*
Openable branch may include other branches (must be seted in parameter pBranchAddTo in methods: ZC_GUI__Tree::AddBranch(ZC_GUI_BranchSimple* pBranchAdd, ZC_GUI_BranchSimple* pBranchAddTo)
or ZC_GUI__Tree::AddBranches(const std::vector<ZC_GUI_BranchSimple*>& branchesAdd, ZC_GUI_BranchSimple* pBranchAddTo).
Method GetBranch() returns ZC_GUI_BranchSimple* to add in ZC_GUI__Tree.
May be override:
- void VBranchBecameActive()
*/
class ZC_GUI__BranchOpenable : public ZC_GUI__Branch<ZC_GUI_BranchOpenable>
{
public:
    ZC_GUI__BranchOpenable(const std::wstring& name, bool isOpen, const ZC_GUI_ColorsBranchOpenable& branchOpenableColors = {});
};