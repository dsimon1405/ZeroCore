#pragma once

struct ZC_GUI_BranchSimple;

template <typename TBranch>
struct ZC_GUI__Branch
{
    ZC_GUI__Branch(TBranch&& _branch);
    virtual ~ZC_GUI__Branch() = default;

        //  If override, calls when branch begame active (focused).
    virtual void VBranchBecameActive() {}

        //  Return object for ZC_GUI__Tree methods.
    ZC_GUI_BranchSimple* GetBranch();

        //  Changes branch name.
    void UpdateName(const std::wstring& name);

protected:
    struct Branch : public TBranch
    {
        Branch(ZC_GUI__Branch<TBranch>* _pHolder, TBranch&& _branch);

    protected:
        ZC_GUI__Branch<TBranch>* pHolder;

    private:
        void VBecameActive_Br() override;
    };

    Branch branch;
};


    //  ZC_GUI__Branch

template <typename TBranch>
ZC_GUI__Branch<TBranch>::ZC_GUI__Branch(TBranch&& _branch)
    : branch(this, std::move(_branch))
{}

template <typename TBranch>
ZC_GUI_BranchSimple* ZC_GUI__Branch<TBranch>::GetBranch()
{
    return &branch;
}

template <typename TBranch>
void ZC_GUI__Branch<TBranch>::UpdateName(const std::wstring& name)
{
    branch.UpdateName(name);
}


    //  ZC_GUI__Branch::Branch

template <typename TBranch>
ZC_GUI__Branch<TBranch>::Branch::Branch(ZC_GUI__Branch<TBranch>* _pHolder, TBranch&& _branch)
    : ZC_GUI_ButtonBase(dynamic_cast<ZC_GUI_ButtonBase&&>(_branch)),
    TBranch(std::move(_branch)),
    pHolder(_pHolder)
{}

template <typename TBranch>
void ZC_GUI__Branch<TBranch>::Branch::VBecameActive_Br()
{
    pHolder->VBranchBecameActive();        
}