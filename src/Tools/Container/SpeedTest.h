

// size_t size =   100;
// size_t search = 82;
// size_t delet =  93;
// size_t delet1 = 77;

// void FL()
// {
//     ZC_Clock cl;
//     std::forward_list<size_t> fl;
//     for (size_t i = 0; i < size; i++)
//     {
//         fl.emplace_front(i);
//     }
//     auto time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout("forward_list\n" + std::to_string(time) + " create and fill\n");

//     cl.Start();
//     for (auto i : fl)
//     {
//         auto q = i;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  full iteration non iter\n");

//     cl.Start();
//     for (auto i : fl)
//     {
//         if (i == search) break;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  find\n");

//     cl.Start();
//     for (auto i = fl.begin(); i != fl.end(); ++i)
//     {
//         if (*i == search) break;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  find iter\n");

//     cl.Start();
//     auto find = std::find(fl.begin(), fl.end(), search);
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  find func\n");

//     cl.Start();
//     auto prev = fl.before_begin();
//     for (auto i = fl.begin(); i != fl.end(); ++i)
//     {
//         if (*i == delet)
//         {
//             fl.erase_after(prev);
//             break;
//         }
//         prev = i;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  erase\n");

//     cl.Start();
//     auto is = std::erase(fl, delet1);
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  erase func\n\n");
// }
// void L()
// {
//     ZC_Clock cl;
//     std::list<size_t> fl;
//     for (size_t i = 0; i < size; i++)
//     {
//         fl.emplace_back(i);
//     }
//     auto time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout("list\n" + std::to_string(time) + " create and fill\n");

//     cl.Start();
//     for (auto i : fl)
//     {
//         auto q = i;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  full iteration non iter\n");

//     cl.Start();
//     for (auto i : fl)
//     {
//         if (i == search) break;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  find\n");

//     cl.Start();
//     for (auto i = fl.begin(); i != fl.end(); ++i)
//     {
//         if (*i == search) break;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  find iter\n");

//     cl.Start();
//     auto find = std::find(fl.begin(), fl.end(), search);
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  find func\n");
//     cl.Start();
//     for (auto i = fl.begin(); i != fl.end(); ++i)
//     {
//         if (*i == delet)
//         {
//             fl.erase(i);
//             break;
//         }
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  erase\n");

//     cl.Start();
//     auto is = std::erase(fl, delet1);
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  erase func\n\n");
// }
// void M()
// {
//     ZC_Clock cl;
//     std::map<size_t, size_t> fl;
//     for (size_t i = 0; i < size; i++)
//     {
//         fl.emplace(i, i);
//     }
//     auto time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout("map\n" + std::to_string(time) + " create and fill\n");

//     cl.Start();
//     for (auto i : fl)
//     {
//         auto q = i.second;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  full iteration non iter\n");

//     auto find = fl.find(search);
//     ZC_cout(std::to_string(time) + "  find\n");

//     cl.Start();
//     size_t erased = fl.erase(delet);
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  erase\n\n");
// }
// void V()
// {
//     ZC_Clock cl;
//     std::vector<size_t> fl;
//     for (size_t i = 0; i < size; i++)
//     {
//         fl.emplace_back(i);
//     }
//     auto time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout("vector\n" + std::to_string(time) + " create and fill\n");

//     cl.Start();
//     for (auto i : fl)
//     {
//         auto q = i;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  full iteration non iter\n");

//     cl.Start();
//     for (auto i : fl)
//     {
//         if (i == search) break;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  find\n");

//     cl.Start();
//     for (auto i = fl.begin(); i != fl.end(); ++i)
//     {
//         if (*i == search) break;
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  find iter\n");

//     cl.Start();
//     auto find = std::find(fl.begin(), fl.end(), search);
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  find func\n");

//     cl.Start();
//     for (auto i = fl.begin(); i != fl.end(); ++i)
//     {
//         if (*i == delet)
//         {
//             fl.erase(i);
//             break;
//         }
//     }
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  erase\n");

//     cl.Start();
//     auto is = std::erase(fl, delet1);
//     time = cl.Time<ZC_Nanoseconds>();
//     ZC_cout(std::to_string(time) + "  erase func\n\n");
// }
//     ZC_cout("size = " + std::to_string(size) +
//             "\nfind = " + std::to_string(search) +
//             "\ndelete1 = " + std::to_string(delet) +
//             "\ndelete2 = " + std::to_string(delet1) + "\n");
//     FL();
//     L();
//     M();
//     V();