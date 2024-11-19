// #pragma once

// #include <ZC/ZC_Types.h>

// #include <concepts>

// namespace ZC_GUI_Number
// {
//     template <typename T>
//     concept cNumber = std::same_as<T, signed char>
//                     || std::same_as<T, unsigned char>
//                     || std::same_as<T, short>
//                     || std::same_as<T, unsigned short>
//                     || std::same_as<T, int>
//                     || std::same_as<T, unsigned int>
//                     || std::same_as<T, long>
//                     || std::same_as<T, unsigned long>
//                     || std::same_as<T, long long>
//                     || std::same_as<T, unsigned long long>
//                     || std::same_as<T, float>
//                     || std::same_as<T, double>
//                     || std::same_as<T, long double>;


//     template <cNumber TNum>
//     std::wstring NumberToWstr(TNum number, uchar afterDot_count)
//     {
//         if (std::same_as<TNum, float> || std::same_as<TNum, double> || std::same_as<TNum, long double>)
//         {
//             // TNum small_number = 1. / std::pow(10., afterDot_count);
//             // if (-small_number < number && number < small_number)   //  too small number
//             // {
//             //     std::wstring str_null(afterDot_count + 2ull, L'0');
//             //     str_null[1ull] = L'.';
//             //     return str_null;
//             // }

//             l_zc number_integer = number * std::pow(10., ++afterDot_count);     //  all wisible numbers, after dot, pluss one more, move to the left side
//             if (number_integer < 10ll)   //  in integer part now only additional number, caurse lower then 10, all realy wisible number is null, so return null
//             {
//                 std::wstring str_null(afterDot_count + 2ull, L'0');
//                 str_null[1ull] = L'.';
//                 return str_null;
//             }

            

            
//             std::wstring number_wstr = std::to_wstring(number);
//             size_t dot_index = number_wstr.find(L'.');
//             size_t must_beLength = dot_index + 1ull + afterDot_count;
//             if (must_beLength > number_wstr.size())
//             {
//                 number_wstr.append(must_beLength - number_wstr.size(), L'0');
//                 return number_wstr;
//             }
//             else return number_wstr.substr(0, must_beLength);
//         }
//         else return std::to_wstring(number);
//     }
// }