// #pragma once

// #include "ZC_RendererLevelDrawer.h"

// template<typename T1, typename T2, typename T3, typename TData>
// class ZC_RLDForwardLists : public ZC_RendererLevelDrawer
// {
// private:
//     template<typename TFirst, typename TSecond>
//     struct Pair
//     {
//         TFirst first;
//         TSecond second;

//         template<typename TFind>
//         bool operator == (TFind find) const { return first == find; }
//     };

// protected:
//     std::forward_list<Pair<T1, std::forward_list<Pair<T2, std::forward_list<Pair<T3, std::forward_list<TData>>>>>>> rendererSets;

//     void AddInMap(T1 t1, T2 t2, T3 t3, const TData& data);
//     bool EraseFromForwardList(T1 t1, T2 t2, T3 t3, const TData& data);
//     void DrawRendererSets();

// private:
//     template<typename TCont, typename TFind>
//     std::forward_list<TCont>::iterator FindPreviousIterBeforeFind(std::forward_list<TCont>& rCont, const TFind& find);
// };

// #include <cassert>
// #include <ZC/Tools/Container/ZC_ContFunc.h>
// #include <ZC/ErrorLogger/ZC_ErrorLogger.h>

// template<typename T1, typename T2, typename T3, typename TData>
// void ZC_RLDForwardLists<T1, T2, T3, TData>::AddInMap(T1 t1, T2 t2, T3 t3, const TData& data)
// {
//     auto pT1Pair = ZC_Find(rendererSets, t1);
//     if (!pT1Pair)
//         rendererSets.emplace_front(Pair<T1, std::forward_list<Pair<T2, std::forward_list<Pair<T3, std::forward_list<TData>>>>>>{ t1, { { t2, { { t3, { data } } } } } });
//     else
//     {
//         auto pT2Pair = ZC_Find(pT1Pair->second, t2);
//         if (!pT2Pair) pT1Pair->second.emplace_front(Pair<T2, std::forward_list<Pair<T3, std::forward_list<TData>>>>{ t2, { { t3, { data } } } });
//         else
//         {
//             auto pT3Pair = ZC_Find(pT2Pair->second, t3);
//             if (!pT3Pair) pT2Pair->second.emplace_front(Pair<T3, std::forward_list<TData>>{ t3, { data } });
//             else
//             {
//                 assert(!ZC_Find(pT3Pair->second, data));  //  double adding of same data
//                 pT3Pair->second.emplace_front(data);
//             }
//         }
//     }
// }

// template<typename T1, typename T2, typename T3, typename TData>
// bool ZC_RLDForwardLists<T1, T2, T3, TData>::EraseFromForwardList(T1 t1, T2 t2, T3 t3, const TData& data)
// {
//     auto t1Iter = FindPreviousIterBeforeFind(rendererSets, t1);
//     auto t1PrevIter = t1Iter++;
//     assert(t1Iter != rendererSets.end());    //  cant find in ZC_ShProg map
    
//     auto t2Iter = FindPreviousIterBeforeFind(t1Iter->second, t2);
//     auto t2PrevIter = t2Iter++;
//     assert(t2Iter != t1Iter->second.end());    //  cant find in ZC_VAO map
    
//     auto t3Iter = FindPreviousIterBeforeFind(t2Iter->second, t3);
//     auto t3PrevIter = t3Iter++;
//     assert(t3Iter != t2Iter->second.end());    //  cant find in ZC_TexturesHolder map

//     if (!ZC_ForwardListErase(t3Iter->second, data)) ZC_ErrorLogger::Err("EraseFromForwardList(), Can't find ZC_UniformsAndGLDraw!", __FILE__, __LINE__);

//     if (t3Iter->second.empty()) t3PrevIter = t2Iter->second.erase_after(t3PrevIter);    //  need to assign to previous iterator to avoid exception in msvc
//     else return false;

//     if (t2Iter->second.empty()) t2PrevIter = t1Iter->second.erase_after(t2PrevIter);    //  need to assign to previous iterator to avoid exception in msvc
//     else return false;

//     if (t1Iter->second.empty()) t1PrevIter = rendererSets.erase_after(t1PrevIter);    //  need to assign to previous iterator to avoid exception in msvc
//     else return false;

//     return rendererSets.empty();
// }

// template<typename T1, typename T2, typename T3, typename TData>
// template<typename TCont, typename TFind>
// std::forward_list<TCont>::iterator ZC_RLDForwardLists<T1, T2, T3, TData>::FindPreviousIterBeforeFind(std::forward_list<TCont>& rCont, const TFind& find)
// {
//     auto prevIter = rCont.before_begin();
//     for (auto curIter = rCont.begin(); curIter != rCont.end(); ++curIter)
//     {
//         if (*curIter == find) return prevIter;
//         prevIter = curIter;
//     }
//     return prevIter;
// }

// template<typename T1, typename T2, typename T3, typename TData>
// void ZC_RLDForwardLists<T1, T2, T3, TData>::DrawRendererSets()
// {
//     for (auto& t1Pair : rendererSets)
//     {
//         t1Pair.first->ActivateOpenGL();
//         for (auto& t2Pair : t1Pair.second)
//         {
//             if constexpr (std::is_pointer<T2>()) t2Pair.first->ActivateOpenGL();
//             else t2Pair.first.ActivateOpenGL();
//             for (auto& t3Pair : t2Pair.second)
//             {
//                 if constexpr (std::is_pointer<T3>()) t3Pair.first->ActivateOpenGL();
//                 else t3Pair.first.ActivateOpenGL();
//                 for (auto& data : t3Pair.second) data.Draw();
//             }
//         }
//     }
// }