#pragma once

template<typename TCont, typename TVal>
bool ZC_ForwardListErase(TCont& rCont, const TVal& val)
{
    auto prev = rCont.before_begin();
    for (auto rContI = rCont.begin(); rContI != rCont.end(); ++rContI)
    {
        if (*rContI == val)
        {
            rCont.erase_after(prev);
            return true;
        }
        prev = rContI;
    }
    return false;
}